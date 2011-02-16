#ifndef _PYAWD_AWDSUBMESH_H
#define _PYAWD_AWDSUBMESH_H

#include <Python.h>

#include <awd/mesh.h>


extern PyTypeObject pyawd_AWDSubMeshType;

typedef struct {
    PyObject_HEAD

    AWDSubMesh *ob_sub;

    //struct _pyawd_AWDSubMesh *  next;
} pyawd_AWDSubMesh;


void pyawd_AWDSubMesh_dealloc(pyawd_AWDSubMesh *);
PyObject *pyawd_AWDSubMesh_new(PyTypeObject *, PyObject *, PyObject *);

int pyawd_AWDSubMesh_init(pyawd_AWDSubMesh *, PyObject *, PyObject *);

PyObject *pyawd_AWDSubMesh_add_stream(pyawd_AWDSubMesh *, PyObject *, PyObject *);


#endif

