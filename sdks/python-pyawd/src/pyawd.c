#include <Python.h>
#include <awd/libawd.h>

#include "AWD.h"
#include "AWDMeshData.h"


static PyModuleDef pyawd_mod = {
    PyModuleDef_HEAD_INIT,
    "pyawd",
    "Module for reading/writing Away3D data format (AWD).",
    -1,
    NULL, NULL, NULL, NULL, NULL
};


PyMODINIT_FUNC
PyInit_pyawd(void)
{
    PyObject *m;

    if ((PyType_Ready(&pyawd_AWDType) < 0)
        || (PyType_Ready(&pyawd_AWDMeshDataType) < 0))
        return NULL;

    m = PyModule_Create(&pyawd_mod);
    if (m == NULL)
        return NULL;

    // Constants
    PyModule_AddIntConstant(m, "PYAWD_MAJOR_VERSION", 1);
    PyModule_AddIntConstant(m, "PYAWD_MINOR_VERSION", 0);
    PyModule_AddIntConstant(m, "UNCOMPRESSED", UNCOMPRESSED);
    PyModule_AddIntConstant(m, "DEFLATE", DEFLATE);
    PyModule_AddIntConstant(m, "LZMA", LZMA);
    PyModule_AddIntConstant(m, "LIBAWD_MAJOR_VERSION", AWD_MAJOR_VERSION);
    PyModule_AddIntConstant(m, "LIBAWD_MINOR_VERSION", AWD_MINOR_VERSION);

    Py_INCREF(&pyawd_AWDType);
    PyModule_AddObject(m, "AWD", (PyObject *)&pyawd_AWDType);

    Py_INCREF(&pyawd_AWDMeshDataType);
    PyModule_AddObject(m, "AWDMeshData", (PyObject *)&pyawd_AWDMeshDataType);

    return m;
}


