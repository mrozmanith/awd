#include <Python.h>
#include "structmember.h"

#include "AWDMaterial.h"
#include "AWDTexture.h"

#include <awd/libawd.h>



/**
 * free()
*/
void
pyawd_AWDMaterial_dealloc(pyawd_AWDMaterial *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDMaterial();
*/
PyObject *
pyawd_AWDMaterial_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWDMaterial *self;

    self = (pyawd_AWDMaterial *)type->tp_alloc(type, 0);
    if (self != NULL) {
    }

    return (PyObject *)self;
}


/**
 * AWDMaterial.__init__();
*/
int
pyawd_AWDMaterial_init(pyawd_AWDMaterial *self, PyObject *args, PyObject *kwds)
{
    PyObject *name;
    PyObject *type;

    char *kwlist[] = {"type", "name"};

    name = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!|O!O!", kwlist,
        &PyInt_Type, &type, &PyString_Type, &name ))
        return -1;

    if (name == NULL)
        name = PyString_FromString("");

    Py_INCREF(type);
    self->type = type;

    Py_INCREF(name);
    self->name = name;

    Py_INCREF(Py_None);
    self->texture = Py_None;

    return 0;
}




/**
 * AWDMaterial.url (setter)
*/
static int
pyawd_AWDMaterial_set_texture(pyawd_AWDMaterial *self, PyObject *value, void *closure)
{
    if (value == self->texture)
        return 0;

    if (PyObject_IsInstance(value, (PyObject *)&pyawd_AWDTextureType)) {
        if (self->texture)
            Py_DECREF(self->texture);

        self->texture= value;
        Py_INCREF(self->texture);

        return 0;
    }
    else {
        PyErr_SetString(PyExc_RuntimeError, "Value must be string or None");
        return -1;
    }
}


/**
 * AWDMaterial.url (getter)
*/
static PyObject *
pyawd_AWDMaterial_get_texture(pyawd_AWDMaterial *self, void *closure)
{
    return self->texture;
}



/**
 * Method dictionary
*/
PyMethodDef pyawd_AWDMaterial_methods[] = {
    { NULL }
};


/**
 * Getter/setter dictionary
*/
PyGetSetDef pyawd_AWDMaterial_getset[] = {
    { "texture",
        (getter)pyawd_AWDMaterial_get_texture,
        (setter)pyawd_AWDMaterial_set_texture,
        "Defines which texture should be used (for bitmap materials only)",
        NULL },
    { NULL }
};



/**
 * Type object
*/
PyTypeObject pyawd_AWDMaterialType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDMaterial",                     // tp_name 
    sizeof(pyawd_AWDMaterial),               // tp_basicsize 
    0,                                      // tp_itemsize 
    (destructor)pyawd_AWDMaterial_dealloc,   // tp_dealloc 
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
    "AWD mesh representation.",             // tp_doc 
    0,                                      // tp_traverse 
    0,                                      // tp_clear 
    0,                                      // tp_richcompare 
    0,                                      // tp_weaklistoffset 
    0,                                      // tp_iter 
    0,                                      // tp_iternext 
    pyawd_AWDMaterial_methods,               // tp_methods 
    0,                                      // tp_members 
    pyawd_AWDMaterial_getset,                // tp_getset 
    0,                                      // tp_base 
    0,                                      // tp_dict 
    0,                                      // tp_descr_get 
    0,                                      // tp_descr_set 
    0,                                      // tp_dictoffset 
    (initproc)pyawd_AWDMaterial_init,        // tp_init 
    0,                                      // tp_alloc 
    pyawd_AWDMaterial_new,                   // tp_new 
};


void
pyawd_AWDMaterial__prep(pyawd_AWDMaterial *self)
{
    char *name;
    int name_len;
    awd_uint8 type;

    type = (char)PyInt_AsLong(self->type);
    name = PyString_AsString(self->name);
    name_len = PyString_Size(self->name);

    self->lawd_obj = new AWDSimpleMaterial(type, name, name_len);
    if (self->texture != Py_None) {
        pyawd_AWDTexture *tex;
        tex = (pyawd_AWDTexture *)self->texture;
        self->lawd_obj->set_texture(tex->lawd_obj);
    }
}
