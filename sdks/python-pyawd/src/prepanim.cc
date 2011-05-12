/**
 * This file contains reparation functions for all blocks belonging in the pyawd.anim
 * module. 
 *
 * These are invoked by AWDWriter to convert a python object to it's C++/libawd
 * equivalence and add it to the AWD document.
*/

#include "AWDWriter.h"
#include "bcache.h"
#include "util.h"


static AWDSkeletonJoint *
__prepare_skeljoint(PyObject *py_joint, AWDSkeletonJoint *lawd_parent)
{
    int i;
    int num_children;
    char *name;
    int name_len;
    awd_float64 *mtx;

    AWDSkeletonJoint *lawd_joint;

    PyObject *name_attr;
    PyObject *bind_mtx_attr;
    PyObject *children_attr;

    name_attr = PyObject_GetAttrString(py_joint, "name");
    name = PyString_AsString(name_attr);
    name_len = PyString_Size(name_attr);

    bind_mtx_attr = PyObject_GetAttrString(py_joint, "inv_bind_mtx");
    if (bind_mtx_attr && bind_mtx_attr != Py_None) {
        PyObject *raw_data_attr;

        raw_data_attr = PyObject_GetAttrString(bind_mtx_attr, "raw_data");
        if (raw_data_attr)
            mtx = pyawdutil_pylist_to_float64(raw_data_attr, NULL, 16);
    }

    lawd_joint = new AWDSkeletonJoint(name, name_len, mtx);

    if (lawd_parent)
        lawd_parent->add_child_joint(lawd_joint);


    children_attr = PyObject_GetAttrString(py_joint, "_AWDSkeletonJoint__children");
    num_children = PyList_Size(children_attr);
    for (i=0; i<num_children; i++) {
        PyObject *py_child;

        py_child = PyList_GetItem(children_attr, i);
        __prepare_skeljoint(py_child, lawd_joint);
    }

    return lawd_joint;
}


void
__prepare_skeleton(PyObject *block, AWD *awd, pyawd_bcache *bcache)
{
    char *name;
    int name_len;
    AWDSkeleton *lawd_skel;
    PyObject *name_attr;
    PyObject *root_attr;

    name_attr = PyObject_GetAttrString(block, "name");
    name = PyString_AsString(name_attr);
    name_len = PyString_Size(name_attr);

    lawd_skel = new AWDSkeleton(name, name_len);

    root_attr = PyObject_GetAttrString(block, "root_joint");
    if (root_attr != Py_None) {
        AWDSkeletonJoint *lawd_joint;

        lawd_joint = __prepare_skeljoint(root_attr, NULL);
        lawd_skel->set_root_joint(lawd_joint);
    }

    awd->add_skeleton(lawd_skel);
    pyawd_bcache_add(bcache, block, lawd_skel);
}

void
__prepare_skelanim(PyObject *block, AWD *awd, pyawd_bcache *bcache)
{
    int i;
    char *name;
    int name_len;
    int num_frames;
    PyObject *frames_attr;
    PyObject *name_attr;
    AWDSkeletonAnimation *lawd_anim;

    name_attr = PyObject_GetAttrString(block, "name");
    name = PyString_AsString(name_attr);
    name_len = PyString_Size(name_attr);

    frames_attr = PyObject_GetAttrString(block, "frames");
    num_frames = PyList_Size(frames_attr);
    
    lawd_anim = new AWDSkeletonAnimation(name, name_len, num_frames);
    
    for (i=0; i<num_frames; i++) {
        AWDSkeletonPose *lawd_pose;
        PyObject *pose;

        pose = PyList_GetItem(frames_attr, i);
        lawd_pose = (AWDSkeletonPose *)pyawd_bcache_get(bcache, pose);
        lawd_anim->set_next_frame_pose(lawd_pose);
    }

    awd->add_skeleton_anim(lawd_anim);
    pyawd_bcache_add(bcache, block, lawd_anim);
}

void
__prepare_skelpose(PyObject *block, AWD *awd, pyawd_bcache *bcache)
{
    int i;
    char *name;
    int name_len;
    int num_jtf;

    PyObject *tfs_attr;
    PyObject *name_attr;
    AWDSkeletonPose *lawd_pose;
    
    name_attr = PyObject_GetAttrString(block, "name");
    name = PyString_AsString(name_attr);
    name_len = PyString_Size(name_attr);

    lawd_pose = new AWDSkeletonPose(name, name_len);
    
    tfs_attr = PyObject_GetAttrString(block, "transforms");
    num_jtf = PyList_Size(tfs_attr);
    for (i=0; i<num_jtf; i++) {
        PyObject *joint_tf;
    
        joint_tf = PyList_GetItem(tfs_attr, i);
        if (joint_tf != Py_None) {
            PyObject *raw_data_attr;
            awd_float64 *mtx;

            raw_data_attr = PyObject_GetAttrString(joint_tf, "raw_data");
            mtx = pyawdutil_pylist_to_float64(raw_data_attr, NULL, 16);
            lawd_pose->set_next_transform(mtx);
        }
        else {
            lawd_pose->set_next_transform(NULL);
        }
    }

    awd->add_skeleton_pose(lawd_pose);
    pyawd_bcache_add(bcache, block, lawd_pose);
}

