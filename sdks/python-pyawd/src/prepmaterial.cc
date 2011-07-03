/**
 * This file contains reparation functions for all blocks belonging in the 
 * pyawd.material module. 
 *
 * These are invoked by AWDWriter to convert a python object to it's C++/libawd
 * equivalence and add it to the AWD document.
*/

#include "AWDWriter.h"
#include "bcache.h"
#include "util.h"


void
__prepare_material(PyObject *block, AWD *awd, pyawd_bcache *bcache)
{
    const char *name;
    int name_len;
    awd_uint8 type;
    PyObject *type_attr;
    PyObject *tex_attr;

    AWDSimpleMaterial *lawd_mat;

    type_attr = PyObject_GetAttrString(block, "type");
    type = (awd_uint8)PyLong_AsLong(type_attr);

    pyawdutil_get_strattr(block, "name", &name, &name_len);

    lawd_mat = new AWDSimpleMaterial(type, name, name_len);

    tex_attr = PyObject_GetAttrString(block, "texture");
    if (tex_attr != Py_None) {
        AWDTexture *tex;
        tex = (AWDTexture *)pyawd_bcache_get(bcache, tex_attr);
        if (tex)
            lawd_mat->set_texture(tex);
    }

    // Prep any user attributes
    __prepare_attr_element(block, awd, lawd_mat);

    awd->add_material(lawd_mat);
    pyawd_bcache_add(bcache, block, lawd_mat);
}

void
__prepare_texture(PyObject *block, AWD *awd, pyawd_bcache *bcache)
{
    const char *name;
    int name_len;
    AWD_tex_type type;
    PyObject *type_attr;

    AWDTexture *lawd_tex;

    type_attr = PyObject_GetAttrString(block, "type");
    type = (AWD_tex_type)PyLong_AsLong(type_attr);

    pyawdutil_get_strattr(block, "name", &name, &name_len);

    lawd_tex = new AWDTexture(type, name, name_len);

    if (type == EXTERNAL) {
        const char *url;
        int url_len;

        pyawdutil_get_strattr(block, "url", &url, &url_len);

        if (url) {
            lawd_tex->set_url(url, url_len);
        }
    }
    else {
        const char *buf;
        int buf_len;

        pyawdutil_get_strattr(block, "_AWDTexture__data", &buf, &buf_len);
        if (buf_len>0) {
            lawd_tex->set_embed_data((awd_uint8 *)buf, (awd_uint32)buf_len);
        }
        else {
            // TODO: Implement way to set errors (return null)
        }
    }

    // Prep any user attributes
    __prepare_attr_element(block, awd, lawd_tex);

    awd->add_texture(lawd_tex);
    pyawd_bcache_add(bcache, block, lawd_tex);
}

