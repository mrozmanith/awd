#ifndef _LIBAWD_MESH_H
#define _LIBAWD_MESH_H

#include "awd_types.h"
#include "block.h"
#include "stream.h"
#include "skeleton.h"


#define PROP_MD_SKELETON 1


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
    VERTEX_WEIGHTS,
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


class AWDMeshData : public AWDBlock, public AWDAttrElement
{
    private:
        int num_subs;
        AWDSubMesh * first_sub;
        AWDSubMesh * last_sub;

        AWDSkeleton * skeleton;

    protected:
        awd_uint32 calc_body_length(awd_bool);
        void prepare_write();
        void write_body(int, awd_bool);

    public:
        AWDMeshData();

        int get_num_subs();
        AWDSubMesh *get_sub_at(int);
        void add_sub_mesh(AWDSubMesh *);

        AWDSkeleton *get_skeleton();
        void set_skeleton(AWDSkeleton *skeleton);
};



class AWDMeshInst : public AWDBlock, public AWDAttrElement
{
    private:
        AWDMeshData * data;
        awd_float64 * transform_mtx;

        void init();

    protected:
        awd_uint32 calc_body_length(awd_bool);
        void write_body(int, awd_bool);

    public:
        AWDMeshInst(AWDMeshData *);
        AWDMeshInst(AWDMeshData *, awd_float64 *);

        void set_transform(awd_float64 *);

        AWDMeshData * get_data();
        void set_data(AWDMeshData *);

};

#endif
