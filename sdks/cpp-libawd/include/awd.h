#ifndef _LIBAWD_AWD_H
#define _LIBAWD_AWD_H

#include "mesh.h"
#include "block.h"
#include "awd_types.h"


#define AWD_MAJOR_VERSION 0
#define AWD_MINOR_VERSION 1

#define AWD_TRUE 1
#define AWD_FALSE 0
#define AWD_NULL 0

#define AWD_STREAMING               0x1
#define AWD_OPTIMIZE_FOR_ACCURACY   0x2


using namespace std;

class AWD
{
    private:
        // File header fields
        awd_uint8 major_version;
        awd_uint8 minor_version;
        awd_uint16 flags;
        AWD_compression compression;

        AWDBlockList * mesh_data_blocks;
        AWDBlockList * mesh_inst_blocks;

        // Flags and misc
        awd_baddr last_used_baddr;
        awd_bool header_written;

        void write_header(int, awd_uint32);
        size_t write_blocks(AWDBlockList *, int);

    public:
        AWD(AWD_compression, awd_uint16);
        awd_uint32 flush(int);

        void add_mesh_data(AWDMeshData *);
        void add_mesh_inst(AWDMeshInst *);
};

#endif
