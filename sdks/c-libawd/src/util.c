#include <stdlib.h>
#include <string.h>

#include "libawd.h"

awd_float64 *
awdutil_id_mtx4(awd_float64 *mtx)
{
    if (mtx == NULL) {
        mtx = malloc(16 * sizeof(awd_float64));
    }

    mtx[0]  = 1.0; mtx[1]  = 0.0; mtx[2]  = 0.0; mtx[3]  = 0.0;
    mtx[4]  = 0.0; mtx[5]  = 1.0; mtx[6]  = 0.0; mtx[7]  = 0.0;
    mtx[8]  = 0.0; mtx[9]  = 0.0; mtx[10] = 1.0; mtx[11] = 0.0;
    mtx[12] = 0.0; mtx[13] = 0.0; mtx[14] = 0.0; mtx[15] = 1.0;

    return mtx;
}


inline awd_uint32
awdutil_stream_len(AWD_mesh_data_stream *str, awd_bool opt_for_acc)
{
    return (str->num_elements * awdutil_stream_elem_size(str, opt_for_acc));
}

size_t
awdutil_stream_elem_size(AWD_mesh_data_stream *str, awd_bool opt_for_acc)
{
    size_t elem_size;

    switch (str->type) {
        case VERTICES:
            elem_size = sizeof(awd_float32);
            break;
        case TRIANGLES:
            elem_size = sizeof(awd_uint16);
            break;
        case UVS:
            elem_size = sizeof(awd_float32);
            break;
    }

    if (opt_for_acc == AWD_TRUE)
        elem_size *= 2;

    return elem_size;
}

awd_bool
awdutil_check_flag(AWD *awd, awd_uint16 flag)
{
    if ((awd->header->flags & flag) > 0)
        return AWD_TRUE;
    else return AWD_FALSE;
}



int
awdutil_mktmp(char **path)
{
    int len;
    int fd;
    char *tpl;

    len = strlen(TMPFILE_TEMPLATE);
    tpl = malloc(len);
    strncpy(tpl, TMPFILE_TEMPLATE, len);

    fd = mkstemp(tpl);

    if (path)
        *path = tpl;
    else free(tpl);

    return fd;
}


awd_uint16
awdutil_swapui16(awd_uint16 n)
{
    union {
        awd_uint16 i;
        awd_uint8 b[2];
    } in, out;

    in.i = n;


    out.b[0] = in.b[1];
    out.b[1] = in.b[0];

    return out.i;
}

awd_uint32 
awdutil_swapui32(awd_uint32 n)
{
    union {
        awd_uint32 i;
        awd_uint8 b[4];
    } in, out;

    in.i = n;
    out.b[0] = in.b[3];
    out.b[1] = in.b[2];
    out.b[2] = in.b[1];
    out.b[3] = in.b[0];

    return out.i;
}

awd_float32 
awdutil_swapf32(awd_float32 n)
{
    union {
        awd_float32 f;
        awd_uint8 b[4];
    } in, out;

    in.f = n;
    out.b[0] = in.b[3];
    out.b[1] = in.b[2];
    out.b[2] = in.b[1];
    out.b[3] = in.b[0];

    return out.f;
}

awd_float64 
awdutil_swapf64(awd_float64 n)
{
    union {
        awd_float64 f;
        awd_uint8 b[8];
    } in, out;

    in.f = n;
    out.b[0] = in.b[7];
    out.b[1] = in.b[6];
    out.b[2] = in.b[5];
    out.b[3] = in.b[4];
    out.b[4] = in.b[3];
    out.b[5] = in.b[2];
    out.b[6] = in.b[1];
    out.b[7] = in.b[0];

    return out.f;
}





