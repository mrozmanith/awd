#include <Python.h>
#include "structmember.h"

#include "util.h"
#include "AWDMatrix4.h"

#include <awd/libawd.h>



/**
 * free()
*/
void
pyawd_AWDMatrix4_dealloc(pyawd_AWDMatrix4 *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
    free(self->raw_data);
}


/**
 * AWDMatrix4();
*/
PyObject *
pyawd_AWDMatrix4_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWDMatrix4 *self;

    self = (pyawd_AWDMatrix4 *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->raw_data = awdutil_id_mtx4(NULL);
    }
    return (PyObject *)self;
}


/**
 * AWDMatrix4.__init__();
*/
int
pyawd_AWDMatrix4_init(pyawd_AWDMatrix4 *self, PyObject *args, PyObject *kwds)
{
    PyListObject *mtx_list;

    mtx_list = NULL;
    if (!PyArg_ParseTuple(args, "|O!", &PyList_Type, &mtx_list))
        return NULL;

    if (mtx_list != NULL) {
        unsigned int mtx_len;
        Py_ssize_t list_len;

        mtx_len = 16;
        list_len = PyList_Size((PyObject*)mtx_list);
        if (list_len < 16)
            mtx_len = list_len;
        
        pyawdutil_pylist_to_float64((PyObject *)mtx_list, self->raw_data, mtx_len);
    }
    
    return 0;
}


static int
pyawd_AWDMatrix4_mp_length(pyawd_AWDMatrix4 *self)
{
    return 16;
}


PyObject *
pyawd_AWDMatrix4_mp_subscript(pyawd_AWDMatrix4 *self, PyObject *arg)
{
    if (PyInt_Check(arg)) {
        long idx;

        idx = PyInt_AsLong(arg);
        if (idx >= 0 && idx < 16) {
            return PyFloat_FromDouble(self->raw_data[idx]);
        }
        else {
            PyErr_Format(PyExc_RuntimeError, "Index %ld out of bounds.", idx);
            return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_RuntimeError, "Index must be integer");
        return NULL;
    }
}


static int
pyawd_AWDMatrix4_mp_ass_subscript(pyawd_AWDMatrix4 *self, PyObject *arg, PyObject *val)
{
    if (PyInt_Check(arg)) {
        if (PyNumber_Check(val)) {
            long idx;

            idx = PyInt_AsLong(arg);
            if (idx >= 0 && idx < 16) {
                PyObject *f = PyNumber_Float(val);
                if (f) {
                    self->raw_data[idx] = PyFloat_AsDouble(f);
                    return 0;
                }
                else {
                    PyErr_SetString(PyExc_RuntimeError, "Could not convert value to float");
                    return -1;
                }
            }
            else {
                PyErr_Format(PyExc_RuntimeError, "Index %ld out of bounds", idx);
                return -1;
            }
        }
        else {
            PyErr_SetString(PyExc_RuntimeError, "Value must be float");
            return -1;
        }
    }
    else {
        PyErr_SetString(PyExc_RuntimeError, "index must be integer");
        return -1;
    }
}


PyMappingMethods pyawd_AWDMatrix4Mapping = {
    (lenfunc)pyawd_AWDMatrix4_mp_length,
    (binaryfunc)pyawd_AWDMatrix4_mp_subscript,
    (objobjargproc)pyawd_AWDMatrix4_mp_ass_subscript,
};



/**
 * Type object
*/
PyTypeObject pyawd_AWDMatrix4Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDMatrix4",                     // tp_name
    sizeof(pyawd_AWDMatrix4),               // tp_basicsize
    0,                                      // tp_itemsize
    (destructor)pyawd_AWDMatrix4_dealloc,   // tp_dealloc
    0,                                      // tp_print
    0,                                      // tp_getattr
    0,                                      // tp_setattr
    0,                                      // tp_reserved
    0,                                      // tp_repr
    0,                                      // tp_as_number
    0,                                      // tp_as_sequence
    &pyawd_AWDMatrix4Mapping,               // tp_as_mapping
    0,                                      // tp_hash 
    0,                                      // tp_call
    0,                                      // tp_str
    0,                                      // tp_getattro
    0,                                      // tp_setattro
    0,                                      // tp_as_buffer
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,                // tp_flags
    "AWD matrix representation.",           // tp_doc
    0,                                      // tp_traverse
    0,                                      // tp_clear
    0,                                      // tp_richcompare
    0,                                      // tp_weaklistoffset
    0,                                      // tp_iter
    0,                                      // tp_iternext
    0,                                      // tp_methods
    0,                                      // tp_members
    0,                                      // tp_getset
    0,                                      // tp_base
    0,                                      // tp_dict
    0,                                      // tp_descr_get
    0,                                      // tp_descr_set
    0,                                      // tp_dictoffset
    (initproc)pyawd_AWDMatrix4_init,        // tp_init
    0,                                      // tp_alloc
    pyawd_AWDMatrix4_new,                   // tp_new
};


