#ifndef _PYAWD_AWDSKELETON_H
#define _PYAWD_AWDSKELETON_H

#include <Python.h>

#include <awd/libawd.h>

#include "AWDSkeletonJoint.h"


extern PyTypeObject pyawd_AWDSkeletonType;



typedef struct {
    PyObject_HEAD

    PyObject *name;
    PyObject *root_joint;

    AWDSkeleton *ob_skeleton;
} pyawd_AWDSkeleton;


void pyawd_AWDSkeleton__prep(pyawd_AWDSkeleton *);


#endif

