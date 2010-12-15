#include <stdlib.h>

#include "libawd.h"


AWD_mesh_data *
awd_create_mesh_data()
{
    AWD_mesh_data *mesh;

    mesh = malloc(sizeof(AWD_mesh_data));
    mesh->first_sub_mesh = NULL;

    return mesh;
}


AWD_sub_mesh *
awd_create_sub_mesh()
{
    AWD_sub_mesh *sub;

    sub = malloc(sizeof(AWD_sub_mesh));
    sub->material = NULL;
    sub->next = NULL;

    return sub;
}


awd_bool
awd_mesh_add_sub(AWD_mesh_data *mesh, AWD_sub_mesh *sub)
{
    if (!mesh->first_sub_mesh) {
        mesh->first_sub_mesh = sub;
    }
    else {
        AWD_sub_mesh *next;
        while (1) {
            if (!next->next) {
                next->next = sub;
                break;
            }
            else next = next->next;
        }
    }

    return AWD_TRUE;
}



awd_bool
awd_sub_mesh_add_stream(AWD_sub_mesh *sub, AWD_stream_type type, unsigned int num_elements, void *data)
{
    AWD_mesh_data_stream_ptr ptr;
    AWD_mesh_data_stream *stream;

    ptr.v = data;

    stream = malloc(sizeof(AWD_mesh_data_stream));
    stream->type = type;
    stream->num_elements = num_elements;
    stream->data = ptr;
    stream->next = NULL;

    if (!sub->first_stream) {
        sub->first_stream = stream;
    }
    else {
        AWD_mesh_data_stream *next = sub->first_stream;
        while (1) {
            if (!next->next) {
                next->next = stream;
                break;
            }

            next = next->next;
        }
    }

    return AWD_TRUE;
}


/**
 * Utility function to add a simple sub-mesh with vertex,
 * triangle index and UV data streams.
 *
 * The stream data is defined as void pointers but is assumed
 * to be of the right type for the AWD file to which it's 
 * being added (i.e. float32/uint16 by default, float64/uint32
 * for wide files.)
*/
awd_bool 
awd_mesh_add_sub_vtu(AWD_mesh_data *mesh, awd_uint32 num_verts, awd_uint32 num_tris, 
                                void *verts, void *tris, void *uvs, AWD_material *mat)
{
    AWD_sub_mesh *sub;

    sub = awd_create_sub_mesh();

    if (verts != NULL)
        awd_sub_mesh_add_stream(sub, VERTICES, num_verts * 3, verts);
    if (tris != NULL) 
        awd_sub_mesh_add_stream(sub, TRIANGLES, num_tris * 3, tris);
    if (uvs != NULL)
        awd_sub_mesh_add_stream(sub, UVS, num_verts * 2, uvs);

    return awd_mesh_add_sub(mesh, sub);
}


