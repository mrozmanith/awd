#include <Python.h>
#include "structmember.h"

#include "AWDTexture.h"

#include <awd/libawd.h>



/**
 * free()
*/
void
pyawd_AWDTexture_dealloc(pyawd_AWDTexture *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDTexture();
*/
PyObject *
pyawd_AWDTexture_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWDTexture *self;

    self = (pyawd_AWDTexture *)type->tp_alloc(type, 0);
    if (self != NULL) {
    }

    return (PyObject *)self;
}


/**
 * AWDTexture.__init__();
*/
int
pyawd_AWDTexture_init(pyawd_AWDTexture *self, PyObject *args, PyObject *kwds)
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
    self->url = Py_None;

    return 0;
}




/**
 * AWDTexture.url (setter)
*/
static int
pyawd_AWDTexture_set_url(pyawd_AWDTexture *self, PyObject *value, void *closure)
{
    if (value == self->url)
        return 0;

    if (PyObject_IsInstance(value, (PyObject *)&PyString_Type)) {
        if (self->url)
            Py_DECREF(self->url);

        self->url = value;
        Py_INCREF(self->url);

        return 0;
    }
    else {
        PyErr_SetString(PyExc_RuntimeError, "Value must be string or None");
        return -1;
    }
}


/**
 * AWDTexture.url (getter)
*/
static PyObject *
pyawd_AWDTexture_get_url(pyawd_AWDTexture *self, void *closure)
{
    return self->url;
}



/**
 * Method dictionary
*/
PyMethodDef pyawd_AWDTexture_methods[] = {
    { NULL }
};


/**
 * Getter/setter dictionary
*/
PyGetSetDef pyawd_AWDTexture_getset[] = {
    { "url",
        (getter)pyawd_AWDTexture_get_url,
        (setter)pyawd_AWDTexture_set_url,
        "Defines URL from which external texture data should be retrieved (ignored for embedded)",
        NULL },
    { NULL }
};



/**
 * Type object
*/
PyTypeObject pyawd_AWDTextureType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDTexture",                     // tp_name 
    sizeof(pyawd_AWDTexture),               // tp_basicsize 
    0,                                      // tp_itemsize 
    (destructor)pyawd_AWDTexture_dealloc,   // tp_dealloc 
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
    pyawd_AWDTexture_methods,               // tp_methods 
    0,                                      // tp_members 
    pyawd_AWDTexture_getset,                // tp_getset 
    0,                                      // tp_base 
    0,                                      // tp_dict 
    0,                                      // tp_descr_get 
    0,                                      // tp_descr_set 
    0,                                      // tp_dictoffset 
    (initproc)pyawd_AWDTexture_init,        // tp_init 
    0,                                      // tp_alloc 
    pyawd_AWDTexture_new,                   // tp_new 
};


void
pyawd_AWDTexture__prep(pyawd_AWDTexture *self)
{
    char *name;
    char *url;
    int name_len;
    int url_len;
    awd_uint8 type;

    type = (char)PyInt_AsLong(self->type);
    name = PyString_AsString(self->name);
    name_len = PyString_Size(self->name);

    self->lawd_obj = new AWDTexture(type, name, name_len);
    if (self->url != Py_None) {
        url = PyString_AsString(self->url);
        url_len = PyString_Size(self->url);
        self->lawd_obj->set_url(url, url_len);
    }
}
