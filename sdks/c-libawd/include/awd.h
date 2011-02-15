#ifndef _AWD_H
#define _AWD_H

#include <stdio.h>

#include "awd_types.h"
#include "attr.h"
#include "mesh.h"


#define AWD_MAJOR_VERSION 0
#define AWD_MINOR_VERSION 1

#define AWD_TRUE 1
#define AWD_FALSE 0
#define AWD_NULL 0

#define AWD_STREAMING               0x1
#define AWD_OPTIMIZE_FOR_ACCURACY   0x2


typedef struct _AWD_header {
    awd_uint8               major_version;
    awd_uint8               minor_version;
    awd_uint16              flags;
    awd_uint32              body_length;

    AWD_compression         compression;
} AWD_header;


typedef struct _AWD_ns {
    awd_uint8               id;
    char *                  uri;
} AWD_namespace;


typedef struct _AWD_block { 
    AWD_namespace *         ns;
    awd_uint16              id;
    awd_uint8               type;
    void *                  data;

    AWD_attr_list *         attributes;

    struct _AWD_block *     next;
} AWD_block;


typedef struct {
    AWD_block *             first_block;
} AWD_block_list;


typedef struct {
    AWD_header *            header;
    
    awd_uint16              _last_block_id;
    awd_uint8               _last_ns_id;
    awd_bool                _header_written;
    
    AWD_block_list *        meshes;
    AWD_block_list *        mesh_instances;
    
    AWD_namespace **        namespaces;
} AWD;





AWD *               awd_create(AWD_compression, awd_uint16);

// Structure management functions
//awd_bool            awd_add_mesh_data(AWD *, AWD_mesh_data *, AWD_attr_list *);
awd_bool            awd_add_mesh_inst(AWD *, AWD_mesh_data *, awd_float64 *);

// Misc
awd_bool            awd_flush(AWD*, int);

#endif

