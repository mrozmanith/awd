#include <Python.h>
#include <listobject.h>
#include <structmember.h>

#include "util.h"
#include "AWDMeshData.h"

#include <awd/libawd.h>



/**
 * free()
*/
void
pyawd_AWDMeshData_dealloc(pyawd_AWDMeshData *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDMeshData();
*/
PyObject *
pyawd_AWDMeshData_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWDMeshData *self;

    self = (pyawd_AWDMeshData *)type->tp_alloc(type, 0);
    if (self != NULL) {
    }

    return (PyObject *)self;
}


/**
 * AWDMeshData.__init__();
*/
int
pyawd_AWDMeshData_init(pyawd_AWDMeshData *self, PyObject *args, PyObject *kwds)
{
    self->ob_data = awd_create_mesh_data();
    return 0;
}



PyObject *
pyawd_AWDMeshData_add_sub_mesh(pyawd_AWDMeshData *self, PyObject *args, PyObject *kwds)
{
    int tris_len;
    int verts_len;
    awd_bool wide;

    PyObject *verts;
    PyObject *inds;
    PyObject *uvs;

    unsigned int len;


    // TODO: Add material
    static char *kwlist[] = { "vertices", "triangles", "uvs", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!O!|O!i", kwlist, 
        &PyList_Type, &verts, &PyList_Type, &inds, &PyList_Type, &uvs))
        return NULL;
    
    tris_len = PyList_Size(inds);
    verts_len = PyList_Size(verts);

    //wide = awdutil_check_flag(self->ob_awd, AWD_OPTIMIZE_FOR_ACCURACY);
    // TODO: Refactor to delay this until added to AWD
    wide = AWD_FALSE;
    if (wide) {
        awd_float64 *verts_arr;
        awd_uint32 *inds_arr;
        awd_float64 *uvs_arr;
    }
    else {
        awd_float32 *verts_arr;
        awd_uint16 *inds_arr;
        awd_float32 *uvs_arr;

        verts_arr = malloc(verts_len * sizeof(awd_float32));
        if (!pyawdutil_pylist_to_float32(verts, verts_arr, verts_len))
            return NULL;

        inds_arr = malloc(tris_len * sizeof(awd_uint16));
        if (!pyawdutil_pylist_to_uint16(inds, inds_arr, tris_len))
            return NULL;
        
        // TODO: Postpone until add to AWD
        /*
        awd_mesh_add_sub_vtu(
            self->ob_data, 
            verts_len/3, 
            tris_len/3,
            verts_arr,
            inds_arr,
            NULL, // TODO: Deal with uvs
            NULL, // Material
            AWD_FALSE // wide?
        );
        */
    }

    Py_RETURN_NONE;
}



/**
 * Method dictionary
*/
PyMethodDef pyawd_AWDMeshData_methods[] = {
    { "add_sub_mesh", (PyCFunction)pyawd_AWDMeshData_add_sub_mesh, METH_VARARGS | METH_KEYWORDS,
        "Add a sub-mesh (geometry) to a mesh data block." },

    { NULL }
};



/**
 * Type object
*/
PyTypeObject pyawd_AWDMeshDataType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDMeshData",                    /* tp_name */
    sizeof(pyawd_AWDMeshData),              /* tp_basicsize */
    0,                                      /* tp_itemsize */
    (destructor)pyawd_AWDMeshData_dealloc,  /* tp_dealloc */
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
    pyawd_AWDMeshData_methods,              /* tp_methods */
    0,                                      /* tp_members */
    0,                                      /* tp_getset */
    0,                                      /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    (initproc)pyawd_AWDMeshData_init,       /* tp_init */
    0,                                      /* tp_alloc */
    pyawd_AWDMeshData_new,                  /* tp_new */
};


