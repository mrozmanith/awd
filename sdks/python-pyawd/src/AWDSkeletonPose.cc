#include <Python.h>
#include "structmember.h"

#include "AWDSkeletonPose.h"
#include "AWDMatrix4.h"

#include <awd/libawd.h>



/**
 * free()
*/
void
pyawd_AWDSkeletonPose_dealloc(pyawd_AWDSkeletonPose *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDSkeletonPose();
*/
PyObject *
pyawd_AWDSkeletonPose_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWDSkeletonPose *self;

    self = (pyawd_AWDSkeletonPose *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->name = NULL;
        self->transforms = PyList_New(0);
        Py_INCREF(self->transforms);
    }

    return (PyObject *)self;
}


/**
 * AWDSkeletonPose.__init__();
*/
int
pyawd_AWDSkeletonPose_init(pyawd_AWDSkeletonPose *self, PyObject *args, PyObject *kwds)
{
    char *kwlist[] = { "name" };

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O!", kwlist, &PyString_Type, &self->name))
        return NULL;

    if (!self->name)
        self->name = PyString_FromString("");

    Py_INCREF(self->name);

    return 0;
}



static int
pyawd_AWDSkeletonPose_mp_length(pyawd_AWDSkeletonPose *self)
{
    return PyList_Size(self->transforms);
}


PyObject *
pyawd_AWDSkeletonPose_mp_subscript(pyawd_AWDSkeletonPose *self, PyObject *key)
{
    if (PyInt_Check(key)) {
        long idx;

        idx = PyInt_AsLong(key);
        return PyList_GetItem(self->transforms, idx);
    }
    else {
        PyErr_SetString(PyExc_RuntimeError, "Index must be integer.");
        return NULL;
    }
}


int
pyawd_AWDSkeletonPose_mp_ass_subscript(pyawd_AWDSkeletonPose *self, PyObject *key, PyObject *value)
{
    if (!PyInt_Check(key)) {
        PyErr_SetString(PyExc_RuntimeError, "Index has to be an integer.");
        return -1;
    }
    if (value == Py_None || PyObject_IsInstance(value, (PyObject*)&pyawd_AWDMatrix4Type)) {
        long idx;
        idx = PyInt_AsLong(key);
        if (idx >= 0 && idx <= PyList_Size(self->transforms)) {
            Py_INCREF(value);
            return PyList_SetItem(self->transforms, idx, value);
        }
        else {
            PyErr_Format(PyExc_RuntimeError, "Index %d out of bounds", idx);
            return -1;
        }
    }
    else {
        PyErr_SetString(PyExc_RuntimeError, "Value has to be Matrix4 or None.");
        return -1;
    }
}


/**
 * AWDSkeletonPose.add_joint_transform()
*/
PyObject *
pyawd_AWDSkeletonPose_add_joint_transform(pyawd_AWDSkeletonPose *self, PyObject *arg)
{
    if (arg == Py_None || PyObject_IsInstance(arg, (PyObject*)&pyawd_AWDMatrix4Type)) {
        PyList_Append(self->transforms, arg);
        Py_RETURN_NONE;
    }
    else {
        PyErr_SetString(PyExc_RuntimeError, "Argument must be AWDMatrix4 or None");
        return NULL;
    }
}





/**
 * Method dictionary
*/
PyMethodDef pyawd_AWDSkeletonPose_methods[] = {
    { "add_joint_transform", (PyCFunction)pyawd_AWDSkeletonPose_add_joint_transform, METH_O,
        "Add a transformation matrix to the list of joint transforms." },
    { NULL }
};


PyMappingMethods pyawd_AWDSkeletonPoseMapping = {
    (lenfunc)pyawd_AWDSkeletonPose_mp_length,
    (binaryfunc)pyawd_AWDSkeletonPose_mp_subscript,
    (objobjargproc)pyawd_AWDSkeletonPose_mp_ass_subscript,
};



/**
 * Type object
*/
PyTypeObject pyawd_AWDSkeletonPoseType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDSkeletonPose",                // tp_name
    sizeof(pyawd_AWDSkeletonPose),          // tp_basicsize
    0,                                      // tp_itemsize
    (destructor)
        pyawd_AWDSkeletonPose_dealloc,      // tp_dealloc
    0,                                      // tp_print
    0,                                      // tp_getattr
    0,                                      // tp_setattr
    0,                                      // tp_reserved
    0,                                      // tp_repr
    0,                                      // tp_as_number
    0,                                      // tp_as_sequence
    &pyawd_AWDSkeletonPoseMapping,          // tp_as_mapping
    0,                                      // tp_hash 
    0,                                      // tp_call
    0,                                      // tp_str
    0,                                      // tp_getattro
    0,                                      // tp_setattro
    0,                                      // tp_as_buffer
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,                // tp_flags
    "AWD mesh representation.",             // tp_doc
    0,                                      // tp_traverse
    0,                                      // tp_clear
    0,                                      // tp_richcompare
    0,                                      // tp_weaklistoffset
    0,                                      // tp_iter
    0,                                      // tp_iternext
    pyawd_AWDSkeletonPose_methods,          // tp_methods
    0,                                      // tp_members
    0,                                      // tp_getset
    0,                                      // tp_base
    0,                                      // tp_dict
    0,                                      // tp_descr_get
    0,                                      // tp_descr_set
    0,                                      // tp_dictoffset
    (initproc)pyawd_AWDSkeletonPose_init,   // tp_init
    0,                                      // tp_alloc
    pyawd_AWDSkeletonPose_new,              // tp_new
};


