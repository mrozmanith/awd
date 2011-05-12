#include <stdio.h>

#include <Python.h>
#include "structmember.h"

#include <awd/libawd.h>

#include "util.h"
#include "AWDWriter.h"



/**
 * free()
*/
void
io_AWDWriter_dealloc(io_AWDWriter *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}


/**
 * AWDWriter();
*/
PyObject *
io_AWDWriter_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    io_AWDWriter *self;

    self = (io_AWDWriter *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}


/**
 * AWDWriter.__init__();
*/
static int
io_AWDWriter_init(io_AWDWriter *self, PyObject *args, PyObject *kwds)
{
    return 0;
}





/**
 * AWDWriter.write(awd, file)
*/
PyObject *
io_AWDWriter_flush(io_AWDWriter *self, PyObject *args)
{
    AWD *lawd_awd;
    PyObject *awd_obj;
    PyObject *fobj;
    int fd;

#if PYTHON_VERSION == 3
    extern PyTypeObject PyIOBase_Type;
    if (!PyArg_ParseTuple(args, "OO!", &awd_obj, &PyIOBase_Type, &fobj))
        return NULL;
#else
    extern PyTypeObject PyFile_Type;
    if (!PyArg_ParseTuple(args, "OO!", &awd_obj, &PyFile_Type, &fobj))
        return NULL;
#endif

    fd = PyObject_AsFileDescriptor(fobj);

    lawd_awd = new AWD(UNCOMPRESSED,0);

    if (fd >= 0) {
        pyawd_bcache *bcache;

        bcache = (pyawd_bcache *)malloc(sizeof(pyawd_bcache));
        pyawd_bcache_init(bcache);

        __prepare_blocks(awd_obj, "mesh_data_blocks", lawd_awd, bcache, __prepare_mesh_data);
        __prepare_blocks(awd_obj, "mesh_inst_blocks", lawd_awd, bcache, __prepare_mesh_inst);

    /*
        int i;
        int len;

        // Poses
        len = PyList_Size(self->skelpose_blocks);
        for (i=0; i<len; i++) {
            int ii;
            char *name;
            int name_len;
            int num_jtf;
            io_AWDWriterSkeletonPose *pose;

            pose = (io_AWDWriterSkeletonPose *)PyList_GetItem(self->skelpose_blocks, i);
            name = PyString_AsString(pose->name);
            name_len = PyString_Size(pose->name);
            pose->ob_pose = new AWDSkeletonPose(name, name_len);
            self->ob_awd->add_skeleton_pose(pose->ob_pose);

            // TODO: Move this into pose or anywhere but here
            num_jtf = PyList_Size(pose->transforms);
            for (ii=0; ii<num_jtf; ii++) {
                PyObject *joint_tf;

                joint_tf = PyList_GetItem(pose->transforms, ii);
                if (joint_tf != Py_None) {
                    pose->ob_pose->set_next_transform(
                        ((io_AWDWriterMatrix4*)joint_tf)->raw_data);
                }
                else {
                    pose->ob_pose->set_next_transform(NULL);
                }
            }
        }

        // Skeleton animations
        len = PyList_Size(self->skelanim_blocks);
        for (i=0; i<len; i++) {
            int ii;
            char *name;
            int name_len;
            int num_frames;
            io_AWDWriterSkeletonAnimation *anim;

            anim = (io_AWDWriterSkeletonAnimation*)PyList_GetItem(self->skelanim_blocks, i);
            name = PyString_AsString(anim->name);
            name_len = PyString_Size(anim->name);
            num_frames = PyList_Size(anim->frames);

            anim->ob_anim = new AWDSkeletonAnimation(name, name_len, num_frames);
            self->ob_awd->add_skeleton_anim(anim->ob_anim);

            for (ii=0; ii<num_frames; ii++) {
                PyObject *pose;
                pose = PyList_GetItem(anim->frames, ii);
                anim->ob_anim->set_next_frame_pose(
                    ((io_AWDWriterSkeletonPose*)pose)->ob_pose);
            }
        }

        // Skeletons
        len = PyList_Size(self->skeleton_blocks);
        for (i=0; i<len; i++) {
            io_AWDWriterSkeleton *skel;
            skel = (io_AWDWriterSkeleton *)PyList_GetItem(self->skeleton_blocks, i);
            // TODO: Implement this paradigm of "prepping" instances on all types of blocks.
            io_AWDWriterSkeleton__prep(skel);
            self->ob_awd->add_skeleton(skel->ob_skeleton);
        }

        // Textures
        len = PyList_Size(self->texture_blocks);
        for (i=0; i<len; i++) {
            io_AWDWriterTexture *tex;
            tex = (io_AWDWriterTexture *)PyList_GetItem(self->texture_blocks, i);
            io_AWDWriterTexture__prep(tex);
            self->ob_awd->add_texture(tex->lawd_obj);
        }

        // Materials
        len = PyList_Size(self->material_blocks);
        for (i=0; i<len; i++) {
            io_AWDWriterMaterial *mat;
            mat = (io_AWDWriterMaterial *)PyList_GetItem(self->material_blocks, i);
            io_AWDWriterMaterial__prep(mat);
            self->ob_awd->add_material(mat->lawd_obj);
        }


        // UV Animations
        len = PyList_Size(self->uvanim_blocks);
        for (i=0; i<len; i++) {
            io_AWDWriterUVAnimation *uvanim;
            uvanim = (io_AWDWriterUVAnimation *)PyList_GetItem(self->uvanim_blocks, i);
            io_AWDWriterUVAnimation__prep(uvanim);
            self->ob_awd->add_uv_anim(uvanim->lawd_obj);
        }


        // Mesh data blocks
        len = PyList_Size(self->mesh_data_blocks);
        for (i=0; i<len; i++) {
        }

        // Mesh instances
        len = PyList_Size(self->mesh_inst_blocks);
        for (i=0; i<len; i++) {
            io_AWDWriterMeshInst *inst;
            inst = (io_AWDWriterMeshInst *)PyList_GetItem(self->mesh_inst_blocks, i);
            io_AWDWriterMeshInst__prep(inst);
            self->ob_awd->add_mesh_inst(inst->lawd_obj);
        }
        */

        // Write buffer
        lawd_awd->flush(fd);
        Py_RETURN_NONE;
    }
    else {
        return NULL;
    }
}





/**
 * Method dictionary
*/
PyMethodDef io_AWDWriter_methods[] = {
    { "write", (PyCFunction)io_AWDWriter_flush, METH_VARARGS,
        "Write everything in an AWD object to an output stream." },

    { NULL }
};




/**
 * Type object 
*/
PyTypeObject io_AWDWriterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyawd.AWDWriter",                      // tp_name
    sizeof(io_AWDWriter),                // tp_basicsize
    0,                                      // tp_itemsize
    (destructor)io_AWDWriter_dealloc,    // tp_dealloc
    0,                                      // tp_print
    0,                                      // tp_getattr
    0,                                      // tp_setattr
    0,                                      // tp_reserved
    0,                                      // tp_repr
    0,                                      // tp_as_number
    0,                                      // tp_as_sequence
    0,                                      // tp_as_mapping
    0,                                      // tp_hash 
    0,                                      // tp_call
    0,                                      // tp_str
    0,                                      // tp_getattro
    0,                                      // tp_setattro
    0,                                      // tp_as_buffer
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,                // tp_flags
    "AWD document object.",                 // tp_doc
    0,                                      // tp_traverse
    0,                                      // tp_clear
    0,                                      // tp_richcompare
    0,                                      // tp_weaklistoffset
    0,                                      // tp_iter
    0,                                      // tp_iternext
    io_AWDWriter_methods,                // tp_methods
    0,                                      // tp_members
    0,                                      // tp_getset
    0,                                      // tp_base
    0,                                      // tp_dict
    0,                                      // tp_descr_get
    0,                                      // tp_descr_set
    0,                                      // tp_dictoffset
    (initproc)io_AWDWriter_init,         // tp_init
    0,                                      // tp_alloc
    io_AWDWriter_new,                    // tp_new
};



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


void
__prepare_mesh_data(PyObject *block, AWD *awd, pyawd_bcache *bcache)
{
    int sub_i;
    int num_subs;
    AWDMeshData *lawd_md;
    char *name;
    int name_len;
    PyObject *name_attr;
    PyObject *subs_list;

    name_attr = PyObject_GetAttrString(block, "name");
    name = PyString_AsString(name_attr);
    name_len = PyString_Size(name_attr);

    lawd_md = new AWDMeshData(name, name_len);

    // Get list of sub-meshes from python-space attribute __sub_meshes
    subs_list = PyObject_GetAttrString(block, "_AWDMeshData__sub_meshes");
    num_subs = PyList_Size(subs_list);
    for (sub_i=0; sub_i<num_subs; sub_i++) {
        AWDSubMesh *lawd_sub;
        int str_i;
        int num_streams;
        PyObject *streams_list;
        PyObject *sub = PyList_GetItem(subs_list, sub_i);

        lawd_sub = new AWDSubMesh();

        // Get list of data streams from python-space attribute __data_streams
        streams_list = PyObject_GetAttrString(sub, "_AWDSubMesh__data_streams");
        num_streams = PyList_Size(streams_list);
        for (str_i=0; str_i<num_streams; str_i++) {
            int data_len;
            AWD_mesh_str_type str_type;
            AWD_str_ptr lawd_data;
            PyObject *type;
            PyObject *data;
            PyObject *str_tuple;
            
            // Data streams are simple tuples in python-space, where the first value 
            // is the stream type and the second is a list containing the stream data
            str_tuple = PyList_GetItem(streams_list, str_i);
            type = PyTuple_GetItem(str_tuple, 0);
            data = PyTuple_GetItem(str_tuple, 1);
            data_len = PyList_Size(data);

            // Read stream type and treat data differently depending on whether it
            // should be float or integer data.
            str_type = (AWD_mesh_str_type)PyInt_AsLong(type);
            if (str_type == TRIANGLES || str_type == JOINT_INDICES) {
                lawd_data.ui32 = pyawdutil_pylist_to_uint32(data, NULL, data_len);
            }
            else {
                lawd_data.f64 = pyawdutil_pylist_to_float64(data, NULL, data_len);
            }

            // Add stream to libawd sub-mesh
            lawd_sub->add_stream(str_type, lawd_data, data_len);
        }

        // Add sub-mesh to libawd mesh data
        lawd_md->add_sub_mesh(lawd_sub);
    }

    awd->add_mesh_data(lawd_md);

    // Add to block cache so other blocks can reference it
    pyawd_bcache_add(bcache, block, lawd_md);
}


void
__prepare_blocks(PyObject *pyawd_AWD, const char *list_attr, AWD *awd, pyawd_bcache *bcache, void (*prep_func)(PyObject*, AWD*, pyawd_bcache *))
{
    PyObject *list;

    list = PyObject_GetAttrString(pyawd_AWD, list_attr);
    if (list && PyObject_IsInstance(list, (PyObject *)&PyList_Type)) {
        int i;
        int len;

        len = PyList_Size(list);
        for (i=0; i<len; i++) {
            prep_func(PyList_GetItem(list, i), awd, bcache);
        }
    }
}

