#include "util.h"
#include "texture.h"
#include <sys/stat.h>

#include "platform.h"

AWDTexture::AWDTexture(AWD_tex_type type, const char *name, awd_uint16 name_len) :
    AWDBlock(TEXTURE),
    AWDNamedElement(name, name_len),
    AWDAttrElement()
{
    this->type = type;
    this->url = NULL;
    this->url_len = 0;
    this->embed_data = NULL;
    this->embed_data_len = 0;
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
AWDTexture::calc_body_length(bool wide_geom, bool wide_mtx)
{
    awd_uint32 len;

    len = 7;
    len += this->get_name_length();

    if (this->type == EXTERNAL) {
        len += this->url_len;
    }
    else {
        len += this->embed_data_len;
    }

    len += this->calc_attr_length(true, true, wide_geom, wide_mtx);

    return len;
}


void
AWDTexture::set_embed_data(awd_uint8 *buf, awd_uint32 buf_len)
{
    this->embed_data = buf;
    this->embed_data_len = buf_len;
}


void
AWDTexture::set_embed_file_data(int fd)
{
    struct stat *s;

    s = (struct stat *)malloc(sizeof(struct stat));
    fstat(fd, s);
    this->embed_data_len = s->st_size;
    this->embed_data = (awd_uint8 *)malloc(this->embed_data_len);

    lseek(fd, 0, SEEK_SET);
    read(fd, this->embed_data, this->embed_data_len);
}


void 
AWDTexture::prepare_write()
{
    // Do nothing
}


void
AWDTexture::write_body(int fd, bool wide_geom, bool wide_mtx)
{
    awd_uint32 data_len;

    awdutil_write_varstr(fd, this->get_name(), this->get_name_length());

    write(fd, &this->type, sizeof(awd_uint8));
    if (this->type == EXTERNAL) {
        data_len = UI32(this->url_len);
        write(fd, &data_len, sizeof(awd_uint32));
        write(fd, this->url, this->url_len);
    }
    else {
        data_len = UI32(this->embed_data_len);
        write(fd, &data_len, sizeof(awd_uint32));
        write(fd, this->embed_data, this->embed_data_len);
    }

    this->properties->write_attributes(fd, wide_geom, wide_mtx);
    this->user_attributes->write_attributes(fd, wide_geom, wide_mtx);
}

