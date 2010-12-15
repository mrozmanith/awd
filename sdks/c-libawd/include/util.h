#ifndef _AWDUTIL_H
#define _AWDUTIL_H

#include "awd.h"
#include "awd_types.h"

#define TMPFILE_TEMPLATE "/tmp/awd.XXXXXX"

// Utility functions
awd_float64     *awdutil_id_mtx4(awd_float64 *);

awd_bool        awdutil_check_flag(AWD *, awd_uint16);
awd_uint32      awdutil_stream_len(AWD_mesh_data_stream *, awd_bool);

size_t          awdutil_stream_elem_size(AWD_mesh_data_stream *, awd_bool);

int             awdutil_mktmp();

awd_uint16      awdutil_swapui16(awd_uint16);
awd_uint32      awdutil_swapui32(awd_uint32);
awd_float32     awdutil_swapf32(awd_float32);
awd_float64     awdutil_swapf64(awd_float64);

#endif
