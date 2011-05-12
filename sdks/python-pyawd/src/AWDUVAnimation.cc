#include <Python.h>
#include "structmember.h"

#include "AWDUVAnimation.h"
#include "AWDMatrix2x3.h"

#include <awd/libawd.h>



/**
 * free()
*/
void
pyawd_AWDUVAnimation_dealloc(pyawd_AWDUVAnimation *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDUVAnimation();
*/
PyObject *
pyawd_AWDUVAnimation_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyawd_AWDUVAnimation *self;

    self = (pyawd_AWDUVAnimation *)type->tp_alloc(type, 0);
    if (self != NULL) {
    }

    return (PyObject *)self;
}


/**
 * AWDUVAnimation.__init__();
*/
int
pyawd_AWDUVAnimation_init(pyawd_AWDUVAnimation *self, PyObject *args, PyObject *kwds)
{
    PyObject *name;

    char *kwlist[] = {"name"};

    name = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O!", kwlist,
        &PyString_Type, &name ))
        return -1;

    if (name == NULL)
        name = PyString_FromString("");

    Py_INCREF(name);
    self->name = name;

    self->frames = PyList_New(0);
    Py_INCREF(self->frames);

    return 0;
}



/**
 * Member dictionary
*/
PyMemberDef pyawd_AWDUVAnimation_members[] = {
    { "frames", T_OBJECT_EX, offsetof(pyawd_AWDUVAnimation, frames), READONLY,
        "Frames in the animation." },
    { NULL }
};




/**
 * Type object
*/
PyTypeObject pyawd_AWDUVAnimationType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDUVAnimation",                     // tp_name 
    sizeof(pyawd_AWDUVAnimation),               // tp_basicsize 
        0,                                      // tp_itemsize 
    (destructor)pyawd_AWDUVAnimation_dealloc,   // tp_dealloc 
    0,                                          // tp_print 
    0,                                          // tp_getattr 
    0,                                          // tp_setattr 
    0,                                          // tp_reserved 
    0,                                          // tp_repr 
    0,                                          // tp_as_number 
    0,                                          // tp_as_sequence 
    0,                                          // tp_as_mapping 
    0,                                          // tp_hash  
    0,                                          // tp_call 
    0,                                          // tp_str 
    0,                                          // tp_getattro 
    0,                                          // tp_setattro 
    0,                                          // tp_as_buffer 
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,                    // tp_flags 
    "AWD mesh representation.",                 // tp_doc 
    0,                                          // tp_traverse 
    0,                                          // tp_clear 
    0,                                          // tp_richcompare 
    0,                                          // tp_weaklistoffset 
    0,                                          // tp_iter 
    0,                                          // tp_iternext 
    0,                                          // tp_methods 
    pyawd_AWDUVAnimation_members,               // tp_members 
    0,                                          // tp_getset 
    0,                                          // tp_base 
    0,                                          // tp_dict 
    0,                                          // tp_descr_get 
    0,                                          // tp_descr_set 
    0,                                          // tp_dictoffset 
    (initproc)pyawd_AWDUVAnimation_init,        // tp_init 
    0,                                          // tp_alloc 
    pyawd_AWDUVAnimation_new,                   // tp_new 
};


void
pyawd_AWDUVAnimation__prep(pyawd_AWDUVAnimation *self)
{
    int i;
    int len;
    char *name;
    int name_len;

    name = PyString_AsString(self->name);
    name_len = PyString_Size(self->name);

    self->lawd_obj = new AWDUVAnimation(name, name_len);

    len = PyList_Size(self->frames);
    for (i=0; i<len; i++) {
        pyawd_AWDMatrix2x3 *mtx;
        mtx = (pyawd_AWDMatrix2x3 *)PyList_GetItem(self->frames, i);
        self->lawd_obj->set_next_frame_tf(mtx->raw_data);
    }
}
