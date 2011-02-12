#include <unistd.h>

#include "libawd.h"

void 
_awd_write_header(AWD *awd, int fd, awd_uint32 body_length)
{
    awd_uint16 flags;

    // Convert byte-order
    flags = UI16(awd->header->flags);
    body_length = UI32(body_length);

    write(fd, "AWD", 3);
    write(fd, &awd->header->major_version, sizeof(awd_uint8));
    write(fd, &awd->header->minor_version, sizeof(awd_uint8));
    write(fd, &flags, sizeof(awd_uint16));
    write(fd, (awd_uint8*)&awd->header->compression, sizeof(awd_uint8));
    write(fd, &body_length, sizeof(awd_uint32));
}


void 
_awd_write_blocks(AWD *awd, AWD_block_list *list, int fd, void (*write_func)(AWD *, AWD_block *, int fd))
{
    AWD_block *next;

    next = list->first_block;
    while (next) {
        next->id = ++awd->_last_block_id;


        write_func(awd, next, fd);
        next = next->next;
    }
}


void 
_awd_write_block_header(AWD_block *block, awd_uint32 length, int fd)
{
    awd_uint8 ns_id;
    awd_uint32 block_id;

    // Convert byte-order
    block_id = UI32(block->id);
    length = UI32(length);

    ns_id=0;
    if (block->ns)
        ns_id = block->ns->id;

    write(fd, &block_id, sizeof(awd_uint32));
    write(fd, &ns_id, sizeof(awd_uint8));
    write(fd, &block->type, sizeof(awd_uint8));
    write(fd, &length, sizeof(awd_uint32));
}


void 
_awd_write_mesh_data(AWD *awd, AWD_block *block, int fd)
{
    awd_bool wide;
    awd_uint32 mesh_len;
    AWD_mesh_data *data;
    AWD_sub_mesh *sub;

    data = (AWD_mesh_data *)block->data;
    wide = awdutil_check_flag(awd, AWD_OPTIMIZE_FOR_ACCURACY);

    // Calculate length of entire mesh 
    // data (not block header)
    mesh_len = 0;
    sub = data->first_sub_mesh;
    while (sub) {
        AWD_mesh_data_stream *str;
        
        // add size of mat ID and sub-mesh
        // length (both awd_uint32)
        mesh_len += 8;

        str = sub->first_stream;
        while (str) {
            mesh_len += (awdutil_stream_len(str, wide) + 5);

            str = str->next;
        }

        sub = sub->next;
    }

    // Write block header
    _awd_write_block_header(block, mesh_len, fd);

    // Write all sub-meshes
    sub = data->first_sub_mesh;
    while (sub) {
        AWD_mesh_data_stream *str;
        awd_baddr mat_id;
        awd_uint32 sub_len;

        sub_len = 0;
        str = sub->first_stream;
        while (str) {
            sub_len += (awdutil_stream_len(str, wide) + 5);

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
            _awd_write_mesh_data_stream(str, wide, fd);

            str = str->next;
        }

        sub = sub->next;
    }
}


void 
_awd_write_mesh_data_stream(AWD_mesh_data_stream *str, awd_bool wide, int fd)
{
    int e;
    awd_uint32 num;
    size_t elem_size;
    awd_uint32 str_len;
    
    elem_size = awdutil_stream_elem_size(str, wide);
    str_len = UI32(awdutil_stream_len(str, wide));
    
    write(fd, (awd_uint8*)&str->type, sizeof(awd_uint8));
    write(fd, &str_len, sizeof(awd_uint32));
    
    num = str->num_elements;
    if (str->type == VERTICES || str->type == UVS) {
        for (e=0; e<num; e++) {
            awd_float64 *p = (str->data.f64 + e);
            if (wide) {
                awd_float64 elem;
                elem = F64((awd_float64)*p);
                write(fd, &elem, sizeof(awd_float64));
            }
            else {
                awd_float32 elem;
                elem = F32((awd_float32)*p);
                write(fd, &elem, sizeof(awd_float32));
            }
        }
    }
    else if (str->type == TRIANGLES) {
        for (e=0; e<num; e++) {
            awd_uint32 *p = (str->data.ui32 + e);

            if (wide) {
                awd_uint32 elem;
                elem = UI32((awd_uint32)*p);
                write(fd, &elem, sizeof(awd_uint32));
            }
            else {
                awd_uint16 elem;
                elem = UI16((awd_uint16)*p);
                write(fd, &elem, sizeof(awd_uint16));
            }
        }
    }
}


void 
_awd_write_mesh_inst(AWD *awd, AWD_block *block, int fd)
{
    int i;
    awd_float64 n;
    awd_baddr parent_id;
    awd_baddr data_id;
    AWD_mesh_inst *inst;

    inst = (AWD_mesh_inst *)block->data;

    // TODO: Use (and create) awd->scene_blocks instead
    // Get IDs for references, verify byte-order
    parent_id = UI32(_awd_get_block_id_by_data(awd->mesh_instances, inst->parent));
    data_id = UI32(_awd_get_block_id_by_data(awd->meshes, inst->data));

    _awd_write_block_header(block, 136, fd);
    write(fd, &parent_id, sizeof(awd_uint32));
    for (i=0; i<16; i++) {
        n = F64(inst->transform_mtx[i]);
        write(fd, &n, sizeof(awd_float64));
    }
    write(fd, &data_id, sizeof(awd_uint32));
}

