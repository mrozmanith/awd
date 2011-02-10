#ifndef _PYAWD_AWDMESHDATA_H
#define _PYAWD_AWDMESHDATA_H

#include <Python.h>

#include <awd/libawd.h>

#include "AWDSubMesh.h"


extern PyTypeObject pyawd_AWDMeshDataType;


typedef struct {
    PyObject_HEAD

    AWD_mesh_data *ob_data;

    pyawd_AWDSubMesh *first_sub_mesh;
} pyawd_AWDMeshData;


void pyawd_AWDMeshData_dealloc(pyawd_AWDMeshData *);
PyObject *pyawd_AWDMeshData_new(PyTypeObject *, PyObject *, PyObject *);

int pyawd_AWDMeshData_init(pyawd_AWDMeshData *, PyObject *, PyObject *);

PyObject *pyawd_AWDMeshData_add_sub(pyawd_AWDMeshData *, PyObject *, PyObject *);


#endif

