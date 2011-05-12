#include <stdio.h>

#include <Python.h>
#include "structmember.h"

#include <awd/libawd.h>

#include "util.h"
#include "AWDWriter.h"



/**
 * free()
*/
void
io_AWDWriter_dealloc(io_AWDWriter *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDWriter();
*/
PyObject *
io_AWDWriter_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    io_AWDWriter *self;

    self = (io_AWDWriter *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}


/**
 * AWDWriter.__init__();
*/
static int
io_AWDWriter_init(io_AWDWriter *self, PyObject *args, PyObject *kwds)
{
    return 0;
}





/**
 * AWDWriter.write(awd, file)
*/
PyObject *
io_AWDWriter_flush(io_AWDWriter *self, PyObject *args)
{
    AWD *lawd_awd;
    PyObject *awd_obj;
    PyObject *fobj;
    int fd;

#if PYTHON_VERSION == 3
    extern PyTypeObject PyIOBase_Type;
    if (!PyArg_ParseTuple(args, "OO!", &awd_obj, &PyIOBase_Type, &fobj))
        return NULL;
#else
    extern PyTypeObject PyFile_Type;
    if (!PyArg_ParseTuple(args, "OO!", &awd_obj, &PyFile_Type, &fobj))
        return NULL;
#endif

    fd = PyObject_AsFileDescriptor(fobj);

    lawd_awd = new AWD(UNCOMPRESSED,0);

    if (fd >= 0) {
        pyawd_bcache *bcache;

        bcache = (pyawd_bcache *)malloc(sizeof(pyawd_bcache));
        pyawd_bcache_init(bcache);

        __prepare_blocks(awd_obj, "uvanim_blocks", lawd_awd, bcache, __prepare_uvanim);
        __prepare_blocks(awd_obj, "texture_blocks", lawd_awd, bcache, __prepare_texture);
        __prepare_blocks(awd_obj, "material_blocks", lawd_awd, bcache, __prepare_material);
        __prepare_blocks(awd_obj, "skeleton_blocks", lawd_awd, bcache, __prepare_skeleton);
        __prepare_blocks(awd_obj, "skelpose_blocks", lawd_awd, bcache, __prepare_skelpose);
        __prepare_blocks(awd_obj, "skelanim_blocks", lawd_awd, bcache, __prepare_skelanim);
        __prepare_blocks(awd_obj, "mesh_data_blocks", lawd_awd, bcache, __prepare_mesh_data);
        __prepare_blocks(awd_obj, "mesh_inst_blocks", lawd_awd, bcache, __prepare_mesh_inst);

        // Write buffer
        lawd_awd->flush(fd);
        Py_RETURN_NONE;
    }
    else {
        return NULL;
    }
}





/**
 * Method dictionary
*/
PyMethodDef io_AWDWriter_methods[] = {
    { "write", (PyCFunction)io_AWDWriter_flush, METH_VARARGS,
        "Write everything in an AWD object to an output stream." },

    { NULL }
};




/**
 * Type object 
*/
PyTypeObject io_AWDWriterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDWriter",                      // tp_name
    sizeof(io_AWDWriter),                // tp_basicsize
    0,                                      // tp_itemsize
    (destructor)io_AWDWriter_dealloc,    // tp_dealloc
    0,                                      // tp_print
    0,                                      // tp_getattr
    0,                                      // tp_setattr
    0,                                      // tp_reserved
    0,                                      // tp_repr
    0,                                      // tp_as_number
    0,                                      // tp_as_sequence
    0,                                      // tp_as_mapping
    0,                                      // tp_hash 
    0,                                      // tp_call
    0,                                      // tp_str
    0,                                      // tp_getattro
    0,                                      // tp_setattro
    0,                                      // tp_as_buffer
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,                // tp_flags
    "AWD document object.",                 // tp_doc
    0,                                      // tp_traverse
    0,                                      // tp_clear
    0,                                      // tp_richcompare
    0,                                      // tp_weaklistoffset
    0,                                      // tp_iter
    0,                                      // tp_iternext
    io_AWDWriter_methods,                // tp_methods
    0,                                      // tp_members
    0,                                      // tp_getset
    0,                                      // tp_base
    0,                                      // tp_dict
    0,                                      // tp_descr_get
    0,                                      // tp_descr_set
    0,                                      // tp_dictoffset
    (initproc)io_AWDWriter_init,         // tp_init
    0,                                      // tp_alloc
    io_AWDWriter_new,                    // tp_new
};



void
__prepare_blocks(PyObject *pyawd_AWD, const char *list_attr, AWD *awd, pyawd_bcache *bcache, void (*prep_func)(PyObject*, AWD*, pyawd_bcache *))
{
    PyObject *list;

    list = PyObject_GetAttrString(pyawd_AWD, list_attr);
    if (list && PyObject_IsInstance(list, (PyObject *)&PyList_Type)) {
        int i;
        int len;

        len = PyList_Size(list);
        for (i=0; i<len; i++) {
            prep_func(PyList_GetItem(list, i), awd, bcache);
        }
    }
}

