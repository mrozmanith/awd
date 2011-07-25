#include "shading.h"
#include "util.h"

#include "platform.h"



inline void __set_tex_prop(AWDNumAttrList *properties, awd_uint16 prop, AWDTexture *tex)
{
    if (tex) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_baddr));
        *val.addr = tex->get_addr();
        properties->set(prop, val, sizeof(awd_baddr), AWD_FIELD_BADDR);
    }
}



awd_uint32
AWDShadingMethod::calc_method_length(bool wide_geom, bool wide_mtx)
{
    return sizeof(awd_uint16) + this->calc_attr_length(true,true, wide_geom,wide_mtx);
}


void
AWDShadingMethod::write_method(int fd, bool wide_geom, bool wide_mtx)
{
    awd_uint16 type_be;

    type_be = UI16(this->type);
    write(fd, &type_be, sizeof(awd_uint16));
    this->properties->write_attributes(fd, wide_geom, wide_mtx);
    this->user_attributes->write_attributes(fd, wide_geom, wide_mtx);
}




AWDBasicAmbientMethod::AWDBasicAmbientMethod()
{
    this->type = AWD_SHADETYPE_BASIC_AMB;
    this->strength = 0.0f;
    this->color = 0;
}

void
AWDBasicAmbientMethod::prepare_write()
{
    if (this->strength != 0.0f) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->strength;
        this->properties->set(PROP_SHAD_STRENGTH, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }
    
    if (this->color != 0) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_color));
        *val.col = this->color;
        this->properties->set(PROP_SHAD_COLOR, val, sizeof(awd_float64), AWD_FIELD_COLOR);
    }
}




AWDEnvMapAmbientMethod::AWDEnvMapAmbientMethod()
{
    this->type = AWD_SHADETYPE_ENVMAP_AMB;
    this->posx_texture = NULL;
    this->negx_texture = NULL;
    this->posy_texture = NULL;
    this->negy_texture = NULL;
    this->posz_texture = NULL;
    this->negz_texture = NULL;
}

void
AWDEnvMapAmbientMethod::prepare_write()
{
    AWDBasicAmbientMethod::prepare_write();

    __set_tex_prop(properties, PROP_SHAD_ENVMAP_POSX, this->posx_texture);
    __set_tex_prop(properties, PROP_SHAD_ENVMAP_NEGX, this->negx_texture);
    __set_tex_prop(properties, PROP_SHAD_ENVMAP_POSY, this->posy_texture);
    __set_tex_prop(properties, PROP_SHAD_ENVMAP_NEGY, this->negy_texture);
    __set_tex_prop(properties, PROP_SHAD_ENVMAP_POSZ, this->posz_texture);
    __set_tex_prop(properties, PROP_SHAD_ENVMAP_NEGZ, this->negz_texture);
}



AWDBasicDiffuseMethod::AWDBasicDiffuseMethod()
{
    this->type = AWD_SHADETYPE_BASIC_DIFF;
    this->color = 0;
    this->alpha = 1.0;
    this->diffuse_map = NULL;
}

void
AWDBasicDiffuseMethod::prepare_write()
{
    if (this->color != 0) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_color));
        *val.col = this->color;
        this->properties->set(PROP_SHAD_COLOR, val, sizeof(awd_color), AWD_FIELD_COLOR);
    }

    if (this->alpha != 1.0) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->alpha;
        this->properties->set(PROP_SHAD_ALPHA, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }

    __set_tex_prop(properties, PROP_SHAD_MAP, this->diffuse_map);
}




AWDCelDiffuseMethod::AWDCelDiffuseMethod()
{
    this->type = AWD_SHADETYPE_CEL_DIFF;
    this->levels = 3;
    this->smoothness = 0.1;
}

void
AWDCelDiffuseMethod::prepare_write()
{
    AWDBasicDiffuseMethod::prepare_write();

    if (this->levels != 3) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_uint16));
        *val.ui16 = this->levels;
        this->properties->set(PROP_SHAD_CEL_LEVELS, val, sizeof(awd_uint16), AWD_FIELD_UINT16);
    }

    if (this->smoothness != 0.1) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->smoothness;
        this->properties->set(PROP_SHAD_CEL_SMOOTHNESS, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }
}



AWDEnvMapDiffuseMethod::AWDEnvMapDiffuseMethod()
{
    this->type = AWD_SHADETYPE_ENVMAP_DIFF;
    this->posx_texture = NULL;
    this->negx_texture = NULL;
    this->posy_texture = NULL;
    this->negy_texture = NULL;
    this->posz_texture = NULL;
    this->negz_texture = NULL;
}

void
AWDEnvMapDiffuseMethod::prepare_write()
{
    AWDBasicDiffuseMethod::prepare_write();

    __set_tex_prop(properties, PROP_SHAD_ENVMAP_POSX, this->posx_texture);
    __set_tex_prop(properties, PROP_SHAD_ENVMAP_NEGX, this->negx_texture);
    __set_tex_prop(properties, PROP_SHAD_ENVMAP_POSY, this->posy_texture);
    __set_tex_prop(properties, PROP_SHAD_ENVMAP_NEGY, this->negy_texture);
    __set_tex_prop(properties, PROP_SHAD_ENVMAP_POSZ, this->posz_texture);
    __set_tex_prop(properties, PROP_SHAD_ENVMAP_NEGZ, this->negz_texture);
}




AWDSSSDiffuseMethod::AWDSSSDiffuseMethod()
{
    this->type = AWD_SHADETYPE_SSS_DIFF;
    this->scattering = 0.2;
    this->translucency = 1.0;
}

void
AWDSSSDiffuseMethod::prepare_write()
{
    AWDBasicDiffuseMethod::prepare_write();

    if (this->scattering != 0.2) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->scattering;
        this->properties->set(PROP_SHAD_SCATTERING, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }

    if (this->translucency != 1.0) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->translucency;
        this->properties->set(PROP_SHAD_TRANSLUCENCY, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }
}




AWDTerrainDiffuseMethod::AWDTerrainDiffuseMethod()
{
    this->type = AWD_SHADETYPE_TERRAIN_DIFF;
}




AWDBasicSpecularMethod::AWDBasicSpecularMethod()
{
    this->type = AWD_SHADETYPE_BASIC_SPEC;
    this->strength = 1.0;
    this->color = 0xffffff;
    this->gloss = 50.0;
    this->specular_map = NULL;
    this->gloss_map = NULL;
}

void
AWDBasicSpecularMethod::prepare_write()
{
    if (this->strength != 1.0) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->strength;
        this->properties->set(PROP_SHAD_STRENGTH, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }

    if (this->color != 0xffffff) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_color));
        *val.col = this->color;
        this->properties->set(PROP_SHAD_COLOR, val, sizeof(awd_color), AWD_FIELD_COLOR);
    }

    if (this->gloss != 50.0) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->gloss;
        this->properties->set(PROP_SHAD_GLOSS, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }

    __set_tex_prop(properties, PROP_SHAD_MAP, this->specular_map);
    __set_tex_prop(properties, PROP_SHAD_GLOSS_MAP, this->gloss_map);
}




AWDCelSpecularMethod::AWDCelSpecularMethod()
{
    this->type = AWD_SHADETYPE_CEL_SPEC;
    this->smoothness = 0.1;
    this->cutoff = 0.5;
}

void
AWDCelSpecularMethod::prepare_write()
{
    AWDBasicSpecularMethod::prepare_write();

    if (this->smoothness != 0.1) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->smoothness;
        this->properties->set(PROP_SHAD_CEL_SMOOTHNESS, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }

    if (this->cutoff != 0.5) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->cutoff;
        this->properties->set(PROP_SHAD_CEL_CUTOFF, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }
}




AWDFresnelSpecularMethod::AWDFresnelSpecularMethod()
{
    this->type = AWD_SHADETYPE_FRESNEL_SPEC;
    this->normal_reflectance = 0.028;
}

void
AWDFresnelSpecularMethod::prepare_write()
{
    AWDBasicSpecularMethod::prepare_write();

    if (this->normal_reflectance != 0.028) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->normal_reflectance;
        this->properties->set(PROP_SHAD_NORMAL_REFLECTANCE, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }
}




AWDFilteredShadowMapMethod::AWDFilteredShadowMapMethod()
{
    this->type = AWD_SHADETYPE_FILTERED_SHAD;
    this->epsilon = 0.003;
    this->step_size = 0.0;
}

void
AWDFilteredShadowMapMethod::prepare_write()
{
    if (this->epsilon != 0.003) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->epsilon;
        this->properties->set(PROP_SHAD_EPSILON, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }

    if (this->step_size != 0.0) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->step_size;
        this->properties->set(PROP_SHAD_STEP_SIZE, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }
}




AWDHardShadowMapMethod::AWDHardShadowMapMethod()
{
    this->type = AWD_SHADETYPE_HARD_SHAD;
    this->epsilon = 0.003;
}

void
AWDHardShadowMapMethod::prepare_write()
{
    if (this->epsilon != 0.003) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->epsilon;
        this->properties->set(PROP_SHAD_EPSILON, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }
}




AWDSoftShadowMapMethod::AWDSoftShadowMapMethod()
{
    this->type = AWD_SHADETYPE_SOFT_SHAD;
    this->epsilon = 0.003;
    this->step_size = 0.0;
}

void
AWDSoftShadowMapMethod::prepare_write()
{
    if (this->epsilon != 0.003) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->epsilon;
        this->properties->set(PROP_SHAD_EPSILON, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }

    if (this->step_size != 0.0) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->step_size;
        this->properties->set(PROP_SHAD_STEP_SIZE, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }
}




AWDSlowFilteredShadowMapMethod::AWDSlowFilteredShadowMapMethod()
{
    this->type = AWD_SHADETYPE_SLOW_FILTERED_SHAD;
    this->epsilon = 0.003;
    this->step_size = 0.0;
}

void
AWDSlowFilteredShadowMapMethod::prepare_write()
{
    if (this->epsilon != 0.003) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->epsilon;
        this->properties->set(PROP_SHAD_EPSILON, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }

    if (this->step_size != 0.0) {
        AWD_field_ptr val;
        val.v = malloc(sizeof(awd_float64));
        *val.f64 = this->step_size;
        this->properties->set(PROP_SHAD_STEP_SIZE, val, sizeof(awd_float64), AWD_FIELD_FLOAT64);
    }
}

