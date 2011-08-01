#include "platform.h"
#include "primitive.h"


AWDPrimitive::AWDPrimitive(const char *name, awd_uint16 name_len, AWD_primitive_type type) :
    AWDSceneBlock(PRIMITIVE, name, name_len, NULL)
{
    this->type = type;
}

AWDPrimitive::~AWDPrimitive()
{
}


void
AWDPrimitive::prepare_write()
{
}


awd_uint32
AWDPrimitive::calc_body_length(bool wide_geom, bool wide_mtx)
{
    return 1 + this->calc_common_length(wide_mtx) +
        this->calc_attr_length(true, true, wide_geom, wide_mtx);
}

void
AWDPrimitive::write_body(int fd, bool wide_geom, bool wide_mtx)
{
    this->write_scene_common(fd, wide_mtx);
    write(fd, &this->type, sizeof(awd_uint8));
    this->properties->write_attributes(fd, wide_geom, wide_mtx);
    this->user_attributes->write_attributes(fd, wide_geom, wide_mtx);
}


