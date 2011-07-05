#include "platform.h"
#include "light.h"



AWDLight::AWDLight(const char *name, awd_uint16 name_len, AWD_light_type type) :
    AWDSceneBlock(LIGHT, name, name_len, NULL)
{
    this->type = type;
}


void
AWDLight::prepare_write()
{
    // TODO: Set all properties
}


awd_uint32
AWDLight::calc_body_length(bool wide_geom, bool wide_mtx)
{
    // Length is a single awd_uint8 (light type) and attr/props length
    return 1 + this->calc_attr_length(true, true, wide_geom, wide_mtx);
}


void
AWDLight::write_body(int fd, bool wide_geom, bool wide_mtx)
{
    write(fd, &this->type, sizeof(awd_uint8));
    this->properties->write_attributes(fd, wide_geom, wide_mtx);
    this->user_attributes->write_attributes(fd, wide_geom, wide_mtx);
}

