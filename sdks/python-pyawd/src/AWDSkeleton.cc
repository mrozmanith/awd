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
    return (PyObject *)self;
}


/**
 * AWDSkeleton.__init__();
*/
int
pyawd_AWDSkeleton_init(pyawd_AWDSkeleton *self, PyObject *args, PyObject *kwds)
{
    const char *name;

    char *kwlist[] = {"name"};

    name = NULL;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|s", kwlist, &name))
        return -1;

    if (name == NULL) name = "";
    self->ob_skeleton = new AWDSkeleton(name);

    return 0;
}


static int
pyawd_AWDSkeleton_set_root_joint(pyawd_AWDSkeleton *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        // TODO: Set error
        return NULL;
    }

    if (PyObject_IsInstance(value, (PyObject *)&pyawd_AWDSkeletonJointType)) {
        pyawd_AWDSkeletonJoint *joint;

        joint = (pyawd_AWDSkeletonJoint *)value;
        self->root_joint = joint;
        self->ob_skeleton->set_root_joint(joint->ob_joint);

        return 0;
    }
    else {
        // TODO: Set error
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
    "pyawd.AWDSkeleton",                    /* tp_name */
    sizeof(pyawd_AWDSkeleton),              /* tp_basicsize */
    0,                                      /* tp_itemsize */
    (destructor)pyawd_AWDSkeleton_dealloc,  /* tp_dealloc */
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
    "AWD keleton block.",                   /* tp_doc */
    0,                                      /* tp_traverse */
    0,                                      /* tp_clear */
    0,                                      /* tp_richcompare */
    0,                                      /* tp_weaklistoffset */
    0,                                      /* tp_iter */
    0,                                      /* tp_iternext */
    pyawd_AWDSkeleton_methods,              /* tp_methods */
    0,                                      /* tp_members */
    pyawd_AWDSkeleton_getset,               /* tp_getset */
    0,                                      /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    (initproc)pyawd_AWDSkeleton_init,       /* tp_init */
    0,                                      /* tp_alloc */
    pyawd_AWDSkeleton_new,                  /* tp_new */
};


