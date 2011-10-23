#ifndef _GEOMUTILS_H
#define _GEOMUTILS_H

#include "mesh.h"

typedef struct _ninfluence {
    double nx;
    double ny;
    double nz;
    struct _ninfluence *next;
} ninfluence;


typedef struct _vdata {
    unsigned int orig_idx;
    int out_idx;
    double x;
    double y;
    double z;
    double u;
    double v;
    double nx;
    double ny;
    double nz;

    bool force_hard;
    struct _vdata *next_exp;
    struct _vdata *next_col;

    ninfluence *first_normal_influence;
    ninfluence *last_normal_influence;
} vdata;

class AWDGeomUtil
{
private:
    vdata *exp_first_vd;
    vdata *exp_last_vd;
    vdata *col_first_vd;
    vdata *col_last_vd;

    int has_vert(vdata *);

public:
    AWDGeomUtil();
    ~AWDGeomUtil();

    double normal_threshold;

    void append_vert_data(unsigned int,  double, double, double, double, double, double, double, double, bool);
    int build_geom(AWDMeshData *);
};


#endif
