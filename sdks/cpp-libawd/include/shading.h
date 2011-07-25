#ifndef _LIBAWD_SHADING_H
#define _LIBAWD_SHADING_H

#include "texture.h"
#include "block.h"
#include "attr.h"

class AWDShadingMethod :
    public AWDAttrElement
{
    private:
        // Type is defined by the sub-classes in their
        // implementations of prepare_write().
        awd_uint16 type;
        
    protected:
        virtual void prepare_write()=0;

    public:
        awd_uint32 calc_method_length(bool, bool);
        void write_method(int, bool, bool);
};


class AWDBasicAmbientMethod :
    public AWDShadingMethod
{
    public:
        awd_float64 strength;
        awd_color color;
};

class AWDEnvMapAmbientMethod :
    public AWDBasicAmbientMethod
{
    public:
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
    public:
        awd_color color;
        awd_float64 alpha;
        AWDTexture *diffuse_map;
};

class AWDCelDiffuseMethod :
    public AWDBasicDiffuseMethod
{
    public:
        awd_uint8 levels;
        awd_float64 smoothness;
};

class AWDEnvMapDiffuseMethod :
    public AWDBasicDiffuseMethod
{
    public:
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
    public:
        awd_color color;
        awd_float64 scattering;
        awd_float64 translucency;
};

class AWDTerrainDiffuseMethod :
    public AWDBasicDiffuseMethod
{
};



class AWDBasicSpecularMethod :
    public AWDShadingMethod
{
    public:
        awd_float64 strength;
        awd_color color;
        AWDTexture *specular_map;
        awd_float64 gloss;
        AWDTexture *gloss_map;
};

class AWDCelSpecularMethod :
    public AWDBasicSpecularMethod
{
    public:
        awd_float64 smoothness;
        awd_float64 cutoff;
};

class AWDFresnelSpecularMethod :
    public AWDBasicSpecularMethod
{
    public:
        awd_float64 normal_reflectance;
};



class AWDFilteredShadowMapMethod :
    public AWDShadingMethod
{
    public:
        awd_float64 epsilon;
        awd_float64 step_size;
};

class AWDHardShadowMapMethod :
    public AWDShadingMethod
{
    public:
        awd_float64 epsilon;
};

class AWDSoftShadowMapMethod :
    public AWDShadingMethod
{
    public:
        awd_float64 epsilon;
        awd_float64 step_size;
};

class AWDSlowFilteredShadowMapMethod :
    public AWDShadingMethod
{
    public:
        awd_float64 epsilon;
        awd_float64 step_size;
};




#endif
