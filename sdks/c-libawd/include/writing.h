#ifndef _WRITING_H
#define _WRITING_H

#include "awd.h"
#include "awd_types.h"

void            _awd_write_header(AWD *, int, awd_uint32);
void            _awd_write_block_header(AWD_block *, awd_uint32, int) ;
void            _awd_write_blocks(AWD *, AWD_block_list *, int, void (*)(AWD *, AWD_block *, int));
void            _awd_write_mesh_data(AWD *, AWD_block *, int);
void            _awd_write_mesh_inst(AWD *, AWD_block *, int);

void            _awd_write_mesh_data_stream(AWD_mesh_data_str *, awd_bool, int);

#endif
