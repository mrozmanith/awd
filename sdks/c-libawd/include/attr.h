#ifndef _ATTR_H
#define _ATTR_H

#include "awd_types.h"

typedef enum {
    INT32=1,
    UINT32,
    FLOAT64,
    BOOLEAN,
    STRING,
    VECTOR2x1=20,
    VECTOR3x1,
    VECTOR4x1,
    MATRIX3x3,
    MATRIX4x4
} AWD_attr_type;


struct AWD_attr_struct {
    char *key;
    AWD_attr_type type;
    void *value;

    struct AWD_attr_struct *next;
};
typedef struct AWD_attr_struct AWD_attr;

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
