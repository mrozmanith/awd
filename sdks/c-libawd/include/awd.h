#ifndef _AWD_H
#define _AWD_H

#include <stdio.h>

#include "awd_types.h"
#include "attr.h"
#include "mesh.h"


#define AWD_MAJOR_VERSION 1
#define AWD_MINOR_VERSION 0

#define AWD_TRUE 1
#define AWD_FALSE 0
#define AWD_NULL 0

#define AWD_STREAMING               0x1
#define AWD_OPTIMIZE_FOR_ACCURACY   0x2


typedef struct {
    awd_uint8   major_version;
    awd_uint8   minor_version;
    awd_uint16  flags;
    awd_uint32  body_length;

    AWD_compression compression;
} AWD_header;



struct AWD_block_struct {
    awd_uint16  id;
    awd_uint8   type;
    void        *data;

    AWD_attr_list *attributes;

    struct AWD_block_struct *next;
};
typedef struct AWD_block_struct AWD_block;


typedef struct {
    AWD_block *first_block;
} AWD_block_list;


typedef struct {
   AWD_header *     header;

   awd_uint16       _last_block_id;
   awd_bool         _header_written;

   AWD_block_list * meshes;
   AWD_block_list * mesh_instances;
} AWD;





AWD *               awd_create(AWD_compression, awd_uint16);

// Structure management functions
awd_bool            awd_add_mesh_data(AWD *, AWD_mesh_data *, AWD_attr_list *);
awd_bool            awd_add_mesh_inst(AWD *, AWD_mesh_data *, awd_float64 *, AWD_attr_list *);

// Misc
awd_bool            awd_flush(AWD*, int);

#endif

