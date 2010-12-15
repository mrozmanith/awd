#ifndef _PYAWD_AWDSUBMESH_H
#define _PYAWD_AWDSUBMESH_H

#include <awd/mesh.h>

typedef struct _pyawd_AWDSubMesh {
    AWD_sub_mesh *              ob_sub;

    struct _pyawd_AWDSubMesh *  next;
} pyawd_AWDSubMesh;


#endif

