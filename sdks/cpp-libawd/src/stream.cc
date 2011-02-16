#include "libawd.h"

AWDDataStream::AWDDataStream(awd_uint8 type, AWD_str_ptr data, awd_uint32 num_elements)
{
    this->type = type;
    this->data = data;
    this->num_elements = num_elements;
}




awd_uint32
AWDDataStream::get_length(awd_bool wide)
{
    size_t elem_size;

    elem_size = this->get_elem_size(wide);
    return (this->num_elements * elem_size);
}



void
AWDDataStream::write_stream(int fd, awd_bool wide)
{
    unsigned int e;
    awd_uint32 num;
    awd_uint32 str_len;
    
    str_len = UI32(this->get_length(wide));
    
    write(fd, (awd_uint8*)&this->type, sizeof(awd_uint8));
    write(fd, &str_len, sizeof(awd_uint32));
    
    num = this->num_elements;
    // TODO: Consider making this nicer by storing 
    // data type in class somehow.
    if (this->type == VERTICES || this->type == UVS) {
        for (e=0; e<num; e++) {
            awd_float64 *p = (this->data.f64 + e);
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
    else if (this->type == TRIANGLES) {
        for (e=0; e<num; e++) {
            awd_uint32 *p = (this->data.ui32 + e);

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






AWDMeshDataStream::AWDMeshDataStream(awd_uint8 type, AWD_str_ptr data, awd_uint32 num_elements)
    : AWDDataStream((awd_uint8)type, data, num_elements)
{}

size_t
AWDMeshDataStream::get_elem_size(awd_bool wide)
{
    size_t elem_size;

    switch ((AWD_mesh_str_type)this->type) {
        case VERTICES:
            elem_size = sizeof(awd_float32);
            break;
        case TRIANGLES:
            elem_size = sizeof(awd_uint16);
            break;
        case UVS:
            elem_size = sizeof(awd_float32);
            break;
        case VERTEX_NORMALS:
            break;
        case VERTEX_TANGENTS:
            break;
        case FACE_NORMALS:
            break;
    }

    if (wide == AWD_TRUE)
        elem_size *= 2;

    return elem_size;
}



AWDPathDataStream::AWDPathDataStream(awd_uint8 type, AWD_str_ptr data, awd_uint32 num_elements)
    : AWDDataStream(type, data, num_elements)
{}

size_t
AWDPathDataStream::get_elem_size(awd_bool wide)
{
    return 0;
}
