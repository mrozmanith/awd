#ifndef _MESH_H
#define _MESH_H


#include "awd.h"
#include "attr.h"
#include "material.h"


/**
 * Data stream type
*/
typedef enum {
    VERTICES=1,
    TRIANGLES,
    UVS,
    VERTEX_NORMALS,
    VERTEX_TANGENTS,
    FACE_NORMALS,
} AWD_mesh_str_type;


/** 
 * Data stream pointer
*/
typedef union {
    awd_uint32 *ui32;
    awd_float64 *f64;
} AWD_data_str_ptr;




/**
 * Sub-mesh data stream
*/
typedef struct _AWD_mesh_data_str{
    AWD_mesh_str_type           type;
    awd_uint32                  num_elements;
    AWD_data_str_ptr            data;

    struct _AWD_mesh_data_str*  next;
} AWD_mesh_data_str;


/**
 * Sub-mesh
*/
typedef struct _AWD_sub_mesh {
    AWD_mesh_data_str *             first_stream;

    AWD_material *                  material;
    struct _AWD_sub_mesh *          next;
} AWD_sub_mesh;


/**
 * Mesh geometry data
*/
typedef struct _AWD_mesh_data {
    AWD_sub_mesh *      first_sub_mesh;
    AWD_attr_list *     attributes;
} AWD_mesh_data;


/**
 * Mesh instance.
*/
typedef struct _AWD_mesh_inst {
    AWD_mesh_data *     data;
    void *              parent;
    awd_float64 *       transform_mtx;
    AWD_attr_list *     attributes;
} AWD_mesh_inst;



AWD_mesh_inst *     awd_create_mesh_inst(void);
AWD_mesh_data *     awd_create_mesh_data(void);
AWD_sub_mesh *      awd_create_sub_mesh(void);

awd_bool            awd_mesh_add_sub(AWD_mesh_data *, AWD_sub_mesh *);
awd_bool            awd_mesh_add_sub_vtu(AWD_mesh_data *, awd_uint32, awd_uint32, awd_float64 *, awd_uint32 *, awd_float64 *, AWD_material *);

awd_bool            awd_sub_mesh_add_stream(AWD_sub_mesh *, AWD_mesh_str_type, awd_uint32, AWD_data_str_ptr);


#endif
