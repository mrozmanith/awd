#ifndef _PYAWD_UTIL_H
#define _PYAWD_UTIL_H

#include <awd/libawd.h>

bool            pyawdutil_has_true_attr(PyObject *, const char *);
void            pyawdutil_get_strattr(PyObject *, const char *, const char **, int *);
awd_float64 *   pyawdutil_pylist_to_float64(PyObject *, awd_float64 *, unsigned int);
awd_float32 *   pyawdutil_pylist_to_float32(PyObject *, awd_float32 *, unsigned int);
awd_uint32 *    pyawdutil_pylist_to_uint32(PyObject *, awd_uint32 *, unsigned int);
awd_uint16 *    pyawdutil_pylist_to_uint16(PyObject *, awd_uint16 *, unsigned int);

#endif
