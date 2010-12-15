#ifndef _AWD_TYPES_H
#define _AWD_TYPES_H


// POD types
typedef char awd_int8;
typedef short awd_int16;
typedef int awd_int32;

typedef unsigned char awd_uint8;
typedef unsigned short awd_uint16;
typedef unsigned int awd_uint32;

typedef float awd_float32;
typedef double awd_float64;

typedef unsigned char awd_bool;


// Byte order conversions
#define UI16(x) awdutil_swapui16(x)
#define UI32(x) awdutil_swapui32(x)
#define F32(x)  awdutil_swapf32(x)
#define F64(x)  awdutil_swapf64(x)


typedef enum {
    NULL_REF=0,

    // Scene graph
    SCENE=1,
    CONTAINER,
    MESH_INSTANCE,
    MESH_DATA,
    BSP_TREE,
    OCT_TREE,

    // Materials/lighting
    MATERIAL=40,
    TEXTURE,
    SHADER,
    LIGHT_SOURCE,

    // Animation
    ARMATURE=60,

    // Misc
    COMMAND=254

} AWD_block_type;



typedef enum {
    UNCOMPRESSED,
    DEFLATE,
    LZMA
} AWD_compression;


#endif
