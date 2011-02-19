#ifndef _PYAWD_AWDSKELETONJOINT_H
#define _PYAWD_AWDSKELETONJOINT_H

#include <Python.h>

#include <awd/libawd.h>

extern PyTypeObject pyawd_AWDSkeletonJointType;



typedef struct _pyawd_AWDSkeletonJoint {
    PyObject_HEAD

    AWDSkeletonJoint *ob_joint;

    const char * name;
    PyListObject * bind_mtx;

    struct _pyawd_AWDSkeletonJoint *parent;
    struct _pyawd_AWDSkeletonJoint *first_child;
    struct _pyawd_AWDSkeletonJoint *last_child;
    struct _pyawd_AWDSkeletonJoint *next;
} pyawd_AWDSkeletonJoint;



#endif

