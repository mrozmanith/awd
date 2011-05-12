#ifndef _PYAWD_AWDATTRBLOCK_H
#define _PYAWD_AWDATTRBLOCK_H

#include <Python.h>

#include <awd/block.h>
#include <awd/mesh.h>


extern PyTypeObject pyawd_AWDAttrBlockType;

typedef struct {
    PyObject_HEAD
    PyObject *attributes;
} pyawd_AWDAttrBlock;


void pyawd_AWDAttrBlock_dealloc(pyawd_AWDAttrBlock *);
PyObject *pyawd_AWDAttrBlock_new(PyTypeObject *, PyObject *, PyObject *);

int pyawd_AWDAttrBlock_init(pyawd_AWDAttrBlock *, PyObject *, PyObject *);

void _apply_block_attributes(AWDBlock *block, pyawd_AWDAttrBlock *);


#endif

