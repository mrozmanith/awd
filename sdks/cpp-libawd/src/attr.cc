#include <cstdio>
#include <stdlib.h>
#include <cstring>

#include "awd_types.h"
#include "attr.h"
#include "util.h"


void
AWDAttr::write_attr(int fd)
{
    AWD_attr_val_ptr val;
    awd_uint16 bytes_written;
    awd_int16 i16_be;
    awd_int32 i32_be;
    awd_float32 f32_be;
    awd_float64 f64_be;

    this->write_metadata(fd);

    bytes_written = 0;
    val.v = this->value.v;

    while (bytes_written < this->value_len) {
        // Check type, and write data accordingly
        switch (this->type) {
            case INT16:
                i16_be = UI16(*val.i16);
                write(fd, &i16_be, sizeof(awd_int16));
                bytes_written += sizeof(awd_int16);
                val.i16++;
                break;

            case BADDR:
            case INT32:
                i32_be = UI32(*val.i32);
                write(fd, &i32_be, sizeof(awd_int32));
                bytes_written += sizeof(awd_int32);
                val.i32++;
                break;

            case FLOAT32:
                f32_be = F32(*val.f32);
                write(fd, &f32_be, sizeof(awd_float32));
                bytes_written += sizeof(awd_float32);
                val.f32++;
                break;

            case FLOAT64:
                f64_be = F64(*val.f64);
                write(fd, &f64_be, sizeof(awd_float64));
                bytes_written += sizeof(awd_float64);
                val.f64++;
                break;

            case STRING:
                // Write entire string in one go
                write(fd, val.str, this->value_len);
                bytes_written += this->value_len;
                break;

            default:
                printf("unknown type: %d\n", this->type);
                return;
        }
    }
}





AWDUserAttr::AWDUserAttr()
{
    this->next = NULL;
}


void
AWDUserAttr::write_metadata(int fd)
{
    awd_uint8 type;
    awd_uint16 len_be;

    len_be = UI16(this->value_len);
    type = (awd_uint8)this->type;

    write(fd, &this->ns_addr, sizeof(awd_uint8));
    awdutil_write_varstr(fd, this->key, this->key_len);
    write(fd, &type, sizeof(awd_uint8));
    write(fd, &len_be, sizeof(awd_uint16));
}


AWDUserAttrList::AWDUserAttrList()
{
    this->first_attr = NULL;
    this->last_attr = NULL;
}


awd_uint32
AWDUserAttrList::calc_length()
{
    awd_uint32 len;
    AWDUserAttr *cur;

    // List length field always included
    len = sizeof(awd_uint32); 

    // Accumulate size of individual attributes
    cur = this->first_attr;
    while (cur) {
        // Meta-data takes up 6 bytes
        len += (6 + cur->key_len + cur->value_len);
        cur = cur->next;
    }

    return len;
}


void
AWDUserAttrList::write_attributes(int fd)
{
    awd_uint32 len_be;
    AWDUserAttr *cur;

    len_be = UI32(this->calc_length() - sizeof(awd_uint32));
    write(fd, &len_be, sizeof(awd_uint32));

    cur = this->first_attr;
    while (cur) {
        cur->write_attr(fd);
        cur = cur->next;
    }
}


AWDUserAttr *
AWDUserAttrList::find(char *key, awd_uint16 key_len)
{
    if (this->first_attr) {
        AWDUserAttr *cur;
        cur = this->first_attr;
        while (cur) {
            if (key_len == cur->key_len) {
                if (strncmp(key, cur->key, key_len)==0)
                    return cur;
            }

            cur = cur->next;
        }
    }

    return NULL;
}


AWD_attr_val_ptr
AWDUserAttrList::get(char *key, awd_uint16 key_len)
{
    AWDUserAttr *attr;

    attr = this->find(key, key_len);
    if (attr) 
        return attr->value;

    // Return null if reached
    ATTR_RETURN_NULL
}


void
AWDUserAttrList::set(char *key, awd_uint16 key_len, AWD_attr_val_ptr value, awd_uint16 value_length, AWD_attr_type type)
{
    bool created;
    AWDUserAttr *attr;

    attr = this->find(key, key_len);
    if (attr == NULL) {
        attr = new AWDUserAttr();
        attr->key = (char *)malloc(key_len);
        attr->key_len = key_len;
        strncpy(attr->key, key, key_len);

        created = true;
    }

    attr->type = type;
    attr->value = value;
    attr->value_len = value_length;

    // Add to internal list if the attribute wasn't
    // originally found there.
    if (created) {
        if (!this->first_attr) {
            this->first_attr = attr;
        }
        else {
            this->last_attr->next = attr;
        }

        this->last_attr = attr;
        this->last_attr->next = NULL;
    }
}








AWDNumAttr::AWDNumAttr()
{
    this->next = NULL;
}


void
AWDNumAttr::write_metadata(int fd)
{
    awd_uint16 key_be;
    awd_uint16 len_be;

    key_be = UI16(this->key);
    len_be = UI16(this->value_len);

    write(fd, &key_be, sizeof(awd_uint16));
    write(fd, &len_be, sizeof(awd_uint16));
}



AWDNumAttrList::AWDNumAttrList()
{
    this->first_attr = NULL;
    this->last_attr = NULL;
}


awd_uint32
AWDNumAttrList::calc_length()
{
    awd_uint32 len;
    AWDNumAttr *cur;

    // Attr list length always included
    len = sizeof(awd_uint32);

    cur = this->first_attr;
    while (cur) {
        // Meta-data is always four bytes
        len += (4 + cur->value_len);
        cur = cur->next;
    }

    return len;
}


void
AWDNumAttrList::write_attributes(int fd)
{
    awd_uint32 len_be;
    AWDNumAttr *cur;

    len_be = UI32(this->calc_length() - sizeof(awd_uint32));
    write(fd, &len_be, sizeof(awd_uint32));

    cur = this->first_attr;
    while (cur) {
        cur->write_attr(fd);
        cur = cur->next;
    }
}



AWDNumAttr *
AWDNumAttrList::find(awd_propkey key)
{
    if (this->first_attr) {
        AWDNumAttr *cur;

        cur = this->first_attr;
        while (cur) {
            if (cur->key == key)
                return cur;
        }
    }

    return NULL;
}


AWD_attr_val_ptr
AWDNumAttrList::get(awd_propkey key)
{
    AWDNumAttr *attr;

    attr = this->find(key);
    if (attr) 
        return attr->value;

    // Return null if reached
    ATTR_RETURN_NULL
}


void
AWDNumAttrList::set(awd_propkey key, AWD_attr_val_ptr value, awd_uint16 value_length, AWD_attr_type type)
{
    bool existed;
    AWDNumAttr *attr;    

    existed = false;
    attr = this->find(key);
    if (!attr) {
        attr = new AWDNumAttr();
        attr->key = key;
    }

    attr->type = type;
    attr->value = value;
    attr->value_len = value_length;

    // Add to internal list if the attribute wasn't
    // originally found there.
    if (!existed) {
        if (!this->first_attr) {
            this->first_attr = attr;
        }
        else {
            this->last_attr->next = attr;
        }

        this->last_attr = attr;
        this->last_attr->next = NULL;
    }
}





AWDAttrElement::AWDAttrElement()
{
    this->properties = new AWDNumAttrList();
    this->user_attributes = new AWDUserAttrList();
}


awd_uint32 
AWDAttrElement::calc_attr_length(bool with_props, bool with_user_attr)
{
    awd_uint32 len;

    len = 0;
    if (with_props) len += this->properties->calc_length();
    if (with_user_attr) len += this->user_attributes->calc_length();

    return len;
}


void
AWDAttrElement::write_attributes(int fd, bool with_props, bool with_user_attr)
{
    if (with_props) this->properties->write_attributes(fd);
    if (with_user_attr) this->user_attributes->write_attributes(fd);
}

