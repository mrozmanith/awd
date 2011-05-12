#ifndef _PYAWD_AWDUVANIMATION_H
#define _PYAWD_AWDUVANIMATION_H

#include <Python.h>

#include <awd/libawd.h>


extern PyTypeObject pyawd_AWDUVAnimationType;


typedef struct {
    PyObject_HEAD

    PyObject *type;
    PyObject *name;
    PyObject *frames;

    AWDUVAnimation *lawd_obj;
} pyawd_AWDUVAnimation;


void pyawd_AWDUVAnimation__prep(pyawd_AWDUVAnimation *);

#endif
