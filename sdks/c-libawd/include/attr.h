#ifndef _ATTR_H
#define _ATTR_H

#include "awd_types.h"

typedef enum {
    AWD_ATTR_INT32=1,
    AWD_ATTR_UINT32,
    AWD_ATTR_FLOAT64,
    AWD_ATTR_BOOLEAN,
    AWD_ATTR_STRING,
    AWD_ATTR_VECTOR2x1=20,
    AWD_ATTR_VECTOR3x1,
    AWD_ATTR_VECTOR4x1,
    AWD_ATTR_MATRIX3x3,
    AWD_ATTR_MATRIX4x4
} AWD_attr_type;


typedef struct _AWD_attr {
    char *key;
    AWD_attr_type type;
    void *value;

    struct _AWD_attr *next;
} AWD_attr;

typedef struct {
    AWD_attr *first_attr;
} AWD_attr_list;



AWD_attr_list   *awd_create_attr_list(void);

void            awd_set_attr_int32(AWD_attr_list *, const char *, awd_int32);
void            awd_set_attr_uint32(AWD_attr_list *, const char *, awd_uint32);
void            awd_set_attr_float64(AWD_attr_list *, const char *, awd_float64);
void            awd_set_attr_bool(AWD_attr_list *, const char *, awd_bool);
void            awd_set_attr_str(AWD_attr_list *, const char *, char *);
void            awd_set_attr_vec2x1(AWD_attr_list *, const char *, awd_float64 *);
void            awd_set_attr_vec3x1(AWD_attr_list *, const char *, awd_float64 *);
void            awd_set_attr_vec4x1(AWD_attr_list *, const char *, awd_float64 *);
void            awd_set_attr_vec3x3(AWD_attr_list *, const char *, awd_float64 *);
void            awd_set_attr_vec4x4(AWD_attr_list *, const char *, awd_float64 *);


AWD_attr        *_awd_create_attr(const char *, AWD_attr_type, void *);
void            _awd_append_attr(AWD_attr_list *, AWD_attr *);



#endif
