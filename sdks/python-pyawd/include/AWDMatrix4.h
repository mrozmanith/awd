#ifndef _PYAWD_AWDMATRIX4_H
#define _PYAWD_AWDMATRIX4_H

#include <Python.h>

#include <awd/libawd.h>

extern PyTypeObject pyawd_AWDMatrix4Type;

typedef struct {
    PyObject_HEAD

    awd_float64 *raw_data;
} pyawd_AWDMatrix4;


PyObject *pyawd_AWDMatrix4_new(PyTypeObject *, PyObject *, PyObject *);

#endif

