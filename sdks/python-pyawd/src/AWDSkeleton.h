#ifndef _PYAWD_AWDSKELETON_H
#define _PYAWD_AWDSKELETON_H

#include <Python.h>

#include <awd/libawd.h>

#include "AWDSkeletonJoint.h"


extern PyTypeObject pyawd_AWDSkeletonType;



typedef struct {
    PyObject_HEAD

    pyawd_AWDSkeletonJoint *root_joint;

    AWDSkeleton *ob_skeleton;
} pyawd_AWDSkeleton;


void pyawd_AWDSkeleton_dealloc(pyawd_AWDSkeleton *);
PyObject *pyawd_AWDSkeleton_new(PyTypeObject *, PyObject *, PyObject *);

int pyawd_AWDSkeleton_init(pyawd_AWDSkeleton *, PyObject *, PyObject *);




#endif

