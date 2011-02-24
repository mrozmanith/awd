#include <Python.h>
#include "structmember.h"

#include <awd/libawd.h>

#include "util.h"
#include "AWD.h"
#include "AWDMeshData.h"
#include "AWDSkeleton.h"
#include "AWDMeshInst.h"
#include "AWDSkeletonPose.h"
#include "AWDSkeletonAnimation.h"



/**
 * free()
*/
void
pyawd_AWD_dealloc(pyawd_AWD *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWD();
*/
PyObject *
pyawd_AWD_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWD *self;

    self = (pyawd_AWD *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->mesh_data_blocks = PyList_New(0);
        self->mesh_inst_blocks = PyList_New(0);
        self->skeleton_blocks = PyList_New(0);
        self->skelanim_blocks = PyList_New(0);
        self->skelpose_blocks = PyList_New(0);

        Py_INCREF(self->mesh_data_blocks);
        Py_INCREF(self->mesh_inst_blocks);
        Py_INCREF(self->skeleton_blocks);
        Py_INCREF(self->skelanim_blocks);
        Py_INCREF(self->skelpose_blocks);
    }

    return (PyObject *)self;
}


/**
 * AWD.__init__();
*/
int
pyawd_AWD_init(pyawd_AWD *self, PyObject *args, PyObject *kwds)
{
    awd_uint16 flags;
    awd_uint8 compression;
    PyObject *streaming;
    PyObject *wide;


    char *kwlist[] = {"compression", "streaming", "wide", NULL};

	flags = 0;
	compression = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|bbb", kwlist,
                                    &compression, &streaming, &wide))
        return -1;

    if (streaming == Py_True)
        flags |= AWD_STREAMING;
    if (wide == Py_True)
        flags |= AWD_OPTIMIZE_FOR_ACCURACY;

    self->ob_awd = new AWD((AWD_compression)compression, flags);

    return 0;
}


/**
 * AWD.add_mesh_data()
*/
PyObject *
pyawd_AWD_add_mesh_data(pyawd_AWD *self, PyObject *arg)
{
    if (PyObject_IsInstance(arg, (PyObject*)&pyawd_AWDMeshDataType)) {
        PyList_Append(self->mesh_data_blocks, arg);
    }
    else {
        PyErr_SetString(PyExc_TypeError, "Argument must be AWDMeshData");
        return NULL;
    }

    Py_RETURN_NONE;
}


/**
 * AWD.add_mesh_inst()
*/
PyObject *
pyawd_AWD_add_mesh_inst(pyawd_AWD *self, PyObject *arg)
{
    if (PyObject_IsInstance(arg, (PyObject*)&pyawd_AWDMeshInstType)) {
        PyList_Append(self->mesh_inst_blocks, arg);
    }
    else {
        PyErr_SetString(PyExc_TypeError, "Argument must be AWDMeshInst");
        return NULL;
    }

    Py_RETURN_NONE;
}



/**
 * AWD.add_skeleton()
*/
PyObject *
pyawd_AWD_add_skeleton(pyawd_AWD *self, PyObject *arg)
{
    if (PyObject_IsInstance(arg, (PyObject*)&pyawd_AWDSkeletonType)) {
        PyList_Append(self->skeleton_blocks, arg);
    }
    else {
        PyErr_SetString(PyExc_TypeError, "Argument must be AWDSkeleton");
        return NULL;
    }
    
    Py_RETURN_NONE;
}


/**
 * AWD.add_skeleton_anim()
*/
PyObject *
pyawd_AWD_add_skeleton_anim(pyawd_AWD *self, PyObject *arg)
{
    if (PyObject_IsInstance(arg, (PyObject*)&pyawd_AWDSkeletonAnimationType)) {
        PyList_Append(self->skelanim_blocks, arg);
    }
    else {
        PyErr_SetString(PyExc_TypeError, "Argument must be AWDSkeletonAnim");
        return NULL;
    }
    
    Py_RETURN_NONE;
}


/**
 * AWD.add_skeleton_pose()
*/
PyObject *
pyawd_AWD_add_skeleton_pose(pyawd_AWD *self, PyObject *arg)
{
    if (PyObject_IsInstance(arg, (PyObject*)&pyawd_AWDSkeletonPoseType)) {
        PyList_Append(self->skelpose_blocks, arg);
    }
    else {
        PyErr_SetString(PyExc_TypeError, "Argument must be AWDSkeletonPose");
        return NULL;
    }
    
    Py_RETURN_NONE;
}



/**
 * AWD.flush()
*/
PyObject *
pyawd_AWD_flush(pyawd_AWD *self, PyObject *args)
{
    PyObject *fobj;
    int fd;

#if PYTHON_VERSION == 3
    extern PyTypeObject PyIOBase_Type;
    if (!PyArg_ParseTuple(args, "O!", &PyIOBase_Type, &fobj))
        return NULL;
#else
    extern PyTypeObject PyFile_Type;
    if (!PyArg_ParseTuple(args, "O!", &PyFile_Type, &fobj))
        return NULL;
#endif

    fd = PyObject_AsFileDescriptor(fobj);

    if (fd >= 0) {
        int i;
        int len;

        // TODO: Mark objects as written, and if encountered
        // in another place, add them only if not marked as written

        len = PyList_Size(self->skelpose_blocks);
        for (i=0; i<len; i++) {
            int ii;
            char *name;
            int name_len;
            int num_jtf;
            pyawd_AWDSkeletonPose *pose;

            pose = (pyawd_AWDSkeletonPose *)PyList_GetItem(self->skelpose_blocks, i);
            name = PyString_AsString(pose->name);
            name_len = PyString_Size(pose->name);
            pose->ob_pose = new AWDSkeletonPose(name, name_len);
            self->ob_awd->add_skeleton_pose(pose->ob_pose);

            // TODO: Move this into pose or anywhere but here
            num_jtf = PyList_Size(pose->transforms);
            for (ii=0; ii<num_jtf; ii++) {
                PyObject *joint_tf;

                joint_tf = PyList_GetItem(pose->transforms, ii);
                if (joint_tf != Py_None) {
                    pose->ob_pose->set_next_transform(
                        ((pyawd_AWDMatrix4*)joint_tf)->raw_data);
                }
                else {
                    pose->ob_pose->set_next_transform(NULL);
                }
            }
        }

        len = PyList_Size(self->skelanim_blocks);
        for (i=0; i<len; i++) {
            int ii;
            char *name;
            int name_len;
            int num_frames;
            pyawd_AWDSkeletonAnimation *anim;

            anim = (pyawd_AWDSkeletonAnimation*)PyList_GetItem(self->skelanim_blocks, i);
            name = PyString_AsString(anim->name);
            name_len = PyString_Size(anim->name);
            num_frames = PyList_Size(anim->frames);

            anim->ob_anim = new AWDSkeletonAnimation(name, name_len, num_frames);
            self->ob_awd->add_skeleton_anim(anim->ob_anim);

            for (ii=0; ii<num_frames; ii++) {
                PyObject *pose;
                pose = PyList_GetItem(anim->frames, ii);
                anim->ob_anim->set_next_frame_pose(
                    ((pyawd_AWDSkeletonPose*)pose)->ob_pose);
            }
        }

        len = PyList_Size(self->skeleton_blocks);
        for (i=0; i<len; i++) {
            pyawd_AWDSkeleton *skel;
            skel = (pyawd_AWDSkeleton *)PyList_GetItem(self->skeleton_blocks, i);
            // TODO: Wait with building skeleton until now
            self->ob_awd->add_skeleton(skel->ob_skeleton);
        }

        // Mesh data blocks
        len = PyList_Size(self->mesh_data_blocks);
        for (i=0; i<len; i++) {
            pyawd_AWDMeshData *md;
            md = (pyawd_AWDMeshData*)PyList_GetItem(self->mesh_data_blocks, i);
            self->ob_awd->add_mesh_data(md->ob_data);
        }

        // Mesh instances
        len = PyList_Size(self->mesh_inst_blocks);
        for (i=0; i<len; i++) {
            char *name;
            int name_len;
            awd_float64 *mtx;
            AWDMeshData *awd_md;
            AWDMeshInst *awd_inst;
            pyawd_AWDMeshInst *inst;

            inst = (pyawd_AWDMeshInst*)PyList_GetItem(self->mesh_inst_blocks, i);

            mtx = NULL;
            if (inst->transform)
                mtx = ((pyawd_AWDMatrix4*)inst->transform)->raw_data;

            awd_md = NULL;
            if (inst->mesh_data)
                awd_md = ((pyawd_AWDMeshData*)inst->mesh_data)->ob_data;
            
            name = PyString_AsString(inst->name);
            name_len = PyString_Size(inst->name);
            awd_inst = new AWDMeshInst(name, name_len, awd_md, mtx);
            self->ob_awd->add_mesh_inst(awd_inst);
        }

        // Write buffer
        self->ob_awd->flush(fd);
        Py_RETURN_NONE;
    }
    else {
        return NULL;
    }
}





/**
 * Method dictionary
*/
PyMethodDef pyawd_AWD_methods[] = {
    { "add_mesh_data", (PyCFunction)pyawd_AWD_add_mesh_data, METH_O,
        "Add a mesh data block to the AWD document." },

    { "add_mesh_inst", (PyCFunction)pyawd_AWD_add_mesh_inst, METH_O,
        "Add a mesh instance block to the AWD document." },

    { "add_skeleton", (PyCFunction)pyawd_AWD_add_skeleton, METH_O,
        "Add a skeleton block to the AWD document." },

    { "add_skeleton_pose", (PyCFunction)pyawd_AWD_add_skeleton_pose, METH_O,
        "Add a skeleton pose block to the AWD document." },

    { "add_skeleton_anim", (PyCFunction)pyawd_AWD_add_skeleton_anim, METH_O,
        "Add a skeleton animation block to the AWD document." },

    { "flush", (PyCFunction)pyawd_AWD_flush, METH_VARARGS,
        "Flush everything in the AWD object to an output stream." },

    { NULL }
};



/**
 * Member dictionary
*/
PyMemberDef pyawd_AWD_members[] = {
    { "mesh_data_blocks", T_OBJECT_EX, offsetof(pyawd_AWD, mesh_data_blocks), READONLY,
        "MeshData blocks that have been added to the AWD document." },
    { "mesh_inst_blocks", T_OBJECT_EX, offsetof(pyawd_AWD, mesh_inst_blocks), READONLY,
        "MeshInstance blocks that have been added to the AWD document." },
    { "skeleton_blocks", T_OBJECT_EX, offsetof(pyawd_AWD, skeleton_blocks), READONLY,
        "Skeleton blocks that have been added to the AWD document." },
    { "skeleton_anim_blocks", T_OBJECT_EX, offsetof(pyawd_AWD, skelanim_blocks), READONLY,
        "SkeletonAnimation blocks that have been added to the AWD document." },
    { "skeleton_pose_blocks", T_OBJECT_EX, offsetof(pyawd_AWD, skelpose_blocks), READONLY,
        "SkeletonPose blocks that have been added to the AWD document." },
    { NULL }
};



/**
 * Type object 
*/
PyTypeObject pyawd_AWDType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWD",                        // tp_name
    sizeof(pyawd_AWD),                  // tp_basicsize
    0,                                  // tp_itemsize
    (destructor)pyawd_AWD_dealloc,      // tp_dealloc
    0,                                  // tp_print
    0,                                  // tp_getattr
    0,                                  // tp_setattr
    0,                                  // tp_reserved
    0,                                  // tp_repr
    0,                                  // tp_as_number
    0,                                  // tp_as_sequence
    0,                                  // tp_as_mapping
    0,                                  // tp_hash 
    0,                                  // tp_call
    0,                                  // tp_str
    0,                                  // tp_getattro
    0,                                  // tp_setattro
    0,                                  // tp_as_buffer
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,            // tp_flags
    "AWD document object.",             // tp_doc
    0,                                  // tp_traverse
    0,                                  // tp_clear
    0,                                  // tp_richcompare
    0,                                  // tp_weaklistoffset
    0,                                  // tp_iter
    0,                                  // tp_iternext
    pyawd_AWD_methods,                  // tp_methods
    pyawd_AWD_members,                  // tp_members
    0,                                  // tp_getset
    0,                                  // tp_base
    0,                                  // tp_dict
    0,                                  // tp_descr_get
    0,                                  // tp_descr_set
    0,                                  // tp_dictoffset
    (initproc)pyawd_AWD_init,           // tp_init
    0,                                  // tp_alloc
    pyawd_AWD_new,                      // tp_new
};



