#include "shading.h"
#include "util.h"

#include "platform.h"

awd_uint32
AWDShadingMethod::calc_method_length(bool wide_geom, bool wide_mtx)
{
    return sizeof(awd_uint16) + this->calc_attr_length(true,true, wide_geom,wide_mtx);
}


void
AWDShadingMethod::write_method(int fd, bool wide_geom, bool wide_mtx)
{
    awd_uint16 type_be;

    type_be = UI16(this->type);
    write(fd, &type_be, sizeof(awd_uint16));
    this->properties->write_attributes(fd, wide_geom, wide_mtx);
    this->user_attributes->write_attributes(fd, wide_geom, wide_mtx);
}
