#ifndef _PYAWD_AWDSKELETONANIMATION_H
#define _PYAWD_AWDSKELETONANIMATION_H

#include <Python.h>

#include <awd/libawd.h>

#include "AWDSkeletonPose.h"


extern PyTypeObject pyawd_AWDSkeletonAnimationType;


typedef struct {
    PyObject_HEAD

    PyObject *name;
    PyObject *frames;

    AWDSkeletonAnimation *ob_anim;
} pyawd_AWDSkeletonAnimation;


void pyawd_AWDSkeletonAnimation_dealloc(pyawd_AWDSkeletonAnimation *);
PyObject *pyawd_AWDSkeletonAnimation_new(PyTypeObject *, PyObject *, PyObject *);

int pyawd_AWDSkeletonAnimation_init(pyawd_AWDSkeletonAnimation *, PyObject *, PyObject *);




#endif

