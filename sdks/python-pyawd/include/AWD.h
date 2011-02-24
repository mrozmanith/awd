#ifndef _PYAWD_AWD_H
#define _PYAWD_AWD_H

#include <Python.h>

#include <awd/libawd.h>

#include "AWDMeshData.h"

#define PYAWD_MAJOR_VERSION 0
#define PYAWD_MINOR_VERSION 1


extern PyTypeObject pyawd_AWDType;


typedef struct {
    PyObject_HEAD

    PyObject    *mesh_data_blocks;
    PyObject    *mesh_inst_blocks;
    PyObject    *skeleton_blocks;
    PyObject    *skelanim_blocks;
    PyObject    *skelpose_blocks;

    AWD         *ob_awd;
    int         ob_has_written_header;
} pyawd_AWD;


void pyawd_AWD_dealloc(pyawd_AWD *);
PyObject *pyawd_AWD_new(PyTypeObject *, PyObject *, PyObject *);

int pyawd_AWD_init(pyawd_AWD *, PyObject *, PyObject *);


PyObject *pyawd_AWD_add_mesh_data(pyawd_AWD *, PyObject *);




#endif
