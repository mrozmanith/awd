#ifndef _LIBAWD_MATERIAL_H
#define _LIBAWD_MATERIAL_H

#include "name.h"
#include "texture.h"
#include "block.h"
#include "attr.h"

#define PROP_MAT_COLOR 1
#define PROP_MAT_TEXTURE 2
#define PROP_MAT_TRANSPARENT 11
#define PROP_MAT_REPEAT 12


typedef enum {
    AWD_MATTYPE_COLOR=1,
    AWD_MATTYPE_BITMAP=2
} AWD_mat_type;

class AWDMaterial : public AWDBlock, public AWDNamedElement, public AWDAttrElement
{
    private:
        AWD_mat_type type;
        AWDTexture *texture;

    protected:
        awd_uint32 calc_body_length(bool, bool);
        void prepare_write();
        void write_body(int, bool, bool);

    public:
        AWDMaterial(AWD_mat_type, const char *, awd_uint16);
        ~AWDMaterial();

        bool repeat;
        bool transparent;

        void set_type(AWD_mat_type);
        AWD_mat_type get_type();

        void set_texture(AWDTexture *);
        AWDTexture *get_texture();
};

#endif
