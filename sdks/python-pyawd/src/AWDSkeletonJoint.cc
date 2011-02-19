#include <Python.h>
#include "structmember.h"

#include "util.h"
#include "AWDSkeletonJoint.h"

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
        self->name = NULL;
        self->bind_mtx = NULL;
        self->parent = NULL;
        self->first_child = NULL;
        self->last_child = NULL;
        self->next = NULL;
    }

    return (PyObject *)self;
}


/**
 * AWDSkeletonJoint.__init__();
*/
int
pyawd_AWDSkeletonJoint_init(pyawd_AWDSkeletonJoint *self, PyObject *args, PyObject *kwds)
{
    const char *name;
    PyListObject *mtx_list;
    awd_float64 *mtx;

    char *kwlist[] = { "name", "transform" };

    name = NULL;
    mtx = NULL;
    mtx_list = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|sO!", kwlist, &name,
                          &PyList_Type, &mtx_list))
        return NULL;

    if (name==NULL) name = "";
    if (mtx_list != NULL) {
        mtx = pyawdutil_pylist_to_float64((PyObject *)mtx_list, NULL, 16);
    }

    self->name = name;
    self->bind_mtx = mtx_list;
    self->ob_joint = new AWDSkeletonJoint(name, mtx);

    return 0;
}


PyObject *
pyawd_AWDSkeletonJoint_add_child_joint(pyawd_AWDSkeletonJoint *self, PyObject *arg)
{
    pyawd_AWDSkeletonJoint *joint;

    if (!PyObject_IsInstance(arg, (PyObject *)&pyawd_AWDSkeletonJointType)) {
        // TODO: Set exception
        return NULL;
    }

    joint = (pyawd_AWDSkeletonJoint *)arg;
    if (joint->parent != NULL) {
        // TODO: Remove from current parent
    }

    // Add to internal list
    if (self->first_child == NULL) {
        self->first_child = joint;
    }
    else {
        self->last_child->next = joint;
    }

    joint->parent = self;
    self->last_child = joint;

    // Add to libawd joint
    self->ob_joint->add_child_joint(joint->ob_joint);
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
    "pyawd.AWDSkeletonJoint",                    /* tp_name */
    sizeof(pyawd_AWDSkeletonJoint),              /* tp_basicsize */
    0,                                      /* tp_itemsize */
    (destructor)pyawd_AWDSkeletonJoint_dealloc,  /* tp_dealloc */
    0,                                      /* tp_print */
    0,                                      /* tp_getattr */
    0,                                      /* tp_setattr */
    0,                                      /* tp_reserved */
    0,                                      /* tp_repr */
    0,                                      /* tp_as_number */
    0,                                      /* tp_as_sequence */
    0,                                      /* tp_as_mapping */
    0,                                      /* tp_hash  */
    0,                                      /* tp_call */
    0,                                      /* tp_str */
    0,                                      /* tp_getattro */
    0,                                      /* tp_setattro */
    0,                                      /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,                /* tp_flags */
    "AWD skeleton joint.",                  /* tp_doc */
    0,                                      /* tp_traverse */
    0,                                      /* tp_clear */
    0,                                      /* tp_richcompare */
    0,                                      /* tp_weaklistoffset */
    0,                                      /* tp_iter */
    0,                                      /* tp_iternext */
    pyawd_AWDSkeletonJoint_methods,         /* tp_methods */
    0,                                      /* tp_members */
    0,                                      /* tp_getset */
    0,                                      /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    (initproc)pyawd_AWDSkeletonJoint_init,  /* tp_init */
    0,                                      /* tp_alloc */
    pyawd_AWDSkeletonJoint_new,             /* tp_new */
};


