import re
import sys
import math
import os.path

import maya.OpenMaya as om
import maya.cmds as mc
import maya.OpenMayaAnim as omanim
import maya.OpenMayaMPx as OpenMayaMPx
import pymel.core.datatypes

import pyawd
from pyawd.core import *
from pyawd.anim import *
from pyawd.scene import *
from pyawd.geom import *
from pyawd.material import *
from pyawd.utils import *






class MayaAWDFileTranslator(OpenMayaMPx.MPxFileTranslator):
    def writer(self, file, options, mode):
        file_path = file.resolvedFullName()
        base_path = os.path.dirname(file_path)

        opts = self.parse_opts(options)
        print(opts)

        def o(key, defval=None):
            'Get option or default value'
            if key in opts:
                return opts[key]
            else:
                return defval

        sequences = self.read_sequences(o('seqsrc'), base_path)

        with open(file_path, 'wb') as file:
            exporter = MayaAWDExporter(file)
            exporter.export(None, 
                include_animation=int(o('animation', False)),
                include_skeletons=int(o('skeletons', False)),
                sequences=sequences)

    def defaultExtension(self):
        return 'awd'

    def haveWriteMethod(self):
        return True

    def parse_opts(self, opt_str):
        if opt_str[0]==';':
            opt_str=opt_str[1:]

        fields = re.split('(?<!\\\)&', str(opt_str))
        return dict([ re.split('(?<!\\\)=', pair) for pair in fields ])

    def read_sequences(self, seq_path, base_path):
        sequences = []
        if seq_path is not None:
            if not os.path.isabs(seq_path):
                seq_path = os.path.join(base_path, seq_path)
            try:
                with open(seq_path, 'r') as seqf:
                    lines = seqf.readlines()
                    for line in lines:
                        line_fields = re.split('[^a-zA-Z0-9]', line.strip())
                        sequences.append((line_fields[0], int(line_fields[1]), int(line_fields[2])))
            except:
                pass

        return sequences

def ftCreator():
    return OpenMayaMPx.asMPxPtr( MayaAWDFileTranslator() )

def initializePlugin(mobject):
    mplugin = OpenMayaMPx.MFnPlugin(mobject, 'Away3D', '1.0')
    stat = mplugin.registerFileTranslator('AWD2', 'none', ftCreator, 'MayaAWDExporterUI')

    return stat
    
def uninitializePlugin(mobject):
    mplugin = OpenMayaMPx.MFnPlugin(mobject)
    stat = mplugin.deregisterFileTranslator('AWD2')

    return stat



class MayaAWDBlockCache:
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
        


class MayaAWDExporter:
    def __init__(self, file):
        self.file = file
        self.block_cache = MayaAWDBlockCache()
        self.skeleton_paths = []
        self.joint_indices = {}
        self.mesh_vert_indices = {}

        self.awd = AWD(compression=AWD.UNCOMPRESSED)


    def export(self, selection, include_skeletons=False, include_animation=False, sequences=[]):
        # Assume that bind pose is on frame 1
        om.MGlobal.viewFrame(0)
  
        #transforms = mc.ls(sl=True, tr=True, l=True)
        #if not transforms:
        #    mc.error('Nothing selected!')
        #    return
  
        dag_it = om.MItDag(om.MItDag.kDepthFirst)
        while not dag_it.isDone():
            visible = False
            try:
                attr0 = '%s.visibility' % dag_it.partialPathName()
                attr1 = '%s.ovv' % dag_it.partialPathName()
                print('attr: %s' % attr0)
                print('attr: %s' % attr1)
                visible = mc.getAttr(attr0) and mc.getAttr(attr1)
                print(visible)
            except:
                pass

            if visible:
                if dag_it.currentItem().hasFn(om.MFn.kTransform):
                    transform = dag_it.fullPathName()

                    print('')
                    print('================================================')
                    print('export %s' % dag_it.fullPathName())
                    print('================================================')

                    awd_parent = None
                    dag_fn = om.MFnDagNode(dag_it.currentItem())
                    if dag_fn.parentCount() > 0:
                        parent_dag = om.MFnDagNode(dag_fn.parent(0))
                        print('looking in cache for %s ' % parent_dag.fullPathName())
                        awd_parent = self.block_cache.get(parent_dag.fullPathName())

                    shapes = mc.listRelatives(transform, s=True, f=True)
                    if shapes is not None:
                        shape = shapes[0]
                        self.export_mesh(transform, shape, awd_parent)
                    else:
                        # Container!
                        mtx = mc.xform(transform, q=True, m=True)
                        ctr = AWDContainer(name=dag_it.partialPathName(), transform=self.mtx_list2awd(mtx))
                        print('saving in cache with id %s' % transform)
                        self.block_cache.add(transform, ctr)
                        if awd_parent is not None:
                            awd_parent.add_child(ctr)
                        else:
                            self.awd.add_scene_block(ctr)
  
            else:
                if dag_it.fullPathName(): # Not root
                    dag_it.prune()
                print('skipping invisible %s' % dag_it.fullPathName())

            dag_it.next()


        # TODO: Only do this if exporting rigs
        if include_skeletons:
            dag_it = om.MItDependencyNodes(om.MFn.kSkinClusterFilter)
            while not dag_it.isDone():
                obj = dag_it.thisNode()
                joints = om.MDagPathArray()
 
                skin_fn = omanim.MFnSkinCluster(obj)
                num_joints = skin_fn.influenceObjects(joints)
 
 
                # Loop through joints and look in block cache whether
                # a skeleton for this joint has been exported. If not,
                # we will ignore this binding altogether.
                skel = None
                #print('found skin cluster for %s!' % skel)
                for i in range(num_joints):
                    #print('affected joint: %s' % joints[i].fullPathName())
                    skel = self.block_cache.get(self.get_skeleton_root(joints[i].fullPathName()))
                    if skel is not None:
                        break
 
                # Skeleton was found
                if skel is not None:
                    #print('found skeleton in cache!')
                    #print('num joints: %d' % num_joints)
 
                    # Loop through meshes that are influenced by this
                    # skeleton, and add weight stream to their mesh data
                    num_geoms = skin_fn.numOutputConnections()
                    #print('num geoms: %d' % num_geoms)
                    for i in range(num_geoms):
                        skin_path = om.MDagPath()
                        skin_fn.getPathAtIndex(i, skin_path)
                        vert_it = om.MItMeshVertex(skin_path)
 
                        #print('skin obj: %s' % skin_path.fullPathName())
 
                        # Check whether a mesh data for this geometry has
                        # been added to the block cache. If not, bindings
                        # for this mesh can be ignored.
                        md = self.block_cache.get(self.get_name(skin_path.fullPathName()))
                        if md is not None:
                            #print('found mesh in cache!')
                            weight_data = []
                            index_data = []
 
                            #md.skeleton = skel
 
                            # TODO: Don't hardcode this
                            joints_per_vert = 3
 
                            while not vert_it.isDone():
                                comp = vert_it.currentItem()
                                weights = om.MDoubleArray()
                                weight_objs = []
 
                                #script_util = om.MScriptUtil()
                                for ii in range(num_joints):
                                    skin_fn.getWeights(skin_path, comp, ii, weights)
                                    joint_name = joints[ii].fullPathName()
                                    joint_idx = self.joint_indices[joint_name.split('|')[-1]]
                                    weight_objs.append( (joint_idx, weights[0]) )
 
                                def comp_weight_objs(wo0, wo1):
                                    if wo0[1] > wo1[1]: return -1
                                    else: return 1
 
                                weight_objs.sort(comp_weight_objs)
 
                                # Normalize top weights
                                weight_objs = weight_objs[0:joints_per_vert]
                                sum_obj = reduce(lambda w0,w1: (0, w0[1]+w1[1]), weight_objs)
                                weight_objs = map(lambda w: (w[0], w[1] / sum_obj[1]), weight_objs)
 
                                # Add more empty weight objects if too few
                                if len(weight_objs) != joints_per_vert:
                                    weight_objs.extend([(0,0)] * (joints_per_vert - len(weight_objs)))
 
                                for w_obj in weight_objs:
                                    index_data.append(w_obj[0])
                                    weight_data.append(w_obj[1])
 
                                vert_it.next()
 
                            weight_stream = []
                            index_stream = []
 
                            # This list contains the old-index of each vertex in the AWD vertex stream
                            vert_indices = self.mesh_vert_indices[skin_path.fullPathName()]
                            for idx in vert_indices:
                                start_idx = idx*joints_per_vert
                                end_idx = start_idx + joints_per_vert
                                w_tuple = weight_data[start_idx:end_idx]
                                i_tuple = index_data[start_idx:end_idx]
                                weight_stream.extend(w_tuple)
                                index_stream.extend(i_tuple)
 
                            if len(md) == 1:
                                print('Setting streams!')
                                sub = md[0]
                                sub.add_stream(AWDSubMesh.JOINT_WEIGHTS, weight_stream)
                                sub.add_stream(AWDSubMesh.JOINT_INDICES, index_stream)
                            else:
                                print('skinning not implemented for meshes with <> 1 sub-mesh')
 
                dag_it.next()

        if include_animation:
            #animated_materials = [ 'MAT_BlueEye_L', 'MAT_BlueEye_R' ]
            animated_materials = [ 'MAT_BrownEye_L', 'MAT_BrownEye_R' ]
            #animated_materials = []
 
            for seq in sequences:
                frame_idx = seq[1]
                end_frame = seq[2]
 
                print('exporting sequence "%s" (%d-%d)' % seq)
 
                if len(self.skeleton_paths) > 0:
                    anim = AWDSkeletonAnimation(seq[0])
                    self.awd.add_skeleton_anim(anim)
 
                uvanims = []
                for mat in animated_materials:
                    uvanim = AWDUVAnimation(mat.replace('MAT', 'UVANIM')+'_'+seq[0])
                    uvanims.append(uvanim)
                    self.awd.add_uv_anim(uvanim)
 
                while frame_idx <= end_frame:
                    om.MGlobal.viewFrame(frame_idx)
 
                    self.sample_materials(animated_materials, uvanims)
 
                    for skeleton_path in self.skeleton_paths:
                        def get_all_transforms(joint_path, list):
                            mtx_list = mc.xform(joint_path, q=True, m=True)
                            list.append( self.mtx_list2awd(mtx_list))
 
                            children = mc.listRelatives(joint_path, type='joint')
                            if children is not None:
                                for child in children:
                                    get_all_transforms(child, list)
 
                        skel_pose = AWDSkeletonPose()
 
                        all_transforms = []
                        get_all_transforms(skeleton_path, all_transforms)
                        for tf in all_transforms:
                            skel_pose.add_joint_transform(tf)
 
                        anim.add_frame(skel_pose)
                        self.awd.add_skeleton_pose(skel_pose)
 
                    # Move to next frame
                    frame_idx += 1
 
 
        self.awd.flush(self.file)


    def export_mesh(self, transform, shape, awd_ctr):
        try:
            mtx = mc.xform(transform, q=True, m=True)
        except:
            print('skipping invalid %s' % transform)
 
        tf_name = self.get_name(transform)
        sh_name = self.get_name(shape)
 
        md = self.block_cache.get(sh_name)
        if md is None:
            print('Creating mesh data %s' % sh_name)
            md = AWDMeshData(sh_name)
            md.bind_matrix = AWDMatrix4x4(mtx)
            self.export_mesh_data(md, shape)
            self.awd.add_mesh_data(md)
            self.block_cache.add(sh_name, md)
 
        inst = AWDMeshInst(md, tf_name, self.mtx_list2awd(mtx))
 
        # Look for materials
        sets = mc.listSets(object=transform, t=1, ets=True)
        print('======')
        if sets is not None:
            for set in sets:
                if mc.nodeType(set)=='shadingEngine':
                    mat_his = mc.listHistory(set)
                    for state in mat_his:
                        state_type = mc.nodeType(state)
 
                        if state_type == 'lambert':
                            mat = self.block_cache.get(state)
                            if mat is None:
                                mat = AWDMaterial(AWDMaterial.BITMAP, name=self.get_name(state))
                                self.awd.add_material(mat)
                                self.block_cache.add(state, mat)
                                print('created material')
 
                            inst.materials.append(mat)
                            print('adding material ' + state)
                            
                        elif state_type == 'file':
                            tex = self.block_cache.get(state)
                            if tex is None:
                                tex_abs_path = str(mc.getAttr(state+'.fileTextureName'))
                                tex = AWDTexture(AWDTexture.EXTERNAL, name=self.get_name(state))
                                tex.url = mc.workspace(pp=tex_abs_path)
                                self.awd.add_texture(tex)
                                self.block_cache.add(state, tex)
                                print('created texture')
 
                            if mat is not None:
                                mat.texture = tex
                            print('adding texture '+state)
 
 
        self.block_cache.add(transform, inst)
        if awd_ctr is not None:
            awd_ctr.add_child(inst)
        else:
            self.awd.add_scene_block(inst)
 
        history = mc.listHistory(transform)
        clusters = mc.ls(history, type='skinCluster')
        if len(clusters) > 0:
            #TODO: Deal with multiple clusters?
            sc = clusters[0]
 
            influences = mc.skinCluster(sc, q=True, inf=True)
            if len(influences) > 0:
                skel_path = self.get_skeleton_root(influences[0])
 
                if self.block_cache.get(skel_path) is None:
                    self.export_skeleton(skel_path)
 
 
 
    def sample_materials(self, animated_materials, uvanims):
        idx = 0
        for mat in animated_materials:
            pt = None
            mat_his = mc.listHistory(mat)
            #print('sampling mat', mat)
 
            uvanim = uvanims[idx]
 
            # Find most recent place2DTexture
            for state in mat_his:
                if mc.nodeType(state) == 'place2dTexture':
                    pt = state
                    break
 
            t = mc.getAttr(pt+'.tf')[0]
            uvanim.frames.append( AWDMatrix2x3([ 1, 0, 0, 1, -t[0], t[1] ]) )
 
            idx += 1
 
 
    def export_skeleton(self, root_path):
        skel = AWDSkeleton(name=root_path)
        joints = []
 
        def create_joint(joint_path, world_mtx=None):
            dag_path = self.get_dag_from_path(joint_path)
            tf_fn = om.MFnTransform(dag_path.node())
            tf = tf_fn.transformation()
            joint_wm = tf.asMatrix()
 
            if world_mtx is not None:
                joint_wm = joint_wm * world_mtx
 
            ibm = joint_wm.inverse()
            awd_mtx = self.mtx_maya2awd(ibm)
 
            name = self.get_name(joint_path)
            joint = AWDSkeletonJoint(name=name, inv_bind_mtx=awd_mtx)
 
            self.joint_indices[joint_path] = len(joints)
            print('added joint %s as idx %d' % (joint_path, len(joints)))
            joints.append(name)
 
            children = mc.listRelatives(joint_path, type='joint')
            print('JOINT CHILDREN: %s', str(children))
            if children is not None:
                for child_path in children:
                    joint.add_child_joint( create_joint(child_path, joint_wm) )
 
            return joint
 
        skel.root_joint = create_joint(root_path)
 
        self.awd.add_skeleton(skel)
        self.block_cache.add(root_path, skel)
        self.skeleton_paths.append(root_path)
 
 
 
    def get_skeleton_root(self, joint_path):
        current = joint_path
        parent = mc.listRelatives(current, p=True)
        while parent:
            current = parent
            parent = mc.listRelatives(current, p=True)
 
        if isinstance(current, list):
            current = current[0]
 
        return str(current)
            
 
    def get_dag_from_path(self, path):
        list = om.MSelectionList()
        list.add(path)
        dag_path = om.MDagPath()
        list.getDagPath(0, dag_path, om.MObject())
 
        return dag_path
        
        
 
    def export_mesh_data(self, md, shape_path):
        dag_path = self.get_dag_from_path(shape_path)
        if dag_path.hasFn(om.MFn.kMesh):
            exp_vert_list = []
    
            def get_uvs(vert_it, face_idx):
                us = om.MFloatArray()
                vs = om.MFloatArray()
                uvis = om.MIntArray()
    
                vert_it.getUVs(us, vs, uvis)
                for i in range(len(uvis)):
                    if uvis[i] == face_idx:
                        return (us[i],vs[i])
    
                print('NO UV FOUND!!!!! WHY!!!!!??')
                return (0,0)
    
            def get_vnormal(vert_it, face_idx):
                vec = om.MVector()
                try:
                    vert_it.getNormal(vec, face_idx)
                except:
                    vert_it.getNormal(vec)
    
                return vec
    
            def get_fnormal(poly_it):
                vec = om.MVector()
                poly_it.getNormal(vec)
    
                return vec
                
    
            print('getting mesh data for %s' % dag_path.fullPathName())
            print('type: %s' % dag_path.node().apiTypeStr())
            vert_it = om.MItMeshVertex(dag_path.node())
            poly_it = om.MItMeshPolygon(dag_path.node())
            while not poly_it.isDone():
                tri_inds = om.MIntArray()
                tri_points = om.MPointArray()
    
                poly_index = poly_it.index()
    
                idx_triple = []
                poly_it.getTriangles(tri_points, tri_inds)
                for i in range(tri_inds.length()):
                    vert_index = tri_inds[i]
    
                    pidx_util = om.MScriptUtil()
                    vert_it.setIndex(vert_index, pidx_util.asIntPtr())
    
                    u,v = get_uvs(vert_it, poly_index)
                    #normal = get_vnormal(vert_it, poly_index)
                    normal = get_fnormal(poly_it)
                    pos = vert_it.position()
    
                    exp_vert_list.append(
                        [ vert_index, poly_index, pos[0], pos[1], pos[2], u, v, normal, [normal] ])
                    
                poly_it.next()
    
    
            print('- Raw (expanded) data list created')
    
            # Store this so binding (joint index) data can be
            # put into the right place of the new vertex list
            vert_indices = []
            self.mesh_vert_indices[dag_path.fullPathName()] = vert_indices
    
            vertices = []
            indices = []
            uvs = []
            normals = []
    
    
            def has_vert(haystack, needle, normal_threshold=45):
                idx = 0
                normal_threshold *= (3.141592636/180)
                for v in haystack:
                    correct = True
                    for prop in range(2, 7):
                        if needle[prop] != v[prop]:
                            correct = False
                            break
    
                    # Get angle between normals and compare
                    if correct:
                        angle = needle[7].angle(v[7])
                        if angle > normal_threshold:
                            correct = False
    
                    if correct:
                        return idx
    
                    idx += 1
    
                return -1
                
            merged_vertices = []
    
            print('- Creating condensed list')
    
            for v in exp_vert_list:
                idx = has_vert(merged_vertices, v)
                if idx >= 0:
                    # Since this vertex already existed, add it's
                    # normal to the list of influential ones, and
                    # calculate a new average normal for these
                    old_v = merged_vertices[idx]
                    old_v[8].append(v[7])               # add to normals list (8)
                    accum = om.MVector()
                    for n in old_v[8]:
                        accum += n
                    old_v[7] = accum / len(old_v[8])    # calculate avg normal (7)
    
                    # Already has vertex
                    indices.append(idx)
                else:
                    # Store this for binding data
                    vert_indices.append(v[0])
    
                    indices.append(len(merged_vertices))
                    merged_vertices.append(v)
                
            for v in merged_vertices:
                # Add vertex and index
                vertices.append(v[2])   # X
                vertices.append(v[3])   # Y
                vertices.append(-v[4])  # Z (inverted)
                uvs.append(v[5])        # U
                uvs.append(1-v[6])      # V
                normals.append(v[7][0])    # Normal X
                normals.append(v[7][1])    # Normal Y
                normals.append(-v[7][2])   # Normal Z (inverted)
    
            print('- DONE! Flipping windings')
    
            # Flip windings
            for idx in range(1, len(indices), 3):
                tmp = indices[idx]
                indices[idx] = indices[idx+1]
                indices[idx+1] = tmp
    
            print('- Creating sub-mesh')
            sub = AWDSubMesh()
            sub.add_stream(AWDSubMesh.VERTICES, vertices)
            sub.add_stream(AWDSubMesh.TRIANGLES, indices)
            sub.add_stream(AWDSubMesh.UVS, uvs)
            sub.add_stream(AWDSubMesh.VERTEX_NORMALS, normals)
    
            print('- Adding sub-mesh')
    
            md.add_sub_mesh(sub)
    
            # Store mesh data block to block cache


    def get_name(self, dag_path):
        # TODO: Deal with unicode names. In pyawd?
        return str(dag_path.split('|')[-1])
        
    def mtx_list2awd(self, mtx):
        mtx_list = [v for v in mtx]
        mtx_list[2] *= -1
        mtx_list[6] *= -1
        mtx_list[8] *= -1
        mtx_list[9] *= -1
        mtx_list[11] *= -1
        mtx_list[14] *= -1
        #mtx_list[0] = mtx[0]
        #mtx_list[1] = mtx[2]
        #mtx_list[2] = mtx[1]
        #mtx_list[3] = mtx[3]
        #mtx_list[4] = mtx[8]
        #mtx_list[5] = mtx[10]
        #mtx_list[6] = mtx[9]
        #mtx_list[7] = mtx[11]
        #mtx_list[8] = mtx[4]
        #mtx_list[9] = mtx[6]
        #mtx_list[10] = mtx[5]
        #mtx_list[11] = mtx[7]
        #mtx_list[12] = mtx[12]
        #mtx_list[13] = mtx[14]
        #mtx_list[14] = mtx[13]
        #mtx_list[15] = mtx[15]
 
        return AWDMatrix4x4(mtx_list)
        
    def mtx_maya2awd(self, mtx):
        mtx_list = []
        for i in range(16):
           row_idx = math.floor(i/4)
           col_idx = i%4
           mtx_list.append(mtx(int(row_idx), int(col_idx)))
 
        #mtx_list[1] *= -1
        #mtx_list[2] *= -1
        #mtx_list[3] *= -1
        #mtx_list[4] *= -1
        #mtx_list[8] *= -1
 
        #print(mtx_list[0:4])
        #print(mtx_list[4:8])
        #print(mtx_list[8:12])
        #print(mtx_list[12:])
        return self.mtx_list2awd(mtx_list)
        
