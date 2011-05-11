#ifndef _PYAWD_AWDWRITER_H
#define _PYAWD_AWDWRITER_H

#include <Python.h>

#include <awd/libawd.h>


extern PyTypeObject io_AWDWriterType;

typedef struct {
    PyObject_HEAD
} io_AWDWriter;


void __prepare_blocks(PyObject *, const char *, AWD *, void (*)(PyObject*,AWD*));
void __prepare_mesh_data(PyObject *, AWD *);

#endif
