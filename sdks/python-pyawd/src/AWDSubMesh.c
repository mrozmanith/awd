#include <Python.h>
#include "structmember.h"

#include "AWDMeshInst.h"

#include <awd/libawd.h>



/**
 * free()
*/
void
pyawd_AWDSubMesh_dealloc(pyawd_AWDMeshInst *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDMeshInst();
*/
PyObject *
pyawd_AWDSubMesh_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
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
pyawd_AWDSubMesh_init(pyawd_AWDMeshInst *self, PyObject *args, PyObject *kwds)
{
    return 0;
}



/**
 * Method dictionary
*/
PyMethodDef pyawd_AWDSubMesh_methods[] = {
    { NULL }
};



/**
 * Type object
*/
PyTypeObject pyawd_AWDSubMeshType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDSubMesh",                      /* tp_name */
    sizeof(pyawd_AWDSubMesh),              /* tp_basicsize */
    0,                                      /* tp_itemsize */
    (destructor)pyawd_AWDSubMesh_dealloc,  /* tp_dealloc */
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


