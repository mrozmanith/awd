#ifndef _PYAWD_AWDTEXTURE_H
#define _PYAWD_AWDTEXTURE_H

#include <Python.h>

#include <awd/libawd.h>


extern PyTypeObject pyawd_AWDTextureType;


typedef struct {
    PyObject_HEAD

    PyObject *type;
    PyObject *name;
    PyObject *url;

    AWDTexture *lawd_obj;
} pyawd_AWDTexture;


void pyawd_AWDTexture__prep(pyawd_AWDTexture *);

#endif
