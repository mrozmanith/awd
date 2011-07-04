#ifndef _LIBAWD_AWD_TYPES_H
#define _LIBAWD_AWD_TYPES_H


// POD types
typedef char awd_int8;
typedef short awd_int16;
typedef int awd_int32;

typedef unsigned char awd_uint8;
typedef unsigned short awd_uint16;
typedef unsigned int awd_uint32;

typedef float awd_float32;
typedef double awd_float64;

typedef unsigned int awd_baddr;
typedef unsigned char awd_nsid;

typedef unsigned char awd_bool;


#define AWD_TRUE 1
#define AWD_FALSE 0
#define AWD_NULL 0



// Byte order conversions
#define UI16(x) awdutil_swapui16(x)
#define UI32(x) awdutil_swapui32(x)
#define F32(x)  awdutil_swapf32(x)
#define F64(x)  awdutil_swapf64(x)


typedef enum {
    NULL_REF=0,

    // Geometry/data
    MESH_DATA=1,
    PATH_DATA=2,

    // Scene objects
    SCENE=21,
    CONTAINER=22,
    PRIMITIVE=23,
    MESH_INSTANCE=24,
    PATH_INSTANCE=25,

    LIGHT=41,
    CAMERA=42,
    SOUND_SOURCE=43,

    // Partitioning
    BSP_TREE=61,
    OCT_TREE=62,

    // Materials
    SIMPLE_MATERIAL=81,
    TEXTURE=82,

    // Animation
    SKELETON=101,
    SKELETON_POSE=102,
    SKELETON_ANIM=103,
    UV_ANIM=121,

    // Misc
    COMMAND=254,
    NAMESPACE=255

} AWD_block_type;



typedef enum {
    UNCOMPRESSED,
    DEFLATE,
    LZMA
} AWD_compression;


#endif
