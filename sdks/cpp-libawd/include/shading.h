#ifndef _LIBAWD_SHADING_H
#define _LIBAWD_SHADING_H

#include "texture.h"
#include "block.h"
#include "attr.h"

#define PROP_SHAD_STRENGTH 1
#define PROP_SHAD_COLOR 2
#define PROP_SHAD_ALPHA 3
#define PROP_SHAD_MAP 4

#define PROP_SHAD_ENVMAP_POSX 101
#define PROP_SHAD_ENVMAP_NEGX 102
#define PROP_SHAD_ENVMAP_POSY 103
#define PROP_SHAD_ENVMAP_NEGY 104
#define PROP_SHAD_ENVMAP_POSZ 105
#define PROP_SHAD_ENVMAP_NEGZ 106

#define PROP_SHAD_CEL_LEVELS 201
#define PROP_SHAD_CEL_SMOOTHNESS 202
#define PROP_SHAD_CEL_CUTOFF 203

#define PROP_SHAD_SCATTERING 301
#define PROP_SHAD_TRANSLUCENCY 302

#define PROP_SHAD_GLOSS 21
#define PROP_SHAD_GLOSS_MAP 22

#define PROP_SHAD_NORMAL_REFLECTANCE 401

#define PROP_SHAD_EPSILON 501
#define PROP_SHAD_STEP_SIZE 502

#define PROP_SHAD_COLOR_MATRIX 1001
#define PROP_SHAD_ALPHA_MULT 1101
#define PROP_SHAD_RED_MULT 1102
#define PROP_SHAD_GREEN_MULT 1103
#define PROP_SHAD_BLUE_MULT 1104
#define PROP_SHAD_COLOR_OFFS 1105

#define PROP_SHAD_FOG_DIST 1201

#define PROP_SHAD_PROJ_MODE 1301

#define PROP_SHAD_POWER 1401


typedef enum {
    AWD_SHADETYPE_BASIC_AMB=1,
    AWD_SHADETYPE_ENVMAP_AMB,

    AWD_SHADETYPE_BASIC_DIFF=101,
    AWD_SHADETYPE_ENVMAP_DIFF,
    AWD_SHADETYPE_CEL_DIFF,
    AWD_SHADETYPE_SSS_DIFF,
    AWD_SHADETYPE_TERRAIN_DIFF,

    AWD_SHADETYPE_BASIC_SPEC=201,
    AWD_SHADETYPE_CEL_SPEC,
    AWD_SHADETYPE_FRESNEL_SPEC,

    AWD_SHADETYPE_HARD_SHAD=301,
    AWD_SHADETYPE_SOFT_SHAD,
    AWD_SHADETYPE_FILTERED_SHAD,
    AWD_SHADETYPE_SLOW_FILTERED_SHAD,

    AWD_SHADETYPE_COLOR_MATRIX=401,
    AWD_SHADETYPE_COLOR_TRANSFORM,
    AWD_SHADETYPE_ENVMAP,
    AWD_SHADETYPE_FOG,
    AWD_SHADETYPE_PROJ_TEXTURE,
    AWD_SHADETYPE_RIMLIGHT
} AWD_shade_type;

class AWDShadingMethod :
    public AWDAttrElement
{
    protected:
        AWD_shade_type type;
        virtual void prepare_write()=0;

    public:
        awd_uint32 calc_method_length(bool, bool);
        void write_method(int, bool, bool);
};


class AWDBasicAmbientMethod :
    public AWDShadingMethod
{
    protected:
        void prepare_write();

    public:
        AWDBasicAmbientMethod();

        awd_float64 strength;
        awd_color color;
};

class AWDEnvMapAmbientMethod :
    public AWDBasicAmbientMethod
{
    protected:
        void prepare_write();

    public:
        AWDEnvMapAmbientMethod();

        AWDTexture *posx_texture;
        AWDTexture *negx_texture;
        AWDTexture *posy_texture;
        AWDTexture *negy_texture;
        AWDTexture *posz_texture;
        AWDTexture *negz_texture;
};

class AWDBasicDiffuseMethod :
    public AWDShadingMethod
{
    protected:
        void prepare_write();

    public:
        AWDBasicDiffuseMethod();

        awd_color color;
        awd_float64 alpha;
        AWDTexture *diffuse_map;
};

class AWDCelDiffuseMethod :
    public AWDBasicDiffuseMethod
{
    protected:
        void prepare_write();

    public:
        AWDCelDiffuseMethod();

        awd_uint16 levels;
        awd_float64 smoothness;
};

class AWDEnvMapDiffuseMethod :
    public AWDBasicDiffuseMethod
{
    protected:
        void prepare_write();

    public:
        AWDEnvMapDiffuseMethod();

        AWDTexture *posx_texture;
        AWDTexture *negx_texture;
        AWDTexture *posy_texture;
        AWDTexture *negy_texture;
        AWDTexture *posz_texture;
        AWDTexture *negz_texture;
};

class AWDSSSDiffuseMethod :
    public AWDBasicDiffuseMethod
{
    protected:
        void prepare_write();

    public:
        AWDSSSDiffuseMethod();

        awd_float64 scattering;
        awd_float64 translucency;
};

class AWDTerrainDiffuseMethod :
    public AWDBasicDiffuseMethod
{
    public:
        AWDTerrainDiffuseMethod();
};



class AWDBasicSpecularMethod :
    public AWDShadingMethod
{
    protected:
        void prepare_write();

    public:
        AWDBasicSpecularMethod();

        awd_float64 strength;
        awd_color color;
        AWDTexture *specular_map;
        awd_float64 gloss;
        AWDTexture *gloss_map;
};

class AWDCelSpecularMethod :
    public AWDBasicSpecularMethod
{
    protected:
        void prepare_write();

    public:
        AWDCelSpecularMethod();
        awd_float64 smoothness;
        awd_float64 cutoff;
};

class AWDFresnelSpecularMethod :
    public AWDBasicSpecularMethod
{
    protected:
        void prepare_write();

    public:
        AWDFresnelSpecularMethod();
        awd_float64 normal_reflectance;
};



class AWDFilteredShadowMapMethod :
    public AWDShadingMethod
{
    protected:
        void prepare_write();

    public:
        AWDFilteredShadowMapMethod();

        awd_float64 epsilon;
        awd_float64 step_size;
};

class AWDHardShadowMapMethod :
    public AWDShadingMethod
{
    protected:
        void prepare_write();

    public:
        AWDHardShadowMapMethod();

        awd_float64 epsilon;
};

class AWDSoftShadowMapMethod :
    public AWDShadingMethod
{
    protected:
        void prepare_write();

    public:
        AWDSoftShadowMapMethod();

        awd_float64 epsilon;
        awd_float64 step_size;
};

class AWDSlowFilteredShadowMapMethod :
    public AWDShadingMethod
{
    protected:
        void prepare_write();

    public:
        AWDSlowFilteredShadowMapMethod();

        awd_float64 epsilon;
        awd_float64 step_size;
};




#endif
