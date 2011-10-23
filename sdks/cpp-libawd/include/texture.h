#ifndef LIBAWD_TEXTURE_H
#define LIBAWD_TEXTURE_H

#include "awd_types.h"
#include "block.h"
#include "name.h"
#include "attr.h"


typedef enum {
    EXTERNAL=0,
    EMBEDDED_JPEG,
    EMBEDDED_PNG
} AWD_tex_type;

class AWDTexture : 
    public AWDBlock, 
    public AWDNamedElement, 
    public AWDAttrElement
{
    private:
        AWD_tex_type type;
        const char *url;
        awd_uint16 url_len;
        awd_uint8 *embed_data;
        awd_uint32 embed_data_len;
    
    protected:
        awd_uint32 calc_body_length(bool, bool);
        void prepare_write();
        void write_body(int, bool, bool);

    public:
        AWDTexture(AWD_tex_type, const char *, awd_uint16);

        void set_embed_data(awd_uint8 *, awd_uint32);
        void set_embed_file_data(int);

        const char *get_url();
        awd_uint16 get_url_length();
        void set_url(const char *, awd_uint16);
};

#endif
