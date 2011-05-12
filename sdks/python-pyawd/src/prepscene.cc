/**
 * This file contains reparation functions for all blocks belonging in the pyawd.scene 
 * module. 
 * 
 * These are invoked by AWDWriter to convert a python object to it's C++/libawd
 * equivalence and add it to the AWD document.
*/


#include "AWDWriter.h"
#include "bcache.h"
#include "util.h"

void
__prepare_mesh_inst(PyObject *block, AWD *awd, pyawd_bcache *bcache)
{
    int i;
    char *name;
    int name_len;
    awd_float64 *mtx;
    int num_materials;

    PyObject *name_attr;
    PyObject *mtx_attr;
    PyObject *md_attr;
    PyObject *mat_attr;

    AWDMeshInst *lawd_inst;
    AWDMeshData *lawd_md;
    
    mtx_attr = PyObject_GetAttrString(block, "transform");
    if (mtx_attr != Py_None) {
        PyObject *mtx_list;
        mtx_list = PyObject_GetAttrString(mtx_attr, "raw_data");
        mtx = pyawdutil_pylist_to_float64(mtx_list, NULL, 16);
    }
    
    lawd_md = NULL;
    md_attr = PyObject_GetAttrString(block, "mesh_data");
    if (md_attr != Py_None) {
        lawd_md = (AWDMeshData *)pyawd_bcache_get(bcache, md_attr);
    }
    
    name_attr = PyObject_GetAttrString(block, "name");
    name = PyString_AsString(name_attr);
    name_len = PyString_Size(name_attr);
    
    lawd_inst = new AWDMeshInst(name, name_len, lawd_md, mtx);

    mat_attr = PyObject_GetAttrString(block, "materials");
    num_materials = PyList_Size(mat_attr);
    for (i=0; i<num_materials; i++) {
        AWDSimpleMaterial *lawd_mat;

        lawd_mat = (AWDSimpleMaterial *)pyawd_bcache_get(bcache, PyList_GetItem(mat_attr, i));
        lawd_inst->add_material(lawd_mat);
    }


    awd->add_mesh_inst(lawd_inst);
    pyawd_bcache_add(bcache, block, lawd_inst);
}

