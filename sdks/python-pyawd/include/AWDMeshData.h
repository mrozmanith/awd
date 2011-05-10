#ifndef _PYAWD_AWDMESHDATA_H
#define _PYAWD_AWDMESHDATA_H

#include <Python.h>

#include <awd/libawd.h>

#include "AWDSkeleton.h"
#include "AWDAttrBlock.h"
#include "AWDSubMesh.h"


extern PyTypeObject pyawd_AWDMeshDataType;


typedef struct _pyawd_AWDMeshData {
    pyawd_AWDAttrBlock base;

    AWDMeshData *ob_data;

    PyObject *skeleton;
    PyObject *bind_mtx;
    pyawd_AWDSubMesh *first_sub_mesh;
    pyawd_AWDSubMesh *last_sub_mesh;
    int num_sub_meshes;

    struct _pyawd_AWDMeshData *next;
} pyawd_AWDMeshData;


/*
void pyawd_AWDMeshData_dealloc(pyawd_AWDMeshData *);
PyObject *pyawd_AWDMeshData_new(PyTypeObject *, PyObject *, PyObject *);

int pyawd_AWDMeshData_init(pyawd_AWDMeshData *, PyObject *, PyObject *);

PyObject *pyawd_AWDMeshData_add_sub(pyawd_AWDMeshData *, PyObject *, PyObject *);
*/


void pyawd_AWDMeshData__prep(pyawd_AWDMeshData *);

#endif

