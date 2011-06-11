#include "material.h"
#include "util.h"

#ifdef WIN32
#include "awdw32.h"
#else
#include <unistd.h>
#endif

AWDSimpleMaterial::AWDSimpleMaterial(awd_uint8 type, const char *name, awd_uint16 name_len) : 
    AWDBlock(SIMPLE_MATERIAL),
    AWDNamedElement(name, name_len),
    AWDAttrElement()
{
    this->type = type;
    this->texture = NULL;
}

AWDSimpleMaterial::~AWDSimpleMaterial()
{
    // Texture will be deleted 
    // by the block list it's in
    this->texture = NULL;
}


awd_uint8
AWDSimpleMaterial::get_type()
{
    return this->type;
}

void
AWDSimpleMaterial::set_type(awd_uint8 type)
{
    this->type = type;
}



AWDTexture *
AWDSimpleMaterial::get_texture()
{
    return this->texture;
}

void
AWDSimpleMaterial::set_texture(AWDTexture *texture)
{
    this->texture = texture;
}



awd_uint32
AWDSimpleMaterial::calc_body_length(bool wide_geom, bool wide_mtx)
{
    return 3 + this->get_name_length() + this->calc_attr_length(true,true, wide_geom, wide_mtx);
}


void
AWDSimpleMaterial::prepare_write()
{
    if (this->texture) {
        AWD_attr_val_ptr tex_val;
        tex_val.addr = (awd_baddr *)malloc(sizeof(awd_baddr));
        *tex_val.addr = this->texture->get_addr();
        this->properties->set(PROP_MAT_TEXTURE, tex_val, sizeof(awd_baddr), AWD_ATTR_BADDR);
    }
}


void
AWDSimpleMaterial::write_body(int fd, bool wide_geom, bool wide_mtx)
{
    awdutil_write_varstr(fd, this->get_name(), this->get_name_length());
    // Write type
    write(fd, &this->type, sizeof(awd_uint8));

    this->properties->write_attributes(fd, wide_geom, wide_mtx);
    this->user_attributes->write_attributes(fd, wide_geom, wide_mtx);
}
