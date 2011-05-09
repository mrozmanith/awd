#include "util.h"
#include "texture.h"

#ifdef WIN32
#include "awdw32.h"
#else
#include <unistd.h>
#endif

AWDTexture::AWDTexture(awd_uint8 type, const char *name, awd_uint16 name_len) :
    AWDBlock(TEXTURE),
    AWDNamedElement(name, name_len),
    AWDAttrElement()
{
    this->type = type;
    this->url = NULL;
    this->url_len = 0;
}


const char *
AWDTexture::get_url()
{
    return this->url;
}

awd_uint16
AWDTexture::get_url_length()
{
    return this->url_len;
}

void
AWDTexture::set_url(const char *url, awd_uint16 url_len)
{
    this->url = url;
    this->url_len = url_len;
}


awd_uint32
AWDTexture::calc_body_length(awd_bool wide)
{
    awd_uint32 len;

    len = 7;
    len += this->get_name_length();
    len += this->url_len;

    len += this->calc_attr_length(true, true);

    return len;
}


void 
AWDTexture::prepare_write()
{
    // Nothing to do here
}


void
AWDTexture::write_body(int fd, awd_bool wide)
{
    awd_uint32 data_len;

    awdutil_write_varstr(fd, this->get_name(), this->get_name_length());

    data_len = UI32(this->url_len);
    write(fd, &this->type, sizeof(awd_uint8));
    write(fd, &data_len, sizeof(awd_uint32));
    write(fd, this->url, this->url_len);

    this->properties->write_attributes(fd);
    this->user_attributes->write_attributes(fd);
}

