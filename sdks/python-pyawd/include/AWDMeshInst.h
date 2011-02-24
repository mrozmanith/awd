#ifndef _PYAWD_AWDMESHINST_H
#define _PYAWD_AWDMESHINST_H

#include <Python.h>

#include <awd/libawd.h>

#include "AWDMeshData.h"
#include "AWDMatrix4.h"


extern PyTypeObject pyawd_AWDMeshInstType;


typedef struct {
    PyObject_HEAD


    PyObject *lawd_obj;

    PyObject *name;
    PyObject *transform;
    PyObject *mesh_data;
} pyawd_AWDMeshInst;

#endif
