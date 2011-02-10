#include <Python.h>
#include <listobject.h>
#include <structmember.h>

#include "util.h"
#include "AWDSubMesh.h"

#include <awd/libawd.h>



/**
 * free()
*/
void
pyawd_AWDSubMesh_dealloc(pyawd_AWDSubMesh *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDSubMesh();
*/
PyObject *
pyawd_AWDSubMesh_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWDSubMesh *self;

    self = (pyawd_AWDSubMesh *)type->tp_alloc(type, 0);

    return (PyObject *)self;
}


/**
 * AWDSubMesh.__init__();
*/
int
pyawd_AWDSubMesh_init(pyawd_AWDSubMesh *self, PyObject *args, PyObject *kwds)
{
    //TODO: Add material
    self->ob_sub = awd_create_sub_mesh();

    return 0;
}



PyObject *
pyawd_AWDSubMesh_add_stream(pyawd_AWDSubMesh *self, PyObject *args, PyObject *kwds)
{
    AWD_stream_type str_type;
    AWD_data_str_ptr data;
    PyObject *data_list;
    size_t data_len;

    if (!PyArg_ParseTuple(args, "iO!", &str_type, &PyList_Type, &data_list))
        return NULL;

    data_len = PyList_Size(data_list);

    switch (str_type) {
        case UVS:
        case VERTICES:
            data.f64 = malloc(data_len * sizeof(awd_float64));
            pyawdutil_pylist_to_float64(data_list, data.f64, data_len);
            break;
        case TRIANGLES:
            data.ui32 = malloc(data_len * sizeof(awd_uint32));
            pyawdutil_pylist_to_uint32(data_list, data.ui32, data_len);
            break;
        default:
            // TODO: Raise "unknown type" exception
            return NULL;
    }

    awd_sub_mesh_add_stream(self->ob_sub, str_type, data_len, data);

    Py_RETURN_NONE;
}




/**
 * Method dictionary
*/
PyMethodDef pyawd_AWDSubMesh_methods[] = {
    { "add_stream", (PyCFunction)pyawd_AWDSubMesh_add_stream, METH_VARARGS,
        "Add a stream of data (e.g. vertices) to the sub-mesh." },
    { NULL },
};



/**
 * Type object
*/
PyTypeObject pyawd_AWDSubMeshType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDSubMesh",                    /* tp_name */
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
    pyawd_AWDSubMesh_methods,               /* tp_methods */
    0,                                      /* tp_members */
    0,                                      /* tp_getset */
    0,                                      /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    (initproc)pyawd_AWDSubMesh_init,       /* tp_init */
    0,                                      /* tp_alloc */
    pyawd_AWDSubMesh_new,                  /* tp_new */
};


