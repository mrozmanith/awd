#include <Python.h>
#include <listobject.h>
#include <structmember.h>

#include "util.h"
#include "AWDMeshData.h"
#include "AWDSubMesh.h"
#include "AWDAttrBlock.h"

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

    return (PyObject *)self;
}


/**
 * AWDMeshData.__init__();
*/
int
pyawd_AWDMeshData_init(pyawd_AWDMeshData *self, PyObject *args, PyObject *kwds)
{
    self->ob_data = new AWDMeshData();
    self->skeleton = NULL;
    self->next = NULL;

    // Superclass __init__
    pyawd_AWDAttrBlock_init((pyawd_AWDAttrBlock *)self, args, kwds);

    return 0;
}



/**
 * AWDMeshData.add_sub_mesh()
*/
PyObject *
pyawd_AWDMeshData_add_sub_mesh(pyawd_AWDMeshData *self, PyObject *args, PyObject *kwds)
{
    int tris_len;
    int verts_len;
    awd_bool wide;

    PyObject *sub_arg;
    pyawd_AWDSubMesh *sub;

    if (!PyArg_ParseTuple(args, "O!", &pyawd_AWDSubMeshType, &sub_arg))
        return NULL;
    
    sub = (pyawd_AWDSubMesh *)sub_arg;
    self->ob_data->add_sub_mesh(sub->ob_sub);

    Py_RETURN_NONE;
}


/**
 * AWDMeshData.skeleton (setter)
*/
static int
pyawd_AWDMeshData_set_skeleton(pyawd_AWDMeshData *self, PyObject *value, void *closure)
{
    if (PyObject_IsInstance(value, (PyObject *)&pyawd_AWDSkeletonType)) {
        self->skeleton = (pyawd_AWDSkeleton *)value;
        self->ob_data->set_skeleton(self->skeleton->ob_skeleton);
        return 0;
    }
    else {
        return -1;
    }
}


/**
 * AWDMeshData.skeleton (getter)
*/
static PyObject *
pyawd_AWDMeshData_get_skeleton(pyawd_AWDMeshData *self, void *closure)
{
    if (self->skeleton != NULL)
        return (PyObject *)self->skeleton;

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
 * Getter/setter dictionary
*/
PyGetSetDef pyawd_AWDMeshData_getset[] = {
    { "skeleton",
        (getter)pyawd_AWDMeshData_get_skeleton,
        (setter)pyawd_AWDMeshData_set_skeleton,
        "Defines the skeleton that this mesh is bound to (if any)",
        NULL },
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
    pyawd_AWDMeshData_getset,               /* tp_getset */
    &pyawd_AWDAttrBlockType,                /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    (initproc)pyawd_AWDMeshData_init,       /* tp_init */
    0,                                      /* tp_alloc */
    pyawd_AWDMeshData_new,                  /* tp_new */
};


