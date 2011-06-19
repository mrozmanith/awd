#ifndef _LIBAWD_MESH_H
#define _LIBAWD_MESH_H

#include "awd_types.h"
#include "material.h"
#include "scene.h"
#include "stream.h"
#include "block.h"
#include "name.h"
#include "attr.h"


#define PROP_MD_BIND_MTX 1


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
    JOINT_INDICES,
    VERTEX_WEIGHTS,
} AWD_mesh_str_type;



class AWDSubMesh
{
    private:
        AWDDataStream * last_stream;

    public:
        AWDSubMesh();
        ~AWDSubMesh();

        AWDDataStream * first_stream;
        AWDSubMesh * next;

        void add_stream(AWD_mesh_str_type, AWD_str_ptr, awd_uint32);
};


class AWDMeshData : 
    public AWDBlock, 
    public AWDNamedElement,
    public AWDAttrElement
{
    private:
        int num_subs;
        AWDSubMesh * first_sub;
        AWDSubMesh * last_sub;

        awd_float64 * bind_mtx;

    protected:
        awd_uint32 calc_body_length(bool, bool);
        void prepare_write();
        void write_body(int, bool, bool);

    public:
        AWDMeshData(const char *, awd_uint16);
        ~AWDMeshData();

        int get_num_subs();
        AWDSubMesh *get_sub_at(int);
        void add_sub_mesh(AWDSubMesh *);

        awd_float64 *get_bind_mtx();
        void set_bind_mtx(awd_float64 *bind_mtx);
};



class AWDMeshInst : 
    public AWDSceneBlock
{
    private:
        AWDMeshData * data;
        AWDBlockList * materials;

        void init();

    protected:
        awd_uint32 calc_body_length(bool, bool);
        void write_body(int, bool, bool);

    public:
        AWDMeshInst(const char *, awd_uint16, AWDMeshData *);
        AWDMeshInst(const char *, awd_uint16, AWDMeshData *, awd_float64 *);
        ~AWDMeshInst();

        void add_material(AWDSimpleMaterial *);

        AWDMeshData * get_data();
        void set_data(AWDMeshData *);
};

#endif
