#include <stdlib.h>

#include "libawd.h"


void 
AWDSubMesh::add_stream(AWD_mesh_str_type type, AWD_str_ptr data, awd_uint32 num_elements)
{
    AWDMeshDataStream *str;

    str = new AWDMeshDataStream((awd_uint8)type, data, num_elements);

    if (this->first_stream == NULL) {
        this->first_stream = str;
    }
    else {
        this->last_stream->next = str;
        this->last_stream = str;
    }
}






AWDMeshData::AWDMeshData()
{
    this->type = MESH_DATA;
}

void 
AWDMeshData::add_sub_mesh(AWDSubMesh *sub)
{
    if (this->first_sub == NULL) {
        this->first_sub = sub;
    }
    else {
        this->last_sub->next = sub;
        this->last_sub = sub;
    }
}


awd_uint32
AWDMeshData::calc_body_length(awd_bool wide)
{
    AWDSubMesh *sub;
    awd_uint32 mesh_len;

    // Calculate length of entire mesh 
    // data (not block header)
    mesh_len = 0;
    sub = this->first_sub;
    while (sub) {
        AWDDataStream *str;
        
        // add size of mat ID and sub-mesh
        // length (both awd_uint32)
        mesh_len += 8;

        str = sub->first_stream;
        while (str) {
            mesh_len += 5 + str->get_length(wide);

            str = str->next;
        }

        sub = sub->next;
    }

    return mesh_len;
}



void
AWDMeshData::write_body(int fd, awd_bool wide)
{
    AWDSubMesh *sub;

    // Write all sub-meshes
    sub = this->first_sub;
    while (sub) {
        AWDDataStream *str;
        awd_baddr mat_id;
        awd_uint32 sub_len;

        sub_len = 0;
        str = sub->first_stream;
        while (str) {
            sub_len += (str->get_length(wide) + 5);

            str = str->next;
        }

        // TODO: Find material correctly
        //mat_id = _awd_get_block_id_by_data(sub->material);
        mat_id = 0;

        // Verify byte-order
        mat_id = UI32(mat_id);
        sub_len = UI32(sub_len);

        // Write sub-mesh header
        write(fd, &mat_id, sizeof(awd_baddr)); 
        write(fd, &sub_len, sizeof(awd_uint32));

        str = sub->first_stream;
        while(str) {
            str->write_stream(fd, wide);

            str = str->next;
        }

        sub = sub->next;
    }

    //_awd_write_attributes(awd, data->attributes, fd);
}





AWDMeshInst::AWDMeshInst(AWDMeshData *data)
{
    this->type = MESH_INSTANCE;
    this->data = data;
    this->transform_mtx[0] = 1.0;
    this->transform_mtx[5] = 1.0;
    this->transform_mtx[10] = 1.0;
    this->transform_mtx[15] = 1.0;
}


AWDMeshData *
AWDMeshInst::get_data()
{
    return this->data;
}


awd_uint32
AWDMeshInst::calc_body_length(awd_bool wide)
{
    return 136;
}


void
AWDMeshInst::write_body(int fd, awd_bool wide)
{
    int i;
    awd_float64 n;
    awd_baddr parent_addr;
    awd_baddr data_addr;

    // TODO: Use (and create) awd->scene_blocks instead
    // Get IDs for references, verify byte-order
    parent_addr = 0;
    data_addr = UI32(this->data->get_addr());

    write(fd, &parent_addr, sizeof(awd_baddr));
    for (i=0; i<16; i++) {
        n = F64(this->transform_mtx[i]);
        write(fd, &n, sizeof(awd_float64));
    }
    write(fd, &data_addr, sizeof(awd_uint32));
}
