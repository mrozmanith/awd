#include <stdlib.h>
#include <string.h>
#include <cstdio>

#include "libawd.h"

// Get mkstemp replacement
#ifdef WIN32
#include "awdw32.h"
#else
#include <unistd.h>
#endif

awd_float64 *
awdutil_id_mtx4(awd_float64 *mtx)
{
    if (mtx == NULL) {
        mtx = (awd_float64*)malloc(16 * sizeof(awd_float64));
    }

    mtx[0]  = 1.0; mtx[1]  = 0.0; mtx[2]  = 0.0; mtx[3]  = 0.0;
    mtx[4]  = 0.0; mtx[5]  = 1.0; mtx[6]  = 0.0; mtx[7]  = 0.0;
    mtx[8]  = 0.0; mtx[9]  = 0.0; mtx[10] = 1.0; mtx[11] = 0.0;
    mtx[12] = 0.0; mtx[13] = 0.0; mtx[14] = 0.0; mtx[15] = 1.0;

    return mtx;
}


void
awdutil_write_mtx4(int fd, awd_float64 *mtx)
{
    int i;
    awd_float64 n;
    for (i=0; i<16; i++) {
        n = F64(mtx[i]);
        write(fd, &n, sizeof(awd_float64));
    }
}


void
awdutil_write_varstr(int fd, const char *str, awd_uint16 str_len)
{
    awd_uint16 len_be;

    if (str != NULL) {
        len_be = UI16(str_len);
        write(fd, &len_be, sizeof(awd_uint16));
        write(fd, str, str_len);
    }
    else {
        len_be = 0;
        write(fd, &len_be, sizeof(awd_uint16));
    }
}


int
awdutil_mktmp(char **path)
{
    int fd;
    int tpl_len;
    char *tmp_path;

#ifdef WIN32
    int ret;
	int path_len;

    tmp_path = (char*)malloc(TMPPATH_MAXLEN);

    tpl_len = strlen(TMPFILE_TEMPLATE);
	path_len = GetTempPath(TMPPATH_MAXLEN, tmp_path);
    if (path_len==0 || (path_len+tpl_len) > TMPPATH_MAXLEN)
        return -1;

    // Concatenate path and template and null-terminate.
    strncpy(tmp_path+path_len, TMPFILE_TEMPLATE, tpl_len);
    memset(tmp_path+path_len+tpl_len, 0, 1);

    ret = _mktemp_s(tmp_path, path_len+tpl_len+1);
	if (ret==0)
		fd = _open(tmp_path, _O_CREAT|_O_TEMPORARY|_O_RDWR|_O_BINARY, _S_IWRITE);
	else fd = -1;

#else
    tpl_len = strlen(TMPFILE_TEMPLATE);
    tmp_path = (char *)malloc(tpl_len);
    strncpy(tmp_path, TMPFILE_TEMPLATE, tpl_len);

    fd = mkstemp(tmp_path);
#endif

    if (path)
	    *path = tmp_path;
    else free(tmp_path);

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





