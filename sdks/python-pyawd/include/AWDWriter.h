#ifndef _PYAWD_AWDWRITER_H
#define _PYAWD_AWDWRITER_H

#include <Python.h>

#include <awd/libawd.h>

#include "bcache.h"


extern PyTypeObject io_AWDWriterType;

typedef struct {
    PyObject_HEAD
} io_AWDWriter;


void __prepare_blocks(PyObject *, const char *, AWD *, pyawd_bcache *, void (*)(PyObject*,AWD*,pyawd_bcache*));
void __prepare_mesh_data(PyObject *, AWD *, pyawd_bcache *);
void __prepare_mesh_inst(PyObject *, AWD *, pyawd_bcache *);
void __prepare_texture(PyObject *, AWD *, pyawd_bcache *);
void __prepare_material(PyObject *, AWD *, pyawd_bcache *);

#endif
