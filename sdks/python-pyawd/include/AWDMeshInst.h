#ifndef _PYAWD_AWDMESHINST_H
#define _PYAWD_AWDMESHINST_H

#include <Python.h>

#include <awd/libawd.h>

#include "AWDMeshData.h"
#include "AWDMatrix4.h"


extern PyTypeObject pyawd_AWDMeshInstType;


typedef struct {
    PyObject_HEAD

    PyObject *name;
    PyObject *transform;
    PyObject *mesh_data;
    
    PyObject *materials;

    AWDMeshInst *lawd_obj;
} pyawd_AWDMeshInst;

void pyawd_AWDMeshInst__prep(pyawd_AWDMeshInst *);

#endif
