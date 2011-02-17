#ifndef _LIBAWD_STREAM_H
#define _LIBAWD_STREAM_H

#include "libawd.h"

/** 
 * Data stream pointer
*/
typedef union {
    awd_uint32 *ui32;
    awd_float64 *f64;
} AWD_str_ptr;



class AWDDataStream
{
    private:
        awd_uint32 num_elements;

    protected:
        virtual size_t get_elem_size(awd_bool)=0;

    public:
        awd_uint8 type;
        AWD_str_ptr data;

        AWDDataStream * next;
        

        AWDDataStream(awd_uint8, AWD_str_ptr, awd_uint32);
        awd_uint32 get_length(awd_bool);
        void write_stream(int, awd_bool);
};



class AWDMeshDataStream : public AWDDataStream
{
    protected:
        size_t get_elem_size(awd_bool);

    public:
        AWDMeshDataStream(awd_uint8, AWD_str_ptr, awd_uint32);
};


class AWDPathDataStream : public AWDDataStream
{
    protected:
        size_t get_elem_size(awd_bool);

    public:
        AWDPathDataStream(awd_uint8, AWD_str_ptr, awd_uint32);
};



#endif