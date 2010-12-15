#ifndef _PYAWD_AWDMESHINST_H
#define _PYAWD_AWDMESHINST_H

#include <Python.h>

#include <awd/libawd.h>

extern PyTypeObject pyawd_AWDMeshInstType;



typedef struct {
    PyObject_HEAD

    awd_float64 *ob_transform;
} pyawd_AWDMeshInst;


void pyawd_AWDMeshInst_dealloc(pyawd_AWDMeshInst *);
PyObject *pyawd_AWDMeshInst_new(PyTypeObject *, PyObject *, PyObject *);

int pyawd_AWDMeshInst_init(pyawd_AWDMeshInst *, PyObject *, PyObject *);




#endif

