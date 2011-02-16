#include <Python.h>
#include <listobject.h>
#include <structmember.h>

#include "util.h"
#include "AWDAttrBlock.h"

#include <awd/libawd.h>



/**
 * free()
*/
void
pyawd_AWDAttrBlock_dealloc(pyawd_AWDAttrBlock *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDAttrBlock();
*/
PyObject *
pyawd_AWDAttrBlock_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWDAttrBlock *self;

    self = (pyawd_AWDAttrBlock *)type->tp_alloc(type, 0);

    return (PyObject *)self;
}


/**
 * AWDAttrBlock.__init__();
*/
int
pyawd_AWDAttrBlock_init(pyawd_AWDAttrBlock *self, PyObject *args, PyObject *kwds)
{
    self->attributes = PyDict_New();
    return 0;
}


void
_apply_block_attributes(AWDBlock *block, pyawd_AWDAttrBlock *)
{
    
}



PyMemberDef pyawd_AWDAttrBlock_members[] = {
    { "attributes", T_OBJECT_EX, offsetof(pyawd_AWDAttrBlock, attributes), 0,
        "Dictionary containing user-attributes for this block" },
    { NULL },
};



/**
 * Type object
*/
PyTypeObject pyawd_AWDAttrBlockType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDAttrBlock",                    /* tp_name */
    sizeof(pyawd_AWDAttrBlock),              /* tp_basicsize */
    0,                                      /* tp_itemsize */
    (destructor)pyawd_AWDAttrBlock_dealloc,  /* tp_dealloc */
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
    "Base class for blocks with attributes.",/* tp_doc */
    0,                                      /* tp_traverse */
    0,                                      /* tp_clear */
    0,                                      /* tp_richcompare */
    0,                                      /* tp_weaklistoffset */
    0,                                      /* tp_iter */
    0,                                      /* tp_iternext */
    0,                                      /* tp_methods */
    pyawd_AWDAttrBlock_members,             /* tp_members */
    0,                                      /* tp_getset */
    0,                                      /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    (initproc)pyawd_AWDAttrBlock_init,       /* tp_init */
    0,                                      /* tp_alloc */
    pyawd_AWDAttrBlock_new,                  /* tp_new */
};


