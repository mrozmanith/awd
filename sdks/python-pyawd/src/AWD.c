#include <Python.h>
#include "structmember.h"

#include <awd/libawd.h>

#include "AWD.h"
#include "AWDMeshData.h"



/**
 * free()
*/
void
pyawd_AWD_dealloc(pyawd_AWD *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWD();
*/
PyObject *
pyawd_AWD_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWD *self;

    self = (pyawd_AWD *)type->tp_alloc(type, 0);
    if (self != NULL) {
    }

    return (PyObject *)self;
}


/**
 * AWD.__init__();
*/
int
pyawd_AWD_init(pyawd_AWD *self, PyObject *args, PyObject *kwds)
{
    awd_uint16 flags;
    awd_uint8 compression;
    PyObject *streaming;
    PyObject *wide;


    char *kwlist[] = {"compression", "streaming", "wide", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|bbb", kwlist,
                                    &compression, &streaming, &wide))
        return -1;

    flags = 0;
    if (streaming == Py_True)
        flags |= AWD_STREAMING;
    if (wide == Py_True)
        flags |= AWD_OPTIMIZE_FOR_ACCURACY;

    self->ob_awd = awd_create(compression, flags);

    return 0;
}


/**
 * AWD.add_mesh_data()
*/
PyObject *
pyawd_AWD_add_mesh_data(pyawd_AWD *self, PyObject *args)
{
    pyawd_AWDMeshData *mesh;
    PyObject *arg;

    if (!PyArg_ParseTuple(args, "O!", &pyawd_AWDMeshDataType, &arg))
        return NULL;

    // TODO: Add to ob_awd
    mesh = (pyawd_AWDMeshData *) arg;

    Py_RETURN_NONE;
}


/**
 * AWD.add_mesh_inst()
*/
PyObject *
pyawd_AWD_add_mesh_inst(pyawd_AWD *self, PyObject *args, PyObject *kwds)
{
    pyawd_AWDMeshData *mesh;
    PyObject *data_arg;
    PyObject *mtx_arg;
    awd_float64 *mtx;

    char *kwlist[] = { "data", "transform", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!|O!", kwlist, 
                                    &pyawd_AWDMeshDataType, &data_arg,
                                    &PyList_Type, &mtx_arg))
        return NULL;


    mtx = malloc(16 * sizeof(awd_float64));
    if (!pyawdutil_pylist_to_float64(mtx_arg, mtx, 16)) {
        // TODO: Set exception
        return NULL;
    }

    mesh = (pyawd_AWDMeshData *) data_arg;
    awd_add_mesh_inst(self->ob_awd, mesh->ob_data, mtx, NULL);


    Py_RETURN_NONE;
}


/**
 * AWD.flush()
*/
PyObject *
pyawd_AWD_flush(pyawd_AWD *self, PyObject *args)
{
    PyObject *fobj;
    FILE *fp;
    int fd;

    extern PyTypeObject PyIOBase_Type;


    if (!PyArg_ParseTuple(args, "O!", &PyIOBase_Type, &fobj))
        return NULL;

    fd = PyObject_AsFileDescriptor(fobj);

    if (fd >= 0) {
        awd_flush(self->ob_awd, fd);
        close(fd);

        Py_RETURN_NONE;
    }
    else {
        return NULL;
    }
}





/**
 * Method dictionary
*/
PyMethodDef pyawd_AWD_methods[] = {
    { "add_mesh_data", (PyCFunction)pyawd_AWD_add_mesh_data, METH_VARARGS,
        "Add a mesh data block to the AWD document." },

    { "add_mesh_inst", (PyCFunction)pyawd_AWD_add_mesh_inst, METH_VARARGS | METH_KEYWORDS,
        "Add a mesh instance block to the AWD document." },

    { "flush", (PyCFunction)pyawd_AWD_flush, METH_VARARGS,
        "Flush everything in the AWD object to an output stream." },

    { NULL }
};



/**
 * Type object 
*/
PyTypeObject pyawd_AWDType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWD",                        /* tp_name */
    sizeof(pyawd_AWD),                  /* tp_basicsize */
    0,                                  /* tp_itemsize */
    (destructor)pyawd_AWD_dealloc,      /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_reserved */
    0,                                  /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash  */
    0,                                  /* tp_call */
    0,                                  /* tp_str */
    0,                                  /* tp_getattro */
    0,                                  /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,            /* tp_flags */
    "AWD document object.",             /* tp_doc */
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    pyawd_AWD_methods,                  /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc)pyawd_AWD_init,           /* tp_init */
    0,                                  /* tp_alloc */
    pyawd_AWD_new,                      /* tp_new */
};



