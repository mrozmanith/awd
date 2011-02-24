#include <Python.h>
#include "structmember.h"

#include "AWDMeshInst.h"
#include "AWDMeshData.h"
#include "AWDMatrix4.h"

#include <awd/libawd.h>



/**
 * free()
*/
void
pyawd_AWDMeshInst_dealloc(pyawd_AWDMeshInst *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDMeshInst();
*/
PyObject *
pyawd_AWDMeshInst_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWDMeshInst *self;

    self = (pyawd_AWDMeshInst *)type->tp_alloc(type, 0);
    if (self != NULL) {
    }

    return (PyObject *)self;
}


/**
 * AWDMeshInst.__init__();
*/
int
pyawd_AWDMeshInst_init(pyawd_AWDMeshInst *self, PyObject *args, PyObject *kwds)
{
    PyObject *name;
    PyObject *tf;
    PyObject *md;

    char *kwlist[] = {"data", "name", "transform"};

    tf = NULL;
    md = NULL;
    name = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O!O!O!", kwlist,
        &pyawd_AWDMeshDataType, &md, &PyString_Type, &name, &pyawd_AWDMatrix4Type, &tf))
        return NULL;

    if (name == NULL)
        name = PyString_FromString("");
    if (tf == NULL)
        tf = (PyObject*)pyawd_AWDMatrix4_new(&pyawd_AWDMatrix4Type, NULL, NULL);
    if (md == NULL)
        md = Py_None;

    Py_INCREF(name);
    self->name = name;

    Py_INCREF(tf);
    self->transform = tf;
    
    Py_INCREF(md);
    self->mesh_data = md;

    return 0;
}



/**
 * Method dictionary
*/
PyMethodDef pyawd_AWDMeshInst_methods[] = {
    { NULL }
};



/**
 * Type object
*/
PyTypeObject pyawd_AWDMeshInstType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDMeshInst",                    /* tp_name */
    sizeof(pyawd_AWDMeshInst),              /* tp_basicsize */
    0,                                      /* tp_itemsize */
    (destructor)pyawd_AWDMeshInst_dealloc,  /* tp_dealloc */
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
    "AWD mesh representation.",             /* tp_doc */
    0,                                      /* tp_traverse */
    0,                                      /* tp_clear */
    0,                                      /* tp_richcompare */
    0,                                      /* tp_weaklistoffset */
    0,                                      /* tp_iter */
    0,                                      /* tp_iternext */
    pyawd_AWDMeshInst_methods,              /* tp_methods */
    0,                                      /* tp_members */
    0,                                      /* tp_getset */
    0,                                      /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    (initproc)pyawd_AWDMeshInst_init,       /* tp_init */
    0,                                      /* tp_alloc */
    pyawd_AWDMeshInst_new,                  /* tp_new */
};


