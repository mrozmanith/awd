#include <Python.h>
#include "structmember.h"

#include "AWDSkeletonAnimation.h"

#include <awd/libawd.h>



/**
 * free()
*/
void
pyawd_AWDSkeletonAnimation_dealloc(pyawd_AWDSkeletonAnimation *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDSkeletonAnimation();
*/
PyObject *
pyawd_AWDSkeletonAnimation_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWDSkeletonAnimation *self;

    self = (pyawd_AWDSkeletonAnimation *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->name = NULL;
        self->frames = PyList_New(0);
        Py_INCREF(self->frames);
    }

    return (PyObject *)self;
}


/**
 * AWDSkeletonAnimation.__init__();
*/
int
pyawd_AWDSkeletonAnimation_init(pyawd_AWDSkeletonAnimation *self, PyObject *args, PyObject *kwds)
{
    char *kwlist[] = { "name" };

    //TODO: add list of transforms as param?
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O!", kwlist, &PyString_Type, &self->name))
        return NULL;

    if (!self->name)
        self->name = PyString_FromString("");

    Py_INCREF(self->name);

    return 0;
}



static int
pyawd_AWDSkeletonAnimation_mp_length(pyawd_AWDSkeletonAnimation *self)
{
    return PyList_Size(self->frames);
}


static PyObject *
pyawd_AWDSkeletonAnimation_mp_subscript(pyawd_AWDSkeletonAnimation *self, PyObject *key)
{
    if (PyInt_Check(key)) {
        long idx;
        idx = PyInt_AsLong(key);
        return PyList_GetItem(self->frames, idx);
    }
    else {
        PyErr_SetString(PyExc_RuntimeError, "Index must be integer.");
        return NULL;
    }
}


static int
pyawd_AWDSkeletonAnimation_mp_ass_subscript(pyawd_AWDSkeletonAnimation *self, PyObject *key, PyObject *value)
{
    if (!PyInt_Check(key)) {
        PyErr_SetString(PyExc_RuntimeError, "Index has to be an integer.");
        return -1;
    }
    if (value == Py_None || PyObject_IsInstance(value, (PyObject*)&pyawd_AWDSkeletonPoseType)) {
        long idx;
        idx = PyInt_AsLong(key);
        PyList_SetItem(self->frames, idx, value);
    }
    else {
        PyErr_SetString(PyExc_RuntimeError, "Value has to be SkeletonPose or None.");
        return -1;
    }
}



/**
 * AWDSkeletonAnimation.add_pose()
*/
PyObject *
pyawd_AWDSkeletonAnimation_add_pose(pyawd_AWDSkeletonAnimation *self, PyObject *arg)
{
    if (PyObject_IsInstance(arg, (PyObject*)&pyawd_AWDSkeletonPoseType)) {
        PyList_Append(self->frames, arg);
        Py_RETURN_NONE;
    }
    else {
        PyErr_SetString(PyExc_RuntimeError, "Value must be AWDSkeletonPose");
        return NULL;
    }
}


/**
 * Method dictionary
*/
PyMethodDef pyawd_AWDSkeletonAnimation_methods[] = {
    { "add_pose", (PyCFunction)pyawd_AWDSkeletonAnimation_add_pose, METH_O,
        "Add a pose into the sequence of pose frames that builds up this animation." },
    { NULL }
};



PyMappingMethods pyawd_AWDSkeletonAnimationMapping = {
    (lenfunc)pyawd_AWDSkeletonAnimation_mp_length,
    (binaryfunc)pyawd_AWDSkeletonAnimation_mp_subscript,
    (objobjargproc)pyawd_AWDSkeletonAnimation_mp_ass_subscript,
};



/**
 * Type object
*/
PyTypeObject pyawd_AWDSkeletonAnimationType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDSkeletonAnimation",               // tp_name
    sizeof(pyawd_AWDSkeletonAnimation),         // tp_basicsize
    0,                                          // tp_itemsize
    (destructor)
        pyawd_AWDSkeletonAnimation_dealloc,     // tp_dealloc
    0,                                          // tp_print
    0,                                          // tp_getattr
    0,                                          // tp_setattr
    0,                                          // tp_reserved
    0,                                          // tp_repr
    0,                                          // tp_as_number
    0,                                          // tp_as_sequence
    &pyawd_AWDSkeletonAnimationMapping,          // tp_as_mapping
    0,                                          // tp_hash 
    0,                                          // tp_call
    0,                                          // tp_str
    0,                                          // tp_getattro
    0,                                          // tp_setattro
    0,                                          // tp_as_buffer
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,                    // tp_flags
    "AWD mesh representation.",                 // tp_doc
    0,                                          // tp_traverse
    0,                                          // tp_clear
    0,                                          // tp_richcompare
    0,                                          // tp_weaklistoffset
    0,                                          // tp_iter
    0,                                          // tp_iternext
    pyawd_AWDSkeletonAnimation_methods,         // tp_methods
    0,                                          // tp_members
    0,                                          // tp_getset
    0,                                          // tp_base
    0,                                          // tp_dict
    0,                                          // tp_descr_get
    0,                                          // tp_descr_set
    0,                                          // tp_dictoffset
    (initproc)pyawd_AWDSkeletonAnimation_init,  // tp_init
    0,                                          // tp_alloc
    pyawd_AWDSkeletonAnimation_new,             // tp_new
};


