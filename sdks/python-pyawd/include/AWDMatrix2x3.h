#ifndef _PYAWD_AWDMATRIX2x3_H
#define _PYAWD_AWDMATRIX2x3_H

#include <Python.h>

#include <awd/libawd.h>

extern PyTypeObject pyawd_AWDMatrix2x3Type;

typedef struct {
    PyObject_HEAD

    awd_float64 *raw_data;
} pyawd_AWDMatrix2x3;


#endif

