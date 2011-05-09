#ifndef LIBAWD_TEXTURE_H
#define LIBAWD_TEXTURE_H

#include "name.h"
#include "block.h"
#include "attr.h"

class AWDTexture : public AWDBlock, public AWDNamedElement, public AWDAttrElement
{
    private:
        awd_uint8 type;
        const char *url;
        awd_uint16 url_len;
    
    protected:
        awd_uint32 calc_body_length(awd_bool);
        void prepare_write();
        void write_body(int, awd_bool);

    public:
        AWDTexture(awd_uint8, const char *, awd_uint16);

        const char *get_url();
        awd_uint16 get_url_length();
        void set_url(const char *, awd_uint16);
};

#endif
