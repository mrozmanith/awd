#include <Python.h>
#include <awd/libawd.h>

#include "AWD.h"
#include "AWDSubMesh.h"
#include "AWDMeshData.h"
#include "AWDMeshInst.h"


#if PYTHON_VERSION == 3
static PyModuleDef pyawd_mod = {
    PyModuleDef_HEAD_INIT,
    "pyawd",
    "Module for reading/writing Away3D data format (AWD).",
    -1,
    NULL, NULL, NULL, NULL, NULL
};
#else // Python 2.6
static PyMethodDef pyawd_methods[] = {
    {NULL, NULL, 0, NULL}        /* Sentinel */
};
#endif


/*
 * Python version-agnostic method to initialize pyawd module.
 * Invoked in different ways depending on whether this module
 * is compiled for Python 2.6 or 3.x.
*/
PyObject *_init_pyawd(PyObject *m)
{
    PyObject *pyawd_ver;
    PyObject *libawd_ver;
    char *version_str;


    version_str = (char*)malloc(64);
    snprintf(version_str, 64, "PyAWD %d.%d (libawd %d.%d)",
        PYAWD_MAJOR_VERSION, PYAWD_MINOR_VERSION,
        AWD_MAJOR_VERSION, AWD_MINOR_VERSION);
    pyawd_ver = Py_BuildValue("(i,i)", PYAWD_MAJOR_VERSION, PYAWD_MINOR_VERSION);
    libawd_ver = Py_BuildValue("(i,i)", AWD_MAJOR_VERSION, AWD_MINOR_VERSION);

    // Version "constants"
    PyModule_AddStringConstant(m, "version", version_str);
    PyModule_AddObject(m, "pyawd_version", pyawd_ver);
    PyModule_AddObject(m, "libawd_version", libawd_ver);

    // Enumeration constants
    PyModule_AddIntConstant(m, "UNCOMPRESSED", UNCOMPRESSED);
    PyModule_AddIntConstant(m, "DEFLATE", DEFLATE);
    PyModule_AddIntConstant(m, "LZMA", LZMA);

    PyModule_AddIntConstant(m, "VERTEX_STREAM", VERTICES);
    PyModule_AddIntConstant(m, "TRIANGLE_STREAM", TRIANGLES);
    PyModule_AddIntConstant(m, "UV_STREAM", UVS);
    PyModule_AddIntConstant(m, "VNORMAL_STREAM", VERTEX_NORMALS);
    PyModule_AddIntConstant(m, "VTANGENT_STREAM", VERTEX_TANGENTS);
    PyModule_AddIntConstant(m, "FNORMAL_STREAM", FACE_NORMALS);


    // Prepare class data types
    if ((PyType_Ready(&pyawd_AWDType) < 0)
        || (PyType_Ready(&pyawd_AWDMeshDataType) < 0)
        || (PyType_Ready(&pyawd_AWDMeshInstType) < 0)
        || (PyType_Ready(&pyawd_AWDSubMeshType) < 0))
        return NULL;

    // Add classes to module
    Py_INCREF(&pyawd_AWDType);
    PyModule_AddObject(m, "AWD", (PyObject *)&pyawd_AWDType);
    Py_INCREF(&pyawd_AWDMeshDataType);
    PyModule_AddObject(m, "AWDMeshData", (PyObject *)&pyawd_AWDMeshDataType);
    Py_INCREF(&pyawd_AWDSubMeshType);
    PyModule_AddObject(m, "AWDSubMesh", (PyObject *)&pyawd_AWDSubMeshType);
    Py_INCREF(&pyawd_AWDMeshInstType);
    PyModule_AddObject(m, "AWDMeshInst", (PyObject *)&pyawd_AWDMeshInstType);

    return m;
}



#if PYTHON_VERSION == 3
PyMODINIT_FUNC
PyInit_pyawd(void)
{
    PyObject *m;

    m = PyModule_Create(&pyawd_mod);
    if (m == NULL)
        return NULL;

    return _init_pyawd(m);
}
#else // Python 2.6
PyMODINIT_FUNC
initpyawd(void)
{
    PyObject *m;

    m = Py_InitModule("pyawd", pyawd_methods);

    _init_pyawd(m);
}
#endif


