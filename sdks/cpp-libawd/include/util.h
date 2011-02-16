#ifndef _AWDUTIL_H
#define _AWDUTIL_H

#include "awd.h"
#include "awd_types.h"

#define TMPFILE_TEMPLATE "/tmp/awd.XXXXXX"

// Utility functions
int             awdutil_mktmp(char **path);

awd_uint16      awdutil_swapui16(awd_uint16);
awd_uint32      awdutil_swapui32(awd_uint32);
awd_float32     awdutil_swapf32(awd_float32);
awd_float64     awdutil_swapf64(awd_float64);

#endif
