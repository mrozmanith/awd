#ifndef _MESH_H
#define _MESH_H


#include "awd.h"
#include "material.h"


/**
 * Data stream type
*/
typedef enum {
    VERTICES=1,
    TRIANGLES,
    UVS
} AWD_stream_type;


/** 
 * Data stream pointer
*/
typedef union {
    void *v;
    awd_uint16 *ui16;
    awd_uint32 *ui32;
    awd_float32 *f32;
    awd_float64 *f64;
} AWD_mesh_data_stream_ptr;





/**
 * Sub-mesh data stream
*/
typedef struct _AWD_mesh_data_stream {
    AWD_stream_type                 type;
    awd_uint32                      num_elements;
    AWD_mesh_data_stream_ptr        data;

    struct _AWD_mesh_data_stream *  next;
} AWD_mesh_data_stream;


/**
 * Sub-mesh
*/
typedef struct _AWD_sub_mesh {
    AWD_mesh_data_stream *          first_stream;

    AWD_material *                  material;
    struct _AWD_sub_mesh *           next;
} AWD_sub_mesh;


/**
 * Mesh geometry data
*/
typedef struct _AWD_mesh_data {
    AWD_sub_mesh *                  first_sub_mesh;
} AWD_mesh_data;


/**
 * Mesh instance.
*/
typedef struct _AWD_mesh_inst {
    AWD_mesh_data *data;
    void *parent;
    awd_float64 *transform_mtx;
} AWD_mesh_inst;



AWD_mesh_data *     awd_create_mesh_data(void);
AWD_sub_mesh *      awd_create_sub_mesh(void);

awd_bool            awd_mesh_add_sub(AWD_mesh_data *, AWD_sub_mesh *);
awd_bool            awd_mesh_add_sub_vtu(AWD_mesh_data *, awd_uint32, awd_uint32, void *, void *, void *, AWD_material *);

awd_bool            awd_sub_mesh_add_stream(AWD_sub_mesh *, AWD_stream_type, unsigned int, void *);


#endif
