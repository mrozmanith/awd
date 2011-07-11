import bpy

import pyawd
from pyawd.core import *
from pyawd.anim import *
from pyawd.scene import *
from pyawd.geom import *
from pyawd.material import *
from pyawd.utils import *

import mathutils
from math import degrees


class AWDBlockCache(object):
    '''A cache of already created AWD blocks, and their connection to
        nodes in the Maya DAG. The cache should always be checked before
        creating a blocks, so that blocks can be reused within the file
        when possible.'''
    
    def __init__(self):
        self.__cache = []

    def get(self, path):
        block = None
        for item in self.__cache:
            if item[0] == path:
                block = item[1]
                break

        return block

    def add(self, path, block):
        if self.get(path) is None:
            self.__cache.append((path, block))
        


class BlenderAWDExporter(object):
    def __init__(self, path):
        self.path = path
        self.block_cache = AWDBlockCache()
        self.exported_objects = []
    
    def export(self):
        self.awd = AWD()
        
        for o in bpy.context.scene.objects:
            print(o.type)
            if o.type == 'EMPTY':
                mtx = self.mtx_bl2awd(o.matrix_local)
                ctr = AWDContainer(name=o.name, transform=mtx)
                self.block_cache.add(o, ctr)
                self.exported_objects.append(o)
                
            elif o.type == 'MESH':
                md = self.block_cache.get(o.data)
                if md is None:
                    md = self.build_mesh_data(o.data)
                    self.awd.add_mesh_data(md)
                    self.block_cache.add(o.data, md)
                
                mtx = self.mtx_bl2awd(o.matrix_local)
                inst = AWDMeshInst(data=md, name=o.name, transform=mtx)
                self.block_cache.add(o, inst)
                
                self.exported_objects.append(o)              
      
      
        # Loop through scene objects again and add either directly
        # to the AWD document root or to it's parent if one exists.
        # At this point, an AWD representation of the parent is
        # guaranteed to have been created if included in the export.
        for o in self.exported_objects:
            block = self.block_cache.get(o)
            if o.parent is not None:
                par_block = self.block_cache.get(o.parent)
                par_block.add_child(block)
            else:
                self.awd.add_scene_block(block)
        
        with open(self.path, 'wb') as f:
            self.awd.flush(f)
        
    
    
    def build_mesh_data(self, geom):
        expanded_vertices = []
        vertex_edges = {}
        
        # Create lookup table for edges by vertex, to use
        # when determining if a vertex is on a hard edge
        for e in geom.edges:
            for v in e.vertices:
                if v not in vertex_edges:
                    vertex_edges[v] = []
                vertex_edges[v].append(e)
        
        tex_data = None
        has_uvs = False
        if len(geom.uv_textures):
            has_uvs = True
            tex_data = geom.uv_textures[0].data
        
        # Generate expanded list of vertices
        for f in geom.faces:
            inds_in_face = [0,1,2]
            if len(f.vertices)==4:
                inds_in_face.extend((0,2,3))
                
            for idx in inds_in_face:
                vert = geom.vertices[f.vertices[idx]]
                edges = vertex_edges[vert.index]
                has_hard_edge = False
                for e in edges:
                    if e.use_edge_sharp:
                        has_hard_edge = True
                        break

                uv = None
                if tex_data is not None and len(tex_data)>0:
                    # TODO: Implement secondary UV sets?
                    tex_face = tex_data[f.index]
                    uv = tex_face.uv[idx]
                    
                
                v_data = {
                    'v': vert,
                    'f': f,
                    'uv': uv,
                    'has_hard_edge': has_hard_edge,
                    'normal': f.normal,
                    'normal_influences': [ f ]
                }
                expanded_vertices.append(v_data)


        # Find influences for all vertices
        for v0 in expanded_vertices:
            for v1 in expanded_vertices:
                angle = degrees(v0['normal'].angle(v1['normal']))
                if angle <= geom.auto_smooth_angle:
                    v0['normal_influences'].append(v1['f'])
                    v1['normal_influences'].append(v0['f'])

        
        collapsed_vertices = []
        
        def get_vert_idx(v0):
            idx = -1
            
            for v1 in collapsed_vertices:
                idx += 1    
                
                # If any of the vertices belongs to a hard edge, then
                # their normals must not be averaged, and hence they 
                # must be returned by this function as separate verts
                if v0['has_hard_edge'] or v1['has_hard_edge']:
                    continue
                
                # If position is incorrect, continue looking
                v0pos = v0['v'].co
                v1pos = v1['v'].co
                if v0pos.x!=v1pos.x or v0pos.y!=v1pos.y or v0pos.z!=v1pos.z:
                    continue
                
                # If mesh has uvs and the UVs for this vertex does not 
                # match the one beeing searched for, continue looping
                v0uv = v0['uv']
                v1uv = v1['uv']
                if has_uvs and (v0uv[0]!=v1uv[0] or v0uv[1]!=v1uv[1]):
                    continue
                
                angle = v0['normal'].angle(v1['normal'])
                if degrees(angle) > geom.auto_smooth_angle:
                    continue
                                    
                # Made it here? Then vertices match!
                return idx
                
            return -1
                
        
        vertices = []
        indices = []
        normals = []
        uvs = []
        
        
        for v_data in expanded_vertices:
            idx = get_vert_idx(v_data)
            if idx >= 0:
                indices.append(idx)
            else:
                vertices.append(v_data['v'].co.x)
                vertices.append(v_data['v'].co.z)
                vertices.append(v_data['v'].co.y)
                uvs.extend(v_data['uv'])
                indices.append(len(collapsed_vertices))
                
                # Add to list of collapsed vertices so that future
                # searches can find the vertex
                collapsed_vertices.append(v_data)
                    
        
        # Smooth all vertices
        for v_data in collapsed_vertices:
            if len(v_data['normal_influences'])>1:
                total_area = 0
                accum_norm = mathutils.Vector((0,0,0))
                
                # Calculate weighted average normal
                for inf in v_data['normal_influences']:
                    total_area += inf.area
                    accum_norm += inf.normal*inf.area

                v_data['normal'] = accum_norm/total_area
                    
            normals.append(v_data['normal'].x)
            normals.append(v_data['normal'].z)
            normals.append(v_data['normal'].y)
        
        # Flip all faces
        for idx in range(1, len(indices), 3):
            tmp = indices[idx]
            indices[idx] = indices[idx+1]
            indices[idx+1] = tmp
        
        
        md = AWDMeshData(geom.name)
        md.add_sub_mesh(AWDSubMesh())
        md[0].add_stream(STR_VERTICES, vertices)
        md[0].add_stream(STR_TRIANGLES, indices)
        md[0].add_stream(STR_UVS, uvs)
        md[0].add_stream(STR_VERTEX_NORMALS, normals)
        
        return md
        
    
    def mtx_bl2awd(self, mtx):    
        # Decompose matrix
        pos, rot, scale = mtx.decompose()
        
        # Swap translation axes
        tmp = pos.y
        pos.y = pos.z
        pos.z = tmp
        
        # Swap rotation axes
        tmp = rot.y
        rot.x = -rot.x
        rot.y = -rot.z
        rot.z = -tmp
        
        # Recompose matrix
        mtx = mathutils.Matrix.Translation(pos).to_4x4() * rot.to_matrix().to_4x4()
        
        # Create list from rows
        rows = list(mtx)
        mtx_list = []
        mtx_list.extend(list(rows[0]))
        mtx_list.extend(list(rows[1]))
        mtx_list.extend(list(rows[2]))
        mtx_list.extend(list(rows[3]))
        
        # Apply swapped-axis scale
        mtx_list[0] *= scale.x
        mtx_list[5] *= scale.y
        mtx_list[10] *= scale.z
        
        #print(mtx_list[0:4])
        #print(mtx_list[4:8])
        #print(mtx_list[8:12])
        #print(mtx_list[12:])
        
        return AWDMatrix4x4(mtx_list)
    


exporter = BlenderAWDExporter('blendout.awd')
exporter.export()
