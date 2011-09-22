#include <math.h>
#include <stdlib.h>
#include <cstdio>

#include "geomutil.h"

AWDGeomUtil::AWDGeomUtil()
{
    this->col_first_vd = NULL;
    this->col_last_vd = NULL;
    this->exp_first_vd = NULL;
    this->exp_last_vd = NULL;
    this->normal_threshold = 3.14;
}

AWDGeomUtil::~AWDGeomUtil()
{
}


void
AWDGeomUtil::append_vert_data(unsigned int idx, double x, double y, double z,
    double u, double v, double nx, double ny, double nz, bool force_hard)
{
    vdata *vd;

    vd = (vdata *)malloc(sizeof(vdata));
    vd->idx = idx;
    vd->x = x;
    vd->y = y;
    vd->z = z;
    vd->u = u;
    vd->v = v;
    vd->nx = nx;
    vd->ny = ny;
    vd->nz = nz;
    vd->force_hard = force_hard;

    if (!this->exp_first_vd)
        this->exp_first_vd = vd;
    else
        this->exp_last_vd->next_exp = vd;

    this->exp_last_vd = vd;
}


int
AWDGeomUtil::has_vert(vdata *vd)
{
    int idx;
    vdata *cur;

    // TODO: Optimize by caching relationship between original vertex index
    // and found vertices.

    idx = 0;
    cur = this->col_first_vd;
    while (cur) {

        // If any of vertices have force_hard set, their normals must
        // not be averaged. Hence, they must be returned by this 
        // function as separate verts.
        printf("checking force\n");
        if (cur->force_hard || vd->force_hard)
            goto next;

        // If position doesn't match, move on to next vertex
        printf("checking position\n");
        if (cur->x != vd->x || cur->y != vd->y || cur->z != vd->z)
            goto next;

        // If UV coordinates do not match, move on to next vertex
        printf("checking UVs\n");
        if (cur->u != vd->u || cur->v != vd->v)
            goto next;

        // Check if normals match (possibly with fuzzy matching using a
        // threshold) and if they don't, move on to the next vertex.
        if (this->normal_threshold > 0) {
            double angle;

            angle = acos(cur->nx*vd->nx + cur->ny*vd->ny + cur->nz*vd->nz);
            if (angle > this->normal_threshold)
                goto next;
        }
        else if (cur->nx != vd->nx || cur->ny != vd->ny || cur->nz != vd->nz)
            goto next;


        // Made it here? Then vertices match!
        printf("match!\n");
        return idx;

next:
        idx++;
        cur = cur->next_col;
    }

    return -1;
}


int 
AWDGeomUtil::build_geom(AWDMeshData **md)
{
    vdata *vd;
    AWDSubMesh *sub;

    int v_idx, i_idx;
    AWD_str_ptr v_str;
    AWD_str_ptr i_str;

    sub = new AWDSubMesh();
    v_str.f64 = (awd_float64*) malloc(sizeof(awd_float64) * 0xffff);
    i_str.ui32 = (awd_uint32*) malloc(sizeof(awd_uint32) * 0xffff);

    v_idx = i_idx = 0;

    vd = this->exp_first_vd;
    while (vd) {
        int idx = this->has_vert(vd);
        if (idx >= 0) {
            i_str.ui32[i_idx++] = idx;
        }
        else {
            v_str.f64[v_idx*3+0] = vd->x;
            v_str.f64[v_idx*3+1] = vd->y;
            v_str.f64[v_idx*3+2] = vd->z;
            i_str.ui32[i_idx++] = v_idx++;

            if (!col_first_vd)
                col_first_vd = vd;
            else
                col_last_vd->next_col = vd;

            col_last_vd = vd;
        }

        vd = vd->next_exp;
    }

    int i;
    for (i=0; i<v_idx; i++) {
        printf("vert: %f, %f, %f\n", v_str.f64[i*3+0], v_str.f64[i*3+1], v_str.f64[i*3+2]);
    }
    for (i=0; i<i_idx; i++) {
        printf("index: %d\n", i_str.ui32[i]);
    }

    return 1;
}


