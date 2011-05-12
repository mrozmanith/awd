#ifndef _PYAWD_AWDMATERIAL_H
#define _PYAWD_AWDMATERIAL_H

#include <Python.h>

#include <awd/libawd.h>


extern PyTypeObject pyawd_AWDMaterialType;


typedef struct {
    PyObject_HEAD

    PyObject *type;
    PyObject *name;
    PyObject *texture;

    AWDSimpleMaterial *lawd_obj;
} pyawd_AWDMaterial;


void pyawd_AWDMaterial__prep(pyawd_AWDMaterial *);

#endif
