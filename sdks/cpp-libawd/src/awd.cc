#include <cstdio>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include "awdw32.h"
#else
#include <unistd.h>
#endif

#include "awd.h"
#include "mesh.h"
#include "skeleton.h"
#include "skelanim.h"
#include "util.h"
#include "awdlzma.h"

#include "Types.h"
#include "LzmaEnc.h"


AWD::AWD(AWD_compression compression, awd_uint16 flags)
{
    this->major_version = AWD_MAJOR_VERSION;
    this->minor_version = AWD_MINOR_VERSION;
    this->compression = compression;
    this->flags = flags;
    this->mesh_data_blocks = new AWDBlockList();
    this->mesh_inst_blocks = new AWDBlockList();
    this->skeleton_blocks = new AWDBlockList();
    this->skelanim_blocks = new AWDBlockList();
    this->skelpose_blocks = new AWDBlockList();

    this->last_used_baddr = 0;
    this->header_written = AWD_FALSE;
}


void
AWD::add_mesh_data(AWDMeshData *block)
{
    this->mesh_data_blocks->append(block);
}


void
AWD::add_mesh_inst(AWDMeshInst *block)
{
    this->mesh_inst_blocks->append(block);
}


void
AWD::add_skeleton(AWDSkeleton *block)
{
    this->skeleton_blocks->append(block);
}


void
AWD::add_skeleton_pose(AWDSkeletonPose *block)
{
    this->skelpose_blocks->append(block);
}


void
AWD::add_skeleton_anim(AWDSkeletonAnimation *block)
{
    this->skelanim_blocks->append(block);
}



void
AWD::write_header(int fd, awd_uint32 body_length)
{
    awd_uint16 flags_be;

    // Convert to big-endian if necessary
    flags_be = UI16(this->flags);
    body_length = UI32(body_length);

    write(fd, "AWD", 3);
    write(fd, &this->major_version, sizeof(awd_uint8));
    write(fd, &this->minor_version, sizeof(awd_uint8));
    write(fd, &flags_be, sizeof(awd_uint16));
    write(fd, (awd_uint8*)&this->compression, sizeof(awd_uint8));
    write(fd, &body_length, sizeof(awd_uint32));
}

size_t
AWD::write_blocks(AWDBlockList *blocks, int fd)
{
    size_t len;
    AWDBlock *block;
    AWDBlockIterator *it;

    len = 0;
    it = new AWDBlockIterator(blocks);
    while ((block = it->next()) != NULL) {
        //TODO: Check flags for wide boolean (hard-coded as false now)
        len += block->write_block(fd, AWD_FALSE, ++this->last_used_baddr);
    }

    return len;
}


awd_uint32
AWD::flush(int out_fd)
{
    int tmp_fd;
    char *tmp_path;

    off_t tmp_len;
    awd_uint8 *tmp_buf;

    awd_uint8 *body_buf;
    awd_uint32 body_len;

    tmp_len = 0;
    tmp_fd = awdutil_mktmp(&tmp_path);
    if (tmp_fd < 0) {
        extern int errno;
        printf("Could not open temporary file necessary for writing, errno=%d\n", errno);
        return AWD_FALSE;
    }

    tmp_len += this->write_blocks(this->skeleton_blocks, tmp_fd);
    tmp_len += this->write_blocks(this->skelpose_blocks, tmp_fd);
    tmp_len += this->write_blocks(this->skelanim_blocks, tmp_fd);
    tmp_len += this->write_blocks(this->mesh_data_blocks, tmp_fd);
    tmp_len += this->write_blocks(this->mesh_inst_blocks, tmp_fd);

    tmp_buf = (awd_uint8 *) malloc(tmp_len);
	lseek(tmp_fd, 0, SEEK_SET);
    read(tmp_fd, tmp_buf, tmp_len);

    // Temp file no longer needed
    close(tmp_fd);
    unlink((const char *)tmp_path);


    if (this->compression == UNCOMPRESSED) {
        // Uncompressed, so output should be the exact
        // same data that was in the temporary file
        body_len = tmp_len;
        body_buf = tmp_buf;
    }
    else if (this->compression == DEFLATE) {
        // TODO: Implement deflate
        printf("DEFLATE compression not implemented in LibAWD!\n");
    }
    else if (this->compression == LZMA) {
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
        lzma_buf = (Byte *)malloc(tmp_len);
        props_len = sizeof(CLzmaEncProps);
        props_buf = (Byte *)malloc(props_len);

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
        body_buf = (awd_uint8*)malloc(body_len);
        memcpy(body_buf, props_buf, props_len);
        memcpy(body_buf+props_len, &tmp_len_bo, sizeof(awd_uint32));
        memcpy(body_buf+props_len+sizeof(awd_uint32), lzma_buf, lzma_len);
    }


    // Write header and then body from possibly
    // compressed buffer
    if (this->header_written == AWD_FALSE) {
        this->header_written = AWD_TRUE;
        this->write_header(out_fd, body_len);
    }

    write(out_fd, body_buf, body_len);

    return AWD_TRUE;
}


