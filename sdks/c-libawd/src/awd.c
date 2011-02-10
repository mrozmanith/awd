#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "libawd.h"

#include "Types.h"
#include "LzmaEnc.h"


AWD *
awd_create(AWD_compression compression, awd_uint16 flags)
{
    AWD *awd;
    AWD_header *header;

    header = malloc(sizeof(AWD_header));
    header->major_version = AWD_MAJOR_VERSION;
    header->minor_version = AWD_MINOR_VERSION;
    header->compression = compression;
    header->flags = flags;

    awd = malloc(sizeof(AWD));
    awd->header = header;
    awd->namespaces = malloc(256 * sizeof(AWD_namespace *));
    
    awd->_last_block_id = 0;
    awd->_last_ns_id = 0;
    awd->_header_written = AWD_FALSE;

    // No block lists initially
    awd->mesh_instances = NULL;
    awd->meshes = NULL;

    return awd;
}


awd_bool 
awd_add_mesh_inst(AWD *awd, AWD_mesh_data *data, awd_float64 *mtx, AWD_attr_list *attributes)
{
    AWD_mesh_inst *inst;
    AWD_block *inst_block;

    // Use identity if no transform matrix has been defined
    if (mtx == NULL) {
        mtx = malloc(16 * sizeof(awd_float64));
        awdutil_id_mtx4(mtx);
    }


    inst = malloc(sizeof(AWD_mesh_inst));
    inst->data = data;
    inst->parent = NULL;
    inst->transform_mtx = mtx;

    // Add mesh data if it doesn't exist
    if (_awd_get_block_by_data(awd->meshes, (void *)data) == AWD_NULL) {
        AWD_block *data_block;

        // Create mesh list if none exists (which would happen
        // if this is the first mesh to be added.)
        if (awd->meshes == NULL) {
            awd->meshes = _awd_alloc_block_list();
        }

        data_block = _awd_create_block(MESH_DATA, data);

        _awd_append_block(awd->meshes, data_block);
    }


    // Create instance list if none exists (this is first inst)
    if (awd->mesh_instances == NULL) {
        awd->mesh_instances = _awd_alloc_block_list();
    }

    inst_block = _awd_create_block(MESH_INSTANCE, inst);
    _awd_append_block(awd->mesh_instances, inst_block);

    return AWD_TRUE;
}



awd_bool 
awd_flush(AWD *awd, int out_fd)
{
    int tmp_fd;
    char *tmp_path;

    off_t tmp_len;
    awd_uint8 *tmp_buf;

    awd_uint8 *body_buf;
    awd_uint32 body_len;

    tmp_fd = awdutil_mktmp(&tmp_path);
    if (tmp_fd < 0) {
        extern int errno;
        printf("Could not open temporary file necessary for writing, errno=%d\n", errno);
        return AWD_FALSE;
    }

    if (awd->meshes) {
        _awd_write_blocks(awd, awd->meshes, tmp_fd, &_awd_write_mesh_data);
    }

    if (awd->mesh_instances) {
        _awd_write_blocks(awd, awd->mesh_instances, tmp_fd, &_awd_write_mesh_inst);
    }

    tmp_len = lseek(tmp_fd, 0, SEEK_END);
    tmp_buf = malloc(tmp_len);
    pread(tmp_fd, tmp_buf, tmp_len, 0);

    // Temp file no longer needed
    close(tmp_fd);
    unlink((const char *)tmp_path);


    if (awd->header->compression == UNCOMPRESSED) {
        // Uncompressed, so output should be the exact
        // same data that was in the temporary file
        body_len = tmp_len;
        body_buf = tmp_buf;
    }
    else if (awd->header->compression == DEFLATE) {
        // TODO: Implement deflate
        printf("DEFLATE compression not implemented in LibAWD!\n");
    }
    else if (awd->header->compression == LZMA) {
        Byte *lzma_buf;
        SizeT lzma_len, props_len;
        CLzmaEncProps props;
        ISzAlloc alloc;
        Byte *props_buf;
        long tmp_len_bo;

        // Create allocation structure. LZMA library uses
        // these functions for memory management. They are
        // defined in awdlzma.c as simple wrappers for 
        // malloc() and free().
        alloc.Alloc = &awd_SzAlloc;
        alloc.Free = &awd_SzFree;

        lzma_len = tmp_len;
        lzma_buf = malloc(tmp_len);
        props_len = sizeof(CLzmaEncProps);
        props_buf = malloc(props_len);

        LzmaEncProps_Init(&props);
        props.algo = 1;
        props.level = 9;

        LzmaEncode(lzma_buf, &lzma_len, tmp_buf, tmp_len,
            &props, props_buf, &props_len, 0, NULL, &alloc, &alloc);

        // Body length is the length of the actual
        // compressed body + size of props and an integer
        // definining the uncompressed length (see below)
        body_len = (awd_uint32)lzma_len + (awd_uint32)props_len + sizeof(awd_uint32);

        // Create new buffer containing LZMA props, length
        // of uncompressed body and the actual body data
        // concatenated together.
        tmp_len_bo = UI32(tmp_len);
        body_buf = malloc(body_len);
        memcpy(body_buf, props_buf, props_len);
        memcpy(body_buf+props_len, &tmp_len_bo, sizeof(awd_uint32));
        memcpy(body_buf+props_len+sizeof(awd_uint32), lzma_buf, lzma_len);
    }


    // Write header and then body from possibly
    // compressed buffer
    if (awd->_header_written == AWD_FALSE) {
        awd->_header_written = AWD_TRUE;
        _awd_write_header(awd, out_fd, body_len);
    }
    write(out_fd, body_buf, body_len);

    return AWD_TRUE;
}


