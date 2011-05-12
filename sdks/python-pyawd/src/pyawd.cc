#include <Python.h>
#include <awd/libawd.h>

#include "AWD.h"
#include "AWDMatrix4.h"
#include "AWDMatrix2x3.h"
#include "AWDAttrBlock.h"
#include "AWDSubMesh.h"
#include "AWDMeshData.h"
#include "AWDMeshInst.h"
#include "AWDSkeleton.h"
#include "AWDSkeletonPose.h"
#include "AWDSkeletonJoint.h"
#include "AWDSkeletonAnimation.h"
#include "AWDUVAnimation.h"
#include "AWDTexture.h"
#include "AWDMaterial.h"


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


void _add_int_const(PyTypeObject *type, const char *attr, int val)
{
    PyObject *pyval;
    
    pyval = PyInt_FromLong(val);
    Py_INCREF(pyval);
    PyDict_SetItemString(type->tp_dict, attr, pyval);
}


void
_add_mod_type(PyObject *m, const char *name, PyTypeObject *type)
{
    Py_INCREF(type);
    PyModule_AddObject(m, name, (PyObject *)type);
}


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

    // Prepare class data types
    if ((PyType_Ready(&pyawd_AWDType) < 0)
        || (PyType_Ready(&pyawd_AWDMatrix4Type) < 0)
        || (PyType_Ready(&pyawd_AWDMatrix2x3Type) < 0)
        || (PyType_Ready(&pyawd_AWDSkeletonPoseType) < 0)
        || (PyType_Ready(&pyawd_AWDSkeletonAnimationType) < 0)
        || (PyType_Ready(&pyawd_AWDUVAnimationType) < 0)
        || (PyType_Ready(&pyawd_AWDSkeletonJointType) < 0)
        || (PyType_Ready(&pyawd_AWDSkeletonType) < 0)
        || (PyType_Ready(&pyawd_AWDAttrBlockType) < 0)
        || (PyType_Ready(&pyawd_AWDMeshDataType) < 0)
        || (PyType_Ready(&pyawd_AWDMeshInstType) < 0)
        || (PyType_Ready(&pyawd_AWDSubMeshType) < 0)
        || (PyType_Ready(&pyawd_AWDTextureType) < 0)
        || (PyType_Ready(&pyawd_AWDMaterialType) > 0))
        return NULL;

    // AWD type constants
    _add_int_const(&pyawd_AWDType, "UNCOMPRESSED", UNCOMPRESSED);
    _add_int_const(&pyawd_AWDType, "DEFLATE", DEFLATE);
    _add_int_const(&pyawd_AWDType, "LZMA", LZMA);

    // SubMesh type constants
    _add_int_const(&pyawd_AWDSubMeshType, "VERTICES", VERTICES);
    _add_int_const(&pyawd_AWDSubMeshType, "TRIANGLES", TRIANGLES);
    _add_int_const(&pyawd_AWDSubMeshType, "UVS", UVS);
    _add_int_const(&pyawd_AWDSubMeshType, "VERTEX_NORMALS", VERTEX_NORMALS);
    _add_int_const(&pyawd_AWDSubMeshType, "VERTEX_TANGENTS", VERTEX_TANGENTS);
    _add_int_const(&pyawd_AWDSubMeshType, "FACE_NORMALS", FACE_NORMALS);
    _add_int_const(&pyawd_AWDSubMeshType, "JOINT_INDICES", JOINT_INDICES);
    _add_int_const(&pyawd_AWDSubMeshType, "VERTEX_WEIGHTS", VERTEX_WEIGHTS);

    // TODO: Get these from libawd
    // Texture and material type constants
    _add_int_const(&pyawd_AWDTextureType, "EXTERNAL", 0);
    _add_int_const(&pyawd_AWDMaterialType, "BITMAP", 2);


    // Add classes to module
    _add_mod_type(m, "AWDMatrix4", &pyawd_AWDMatrix4Type);
    _add_mod_type(m, "AWDMatrix2x3", &pyawd_AWDMatrix2x3Type);
    _add_mod_type(m, "AWD", &pyawd_AWDType);
    _add_mod_type(m, "AWDAttrBlock", &pyawd_AWDAttrBlockType);
    _add_mod_type(m, "AWDMeshData", &pyawd_AWDMeshDataType);
    _add_mod_type(m, "AWDSubMesh", &pyawd_AWDSubMeshType);
    _add_mod_type(m, "AWDMeshInst", &pyawd_AWDMeshInstType);
    _add_mod_type(m, "AWDSkeleton", &pyawd_AWDSkeletonType);
    _add_mod_type(m, "AWDSkeletonJoint", &pyawd_AWDSkeletonJointType);
    _add_mod_type(m, "AWDSkeletonPose", &pyawd_AWDSkeletonPoseType);
    _add_mod_type(m, "AWDSkeletonAnimation", &pyawd_AWDSkeletonAnimationType);
    _add_mod_type(m, "AWDUVAnimation", &pyawd_AWDUVAnimationType);
    _add_mod_type(m, "AWDTexture", &pyawd_AWDTextureType);
    _add_mod_type(m, "AWDMaterial", &pyawd_AWDMaterialType);

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


