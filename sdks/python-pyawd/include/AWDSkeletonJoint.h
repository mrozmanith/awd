#ifndef _PYAWD_AWDSKELETONJOINT_H
#define _PYAWD_AWDSKELETONJOINT_H

#include <Python.h>

#include <awd/libawd.h>

extern PyTypeObject pyawd_AWDSkeletonJointType;



typedef struct _pyawd_AWDSkeletonJoint {
    PyObject_HEAD

    AWDSkeletonJoint *ob_joint;

    PyObject *name;
    PyObject *bind_mtx;

    PyObject *parent;
    PyObject *children;
} pyawd_AWDSkeletonJoint;


void pyawd_AWDSkeletonJoint__prep(pyawd_AWDSkeletonJoint *);

#endif

