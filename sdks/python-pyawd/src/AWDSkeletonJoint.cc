#include <Python.h>
#include "structmember.h"

#include "AWDSkeletonJoint.h"
#include "AWDMatrix4.h"
#include "util.h"

#include <awd/libawd.h>



/**
 * free()
*/
void
pyawd_AWDSkeletonJoint_dealloc(pyawd_AWDSkeletonJoint *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDSkeletonJoint();
*/
PyObject *
pyawd_AWDSkeletonJoint_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWDSkeletonJoint *self;

    self = (pyawd_AWDSkeletonJoint *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->children = PyList_New(0);
    }

    return (PyObject *)self;
}


/**
 * AWDSkeletonJoint.__init__();
*/
int
pyawd_AWDSkeletonJoint_init(pyawd_AWDSkeletonJoint *self, PyObject *args, PyObject *kwds)
{
    char *kwlist[] = { "name", "transform", NULL };

    self->name = NULL;
    self->bind_mtx = NULL;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O!O!", kwlist, 
        &PyString_Type, &self->name, &pyawd_AWDMatrix4Type, &self->bind_mtx))
        return -1;

    // TODO: Fail if unicode
    if (!self->name)
        self->name = PyString_FromString("");
    if (!self->bind_mtx)
        self->bind_mtx = Py_None;

    self->parent = Py_None;

    Py_INCREF(self->parent);
    Py_INCREF(self->name);
    Py_INCREF(self->bind_mtx);

    return 0;
}


PyObject *
pyawd_AWDSkeletonJoint_add_child_joint(pyawd_AWDSkeletonJoint *self, PyObject *arg)
{
    pyawd_AWDSkeletonJoint *joint;

    if (arg == Py_None || PyObject_IsInstance(arg, (PyObject *)&pyawd_AWDSkeletonJointType)) {
        PyList_Append(self->children, arg);
        ((pyawd_AWDSkeletonJoint*)arg)->parent = (PyObject *)self;
        Py_RETURN_NONE;
    }
    else {
        PyErr_SetString(PyExc_RuntimeError, "Value must be SkeletonJoint or None");
        // TODO: Set exception
        return NULL;
    }
}



/**
 * Method dictionary
*/
PyMethodDef pyawd_AWDSkeletonJoint_methods[] = {
    { "add_child_joint", (PyCFunction)pyawd_AWDSkeletonJoint_add_child_joint, METH_O,
        "Add a joint as a child to this joint." },
    { NULL }
};



/**
 * Type object
*/
PyTypeObject pyawd_AWDSkeletonJointType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDSkeletonJoint",                   // tp_name
    sizeof(pyawd_AWDSkeletonJoint),             // tp_basicsize
    0,                                          // tp_itemsize
    (destructor)pyawd_AWDSkeletonJoint_dealloc, // tp_dealloc
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
    "AWD skeleton joint.",                      // tp_doc
    0,                                          // tp_traverse
    0,                                          // tp_clear
    0,                                          // tp_richcompare
    0,                                          // tp_weaklistoffset
    0,                                          // tp_iter
    0,                                          // tp_iternext
    pyawd_AWDSkeletonJoint_methods,             // tp_methods
    0,                                          // tp_members
    0,                                          // tp_getset
    0,                                          // tp_base
    0,                                          // tp_dict
    0,                                          // tp_descr_get
    0,                                          // tp_descr_set
    0,                                          // tp_dictoffset
    (initproc)pyawd_AWDSkeletonJoint_init,      // tp_init
    0,                                          // tp_alloc
    pyawd_AWDSkeletonJoint_new,                 // tp_new
};



void
pyawd_AWDSkeletonJoint__prep(pyawd_AWDSkeletonJoint *self)
{
    int i;
    int num_children;
    char *name;
    int name_len;
    awd_float64 *mtx;

    name = PyString_AsString(self->name);
    name_len = PyString_Size(self->name);

    if (PyObject_IsInstance(self->bind_mtx, (PyObject*)&pyawd_AWDMatrix4Type)) {
        mtx = ((pyawd_AWDMatrix4*)self->bind_mtx)->raw_data;
    }
    else {
        mtx = awdutil_id_mtx4(NULL);
    }

    self->ob_joint = new AWDSkeletonJoint(name, name_len, mtx);
    if (self->parent != Py_None) {
        pyawd_AWDSkeletonJoint *parent;
        parent = (pyawd_AWDSkeletonJoint *)self->parent;
        parent->ob_joint->add_child_joint(self->ob_joint);
    }

    num_children = PyList_Size(self->children);
    for (i=0; i<num_children; i++) {
        pyawd_AWDSkeletonJoint *child;

        child = (pyawd_AWDSkeletonJoint *)PyList_GetItem(self->children, i);
        pyawd_AWDSkeletonJoint__prep(child);
    }
}

