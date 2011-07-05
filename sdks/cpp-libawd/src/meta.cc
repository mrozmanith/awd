#include <cstdio>
#include <string.h>

#include "meta.h"
#include "awd.h"

AWDMetaData::AWDMetaData() :
    AWDBlock(METADATA),
    AWDAttrElement()
{
    this->encoder_name = (char *)malloc(6);
    memcpy(this->encoder_name, "libawd", 6);

    this->encoder_version = (char *)malloc(255);
    snprintf(this->encoder_version, 255, "%d.%d.%d%c",
        AWD_VERSION_MAJOR, AWD_VERSION_MINOR, AWD_VERSION_BUILD, AWD_VERSION_RELEASE);
}


void
AWDMetaData::override_encoder_metadata(char *name, char *version)
{
    this->encoder_name = name;
    this->encoder_version = version;
}

void
AWDMetaData::prepare_write()
{
    AWD_attr_val_ptr val;

    val.str = this->encoder_name;
    this->properties->set(PROP_META_ENCODER_NAME, val, strlen(this->encoder_name), AWD_ATTR_STRING);
    
    val.str = this->encoder_version;
    this->properties->set(PROP_META_ENCODER_VER, val, strlen(this->encoder_version), AWD_ATTR_STRING);
}


awd_uint32
AWDMetaData::calc_body_length(bool wide_geom, bool wide_mtx)
{
    return this->calc_attr_length(true, false, wide_geom, wide_mtx);
}


void
AWDMetaData::write_body(int fd, bool wide_geom, bool wide_mtx)
{
    this->properties->write_attributes(fd, wide_geom, wide_mtx);
}

