#ifndef _LIBAWD_MESH_H
#define _LIBAWD_MESH_H

#include "awd_types.h"
#include "block.h"
#include "stream.h"

using namespace std;


/**
 * Data stream type
*/
typedef enum {
    VERTICES=1,
    TRIANGLES,
    UVS,
    VERTEX_NORMALS,
    VERTEX_TANGENTS,
    FACE_NORMALS,
} AWD_mesh_str_type;



class AWDSubMesh
{
    private:
        AWDDataStream * last_stream;

    public:
        AWDSubMesh();
        AWDDataStream * first_stream;
        AWDSubMesh * next;

        void add_stream(AWD_mesh_str_type, AWD_str_ptr, awd_uint32);
};


class AWDMeshData : public AWDBlock
{
    private:
        AWDSubMesh * first_sub;
        AWDSubMesh * last_sub;

    protected:
        awd_uint32 calc_body_length(awd_bool);
        void write_body(int, awd_bool);

    public:
        AWDMeshData();

        void add_sub_mesh(AWDSubMesh *);
};



class AWDMeshInst : public AWDBlock
{
    private:
        AWDMeshData * data;
        awd_float64 transform_mtx[16];

    protected:
        awd_uint32 calc_body_length(awd_bool);
        void write_body(int, awd_bool);

    public:
        AWDMeshInst(AWDMeshData *);

        AWDMeshData * get_data();
};

#endif
