#include <Python.h>
#include "structmember.h"

#include "AWDSkeleton.h"

#include <awd/libawd.h>



/**
 * free()
*/
void
pyawd_AWDSkeleton_dealloc(pyawd_AWDSkeleton *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDSkeleton();
*/
PyObject *
pyawd_AWDSkeleton_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWDSkeleton *self;

    self = (pyawd_AWDSkeleton *)type->tp_alloc(type, 0);
    if (self) {
        self->root_joint = Py_None;
    }
    return (PyObject *)self;
}


/**
 * AWDSkeleton.__init__();
*/
int
pyawd_AWDSkeleton_init(pyawd_AWDSkeleton *self, PyObject *args, PyObject *kwds)
{
    int name_len;
    const char *name;

    char *kwlist[] = { "name", NULL};

    self->name = NULL;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O!", kwlist, &PyString_Type, &self->name))
        return -1;

    if (!self->name)
        self->name = PyString_FromString("");

    Py_INCREF(self->name);

    return 0;
}


static int
pyawd_AWDSkeleton_set_root_joint(pyawd_AWDSkeleton *self, PyObject *value, void *closure)
{
    if (value == Py_None || PyObject_IsInstance(value, (PyObject *)&pyawd_AWDSkeletonJointType)) {

        if (self->root_joint)
            Py_DECREF(self->root_joint);

        self->root_joint = value;
        Py_INCREF(self->root_joint);

        return 0;
    }
    else {
        PyErr_SetString(PyExc_RuntimeError, "Value must be SkeletonJoint or None");
        return -1;
    }
}


static PyObject *
pyawd_AWDSkeleton_get_root_joint(pyawd_AWDSkeleton *self, void *closure)
{
    return (PyObject *)self->root_joint;
}


/**
 * Method dictionary
*/
PyMethodDef pyawd_AWDSkeleton_methods[] = {
    { NULL }
};

/**
 * Get/set dictionary
*/
PyGetSetDef pyawd_AWDSkeleton_getset[] = {
    { "root_joint", 
        (getter)pyawd_AWDSkeleton_get_root_joint,
        (setter)pyawd_AWDSkeleton_set_root_joint,
        "Set the root joint of this skeleton.",
        NULL },

    { NULL }
};



/**
 * Type object
*/
PyTypeObject pyawd_AWDSkeletonType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDSkeleton",                        // tp_name
    sizeof(pyawd_AWDSkeleton),                  // tp_basicsize
    0,                                          // tp_itemsize
    (destructor)pyawd_AWDSkeleton_dealloc,      // tp_dealloc
    0,                                          // tp_print
    0,                                          // tp_getattr
    0,                                          // tp_setattr
    0,                                          // tp_reserved
    0,                                          // tp_repr
    0,                                          // tp_as_number
    0,                                          // tp_as_sequence
    0,                                          // tp_as_mapping
    0,                                          // tp_hash 
    0,                                          // tp_call
    0,                                          // tp_str
    0,                                          // tp_getattro
    0,                                          // tp_setattro
    0,                                          // tp_as_buffer
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,                    // tp_flags
    "AWD keleton block.",                       // tp_doc
    0,                                          // tp_traverse
    0,                                          // tp_clear
    0,                                          // tp_richcompare
    0,                                          // tp_weaklistoffset
    0,                                          // tp_iter
    0,                                          // tp_iternext
    pyawd_AWDSkeleton_methods,                  // tp_methods
    0,                                          // tp_members
    pyawd_AWDSkeleton_getset,                   // tp_getset
    0,                                          // tp_base
    0,                                          // tp_dict
    0,                                          // tp_descr_get
    0,                                          // tp_descr_set
    0,                                          // tp_dictoffset
    (initproc)pyawd_AWDSkeleton_init,           // tp_init
    0,                                          // tp_alloc
    pyawd_AWDSkeleton_new,                      // tp_new
};



void
pyawd_AWDSkeleton__prep(pyawd_AWDSkeleton *self)
{
    char *name;
    int name_len;

    name = PyString_AsString(self->name);
    name_len = PyString_Size(self->name);

    self->ob_skeleton = new AWDSkeleton(name, name_len);

    if (self->root_joint != Py_None) {
        pyawd_AWDSkeletonJoint *root;
        root = (pyawd_AWDSkeletonJoint *)self->root_joint;
        pyawd_AWDSkeletonJoint__prep(root);
        self->ob_skeleton->set_root_joint(root->ob_joint);
    }
}

