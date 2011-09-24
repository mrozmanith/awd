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
    JOINT_INDICES,
    VERTEX_WEIGHTS,
} AWD_mesh_str_type;



class AWDSubMesh :
    public AWDAttrElement
{
    private:
        unsigned int num_streams;
        AWDDataStream * first_stream;
        AWDDataStream * last_stream;
        awd_uint32 calc_streams_length(bool);

    public:
        AWDSubMesh();
        ~AWDSubMesh();

        AWDSubMesh * next;

        unsigned int get_num_streams();
        AWDDataStream *get_stream_at(unsigned int);
        void add_stream(AWD_mesh_str_type, AWD_str_ptr, awd_uint32);

        awd_uint32 calc_sub_length(bool, bool);
        void write_sub(int, bool, bool);
};


class AWDMeshData : 
    public AWDBlock, 
    public AWDNamedElement,
    public AWDAttrElement
{
    private:
        unsigned int num_subs;
        AWDSubMesh * first_sub;
        AWDSubMesh * last_sub;

        awd_float64 * bind_mtx;

    protected:
        awd_uint32 calc_body_length(bool, bool);
        void write_body(int, bool, bool);

    public:
        AWDMeshData(const char *, awd_uint16);
        ~AWDMeshData();

        unsigned int get_num_subs();
        AWDSubMesh *get_sub_at(unsigned int);
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

        void add_material(AWDMaterial *);

        AWDMeshData * get_data();
        void set_data(AWDMeshData *);
};

#endif
