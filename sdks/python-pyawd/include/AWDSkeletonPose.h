#ifndef _PYAWD_AWDSKELETONPOSE_H
#define _PYAWD_AWDSKELETONPOSE_H

#include <Python.h>

#include <awd/libawd.h>

#include "AWDMatrix4.h"


extern PyTypeObject pyawd_AWDSkeletonPoseType;


typedef struct {
    PyObject_HEAD

    PyObject *name;

    PyObject *transforms;

    AWDSkeletonPose *ob_pose;
} pyawd_AWDSkeletonPose;



#endif

