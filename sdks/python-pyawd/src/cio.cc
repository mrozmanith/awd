
#include <Python.h>
#include <awd/libawd.h>

#include "AWDWriter.h"


#if PYTHON_VERSION == 3
static PyModuleDef cio_mod = {
    PyModuleDef_HEAD_INIT,
    "cio",
    "Module for reading/writing Away3D data format (AWD).",
    -1,
    NULL, NULL, NULL, NULL, NULL
};
#else // Python 2.6
static PyMethodDef cio_methods[] = {
    {NULL, NULL, 0, NULL}        /* Sentinel */
};
#endif


void
_add_mod_type(PyObject *m, const char *name, PyTypeObject *type)
{
    Py_INCREF(type);
    PyModule_AddObject(m, name, (PyObject *)type);

    PyType_Ready(type);
}


/*
 * Python version-agnostic method to initialize C io module.
 * Invoked in different ways depending on whether this module
 * is compiled for Python 2.6 or 3.x.
*/
PyObject *_init_cio(PyObject *m)
{
    // Add classes to module
    _add_mod_type(m, "AWDWriter", &cio_AWDWriterType);

    return m;
}



#if PYTHON_VERSION == 3
PyMODINIT_FUNC
PyInit_cio(void)
{
    PyObject *m;

    m = PyModule_Create(&cio_mod);
    if (m == NULL)
        return NULL;

    return _init_cio(m);
}
#else // Python 2.6
PyMODINIT_FUNC
initcio(void)
{
    PyObject *m;

    m = Py_InitModule("cio", cio_methods);

    _init_cio(m);
}
#endif


