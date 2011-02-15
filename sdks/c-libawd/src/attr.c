#include <stdlib.h>

#include "libawd.h"



AWD_attr_list *
awd_create_attr_list()
{
    AWD_attr_list *list;

    list = malloc(sizeof(AWD_attr_list));
    list->first_attr = NULL;

    return list;
}


void 
awd_set_attr_int32(AWD_attr_list *list, const char *key, awd_int32 val)
{
    AWD_attr *attr = _awd_create_attr(key, AWD_ATTR_INT32, &val);
    _awd_append_attr(list, attr);
}


void 
awd_set_attr_uint32(AWD_attr_list *list, const char *key, awd_uint32 val)
{
    AWD_attr *attr = _awd_create_attr(key, AWD_ATTR_UINT32, &val);
    _awd_append_attr(list, attr);
}


void 
awd_set_attr_float64(AWD_attr_list *list, const char *key, awd_float64 val)
{
    AWD_attr *attr = _awd_create_attr(key, AWD_ATTR_FLOAT64, &val);
    _awd_append_attr(list, attr);
}


void 
awd_set_attr_bool(AWD_attr_list *list, const char *key, awd_bool val)
{
    AWD_attr *attr = _awd_create_attr(key, AWD_ATTR_BOOLEAN, &val);
    _awd_append_attr(list, attr);
}


void 
awd_set_attr_str(AWD_attr_list *list, const char *key, char *val)
{
    AWD_attr *attr = _awd_create_attr(key, AWD_ATTR_STRING, val);
    _awd_append_attr(list, attr);
}


void 
awd_set_attr_vec2x1(AWD_attr_list *list, const char *key, awd_float64 *val)
{
    AWD_attr *attr = _awd_create_attr(key, AWD_ATTR_VECTOR2x1, val);
    _awd_append_attr(list, attr);
}


void 
awd_set_attr_vec3x1(AWD_attr_list *list, const char *key, awd_float64 *val)
{
    AWD_attr *attr = _awd_create_attr(key, AWD_ATTR_VECTOR3x1, val);
    _awd_append_attr(list, attr);
}


void 
awd_set_attr_vec4x1(AWD_attr_list *list, const char *key, awd_float64 *val)
{
    AWD_attr *attr = _awd_create_attr(key, AWD_ATTR_VECTOR4x1, val);
    _awd_append_attr(list, attr);
}


void 
awd_set_attr_mtx3x3(AWD_attr_list *list, const char *key, awd_float64 *val)
{
    AWD_attr *attr = _awd_create_attr(key, AWD_ATTR_MATRIX3x3, val);
    _awd_append_attr(list, attr);
}


void 
awd_set_attr_mtx4x4(AWD_attr_list *list, const char *key, awd_float64 *val)
{
    AWD_attr *attr = _awd_create_attr(key, AWD_ATTR_MATRIX4x4, val);
    _awd_append_attr(list, attr);
}


AWD_attr *
_awd_create_attr(const char *key, AWD_attr_type type, void *val)
{
    AWD_attr *attr;

    attr = malloc(sizeof(AWD_attr));
    attr->key = (char *)key;
    attr->type = type;
    attr->value = val;
    return attr;
}


void 
_awd_append_attr(AWD_attr_list *list, AWD_attr *attr)
{
    if (list->first_attr == NULL) {
        list->first_attr = attr;
    }
    else {
        AWD_attr *next;

        next = list->first_attr;
        while (next->next != NULL) {
            next = next->next;
        }

        next->next = attr;
    }
}

