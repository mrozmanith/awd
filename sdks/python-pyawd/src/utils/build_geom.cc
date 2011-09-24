#include <awd/mesh.h>
#include <awd/geomlawd_util.h>
#include <cstdio>

#include "lawd_utils.h"

PyObject *
cpyawd_lawd_util_build_geom(PyObject *self, PyObject *args)
{
    int i, len;
    int ret;
    PyObject *verts_list;
    PyObject *py_md;
    AWDMeshData *lawd_md;
    AWDGeomUtil *lawd_util;
    AWDSubMesh *lawd_sub;

    if (!PyArg_ParseTuple(args, "O!O", &PyList_Type, &verts_list, &py_md))
        return NULL;

    lawd_util = new AWDGeomUtil();
    len = PyList_Size(verts_list);
    for (i=0; i<len; i++) {
        PyObject *vert_obj = PyList_GetItem(verts_list, i);
        
        if (PyTuple_Check(vert_obj)) {
            unsigned int idx;
            double x, y, z, u, v, nx, ny, nz;
            bool force_hard;

            idx = PyLong_AsLong(PyTuple_GetItem(vert_obj, 0));
            x = PyFloat_AsDouble(PyTuple_GetItem(vert_obj, 1));
            y = PyFloat_AsDouble(PyTuple_GetItem(vert_obj, 2));
            z = PyFloat_AsDouble(PyTuple_GetItem(vert_obj, 3));
            u = PyFloat_AsDouble(PyTuple_GetItem(vert_obj, 4));
            v = PyFloat_AsDouble(PyTuple_GetItem(vert_obj, 5));
            nx = PyFloat_AsDouble(PyTuple_GetItem(vert_obj, 6));
            ny = PyFloat_AsDouble(PyTuple_GetItem(vert_obj, 7));
            nz = PyFloat_AsDouble(PyTuple_GetItem(vert_obj, 8));
            force_hard = PyLong_AsLong(PyTuple_GetItem(vert_obj, 9));

            lawd_util->append_vert_data(idx, x, y, z, u, v, nx, ny, nz, force_hard);
        }
    }

    lawd_md = new AWDMeshData("dummy", 5);
    ret = lawd_util->build_geom(lawd_md);

    len = lawd_md->get_num_lawd_subs();
    for (i=0; i<len; i++) {
        int stream_idx;
        int num_streams;

        PyObject *py_sub;

        lawd_sub = lawd_md->get_sub_at(i);
        num_streams = lawd_sub->get_num_streams();
        for (stream_idx=0; stream_idx<num_streams; stream_idx++) {
            AWDDataStream *stream;

            stream = lawd_sub->get_stream_at(stream_idx);
        }
    }

    return PyLong_FromLong(ret);
}

