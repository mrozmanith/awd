#ifndef _LIBAWD_ATTR_H
#define _LIBAWD_ATTR_H

#include "ns.h"
#include "awd_types.h"

#define ATTR_RETURN_NULL AWD_attr_val_ptr _ptr; _ptr.str = NULL; return _ptr;

typedef awd_uint16 awd_propkey;

typedef enum {
    // Numeric types
    AWD_ATTR_INT8=1,
    AWD_ATTR_INT16,
    AWD_ATTR_INT32,
    AWD_ATTR_UINT8,
    AWD_ATTR_UINT16,
    AWD_ATTR_UINT32,
    AWD_ATTR_FLOAT32,
    AWD_ATTR_FLOAT64,

    // Derived numeric types
    AWD_ATTR_BOOL=21,
    AWD_ATTR_COLOR,
    AWD_ATTR_BADDR,

    // Aggregate/array types
    AWD_ATTR_STRING=31,
    AWD_ATTR_VECTOR2x1,
    AWD_ATTR_VECTOR3x1,
    AWD_ATTR_VECTOR4x1,
    AWD_ATTR_MTX3x2,
    AWD_ATTR_MTX3x3,
    AWD_ATTR_MTX4x3,
    AWD_ATTR_MTX4x4,
} AWD_attr_type;


typedef union {
    void *v;
    awd_bool *b;
    awd_int8 *i8;
    awd_int16 *i16;
    awd_int32 *i32;
    awd_uint8 *ui8;
    awd_uint16 *ui16;
    awd_uint32 *ui32;
    awd_float32 *f32;
    awd_float64 *f64;
    awd_float64 *mtx;
    awd_float64 *vec;
    awd_uint32 *col;
    awd_baddr *addr;
    char *str;
} AWD_attr_val_ptr;



class AWDAttr
{
    protected:
        AWD_attr_type type;
        AWD_attr_val_ptr value;
        awd_uint16 value_len;

        virtual void write_metadata(int)=0;

    public:
        void write_attr(int, bool, bool);

        void set_val(AWD_attr_val_ptr, awd_uint16, AWD_attr_type);
        AWD_attr_val_ptr get_val(awd_uint16 *, AWD_attr_type *);
        awd_uint16 get_val_len();
};


/**
 * User attributes.
*/
class AWDUserAttr : 
    public AWDAttr
{
    private:
        const char *key;
        awd_uint16 key_len;
        AWDNamespace *ns;
        
    protected:
        void write_metadata(int);

    public:
        AWDUserAttr *next;

        AWDUserAttr(AWDNamespace *, const char*, awd_uint16);
        ~AWDUserAttr();

        AWDNamespace *get_ns();
        const char *get_key();
        awd_uint16 get_key_len();
};


class AWDUserAttrList {
    private:
        AWDUserAttr *first_attr;
        AWDUserAttr *last_attr;

        AWDUserAttr *find(AWDNamespace *, const char *, awd_uint16);

    public:
        AWDUserAttrList();
        ~AWDUserAttrList();

        awd_uint32 calc_length(bool, bool);
        void write_attributes(int, bool, bool);

        AWD_attr_val_ptr get_val_ptr(AWDNamespace *ns, const char *, awd_uint16);
        bool get(AWDNamespace *, const char *, awd_uint16, AWD_attr_val_ptr *, awd_uint16 *, AWD_attr_type *);
        void set(AWDNamespace *, const char *, awd_uint16, AWD_attr_val_ptr, awd_uint16, AWD_attr_type);

        //void add_namespaces(AWD *);
};



/**
 * Numeric attributes ("properties")
*/
class AWDNumAttr : 
    public AWDAttr
{
    protected:
        void write_metadata(int);

    public:
        awd_propkey key;

        AWDNumAttr *next;

        AWDNumAttr();
};


class AWDNumAttrList {
    private:
        AWDNumAttr *first_attr;
        AWDNumAttr *last_attr;

        AWDNumAttr *find(awd_propkey);

    public:
        AWDNumAttrList();
        ~AWDNumAttrList();
        awd_uint32 calc_length(bool, bool);
        void write_attributes(int, bool, bool);

        AWD_attr_val_ptr get_val_ptr(awd_propkey);
        bool get(awd_propkey, AWD_attr_val_ptr *, awd_uint16 *, AWD_attr_type *);
        void set(awd_propkey, AWD_attr_val_ptr, awd_uint16, AWD_attr_type);
};




/**
 * "Attribute element", any element (such as a block, or parts of block
 * like skeleton joints) which can have attributes.
*/
class AWDAttrElement
{
    protected:
        AWDAttrElement();
        ~AWDAttrElement();

        AWDNumAttrList *properties;
        AWDUserAttrList *user_attributes;

        awd_uint32 calc_attr_length(bool, bool, bool, bool);

    public:
        bool get_attr(AWDNamespace *, const char *, awd_uint16, AWD_attr_val_ptr *, awd_uint16 *, AWD_attr_type *);
        void set_attr(AWDNamespace *, const char *, awd_uint16, AWD_attr_val_ptr, awd_uint16, AWD_attr_type);
};

#endif
