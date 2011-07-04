#ifndef _LIBAWD_MATERIAL_H
#define _LIBAWD_MATERIAL_H

#include "name.h"
#include "texture.h"
#include "block.h"
#include "attr.h"

#define AWD_MATTYPE_BITMAP 2

#define PROP_MAT_TEXTURE 2


class AWDSimpleMaterial : public AWDBlock, public AWDNamedElement, public AWDAttrElement
{
    private:
        awd_uint8 type;
        AWDTexture *texture;

    protected:
        awd_uint32 calc_body_length(bool, bool);
        void prepare_write();
        void write_body(int, bool, bool);

    public:
        AWDSimpleMaterial(awd_uint8, const char *, awd_uint16);
        ~AWDSimpleMaterial();

        void set_type(awd_uint8);
        awd_uint8 get_type();

        void set_texture(AWDTexture *);
        AWDTexture *get_texture();
};

#endif
