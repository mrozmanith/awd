#include "util.h"
#include "skelanim.h"

#ifdef WIN32
#include "awdw32.h"
#else
#include "unistd.h"
#endif


AWDSkeletonPose::AWDSkeletonPose(const char *name, awd_uint16 name_len) :
    AWDNamedElement(name, name_len), AWDAttrElement(), AWDBlock(SKELETON_POSE)
{
    this->num_transforms = 0;
    this->first_transform = NULL;
    this->last_transform = NULL;
}


AWDSkeletonPose::~AWDSkeletonPose()
{
    AWD_joint_tf *cur;

    cur = this->first_transform;
    while (cur) {
        AWD_joint_tf *next = cur->next;
        cur->next = NULL;
        if (cur->transform_mtx) {
            free(cur->transform_mtx);
            cur->transform_mtx = NULL;
        }
        free(cur);
        cur = next;
    }

    this->num_transforms = 0;
    this->first_transform = NULL;
    this->last_transform = NULL;
}


void
AWDSkeletonPose::set_next_transform(awd_float64 *mtx)
{
    AWD_joint_tf *tf;

    tf = (AWD_joint_tf*)malloc(sizeof(AWD_joint_tf));
    tf->transform_mtx = mtx;

    if (this->first_transform == NULL) {
        this->first_transform = tf;
    }
    else {
        this->last_transform->next = tf;
    }

    this->num_transforms++;
    this->last_transform = tf;
    this->last_transform->next = NULL;
}


awd_uint32
AWDSkeletonPose::calc_body_length(awd_bool wide)
{
    awd_uint32 len;
    AWD_joint_tf *cur;

    len = this->get_name_length() + 4; // strlen field + num transforms
    len += this->calc_attr_length(true,true);

    cur = this->first_transform;
    while (cur) {
        len += sizeof(awd_bool);
        if (cur->transform_mtx != NULL)
            len += 128;

        cur = cur->next;
    }

    return len;
}


void
AWDSkeletonPose::write_body(int fd, awd_bool wide)
{
    awd_bool bt;
    awd_bool bf;
    AWD_joint_tf *cur;
    awd_uint16 num_joints_be;

    awdutil_write_varstr(fd, this->get_name(), this->get_name_length());
    num_joints_be = UI16(this->num_transforms);
    write(fd, &num_joints_be, sizeof(awd_uint16));

    this->properties->write_attributes(fd);

    bt = AWD_TRUE;
    bf = AWD_FALSE;
    cur = this->first_transform;
    while (cur) {
        if (cur->transform_mtx) {
            write(fd, &bt, 1);
            awdutil_write_mtx4(fd, cur->transform_mtx);
        }
        else {
            write(fd, &bf, 1);
        }

        cur = cur->next;
    }

    this->user_attributes->write_attributes(fd);
}





AWDSkeletonAnimation::AWDSkeletonAnimation(const char *name, awd_uint16 name_len, awd_uint8 frame_rate) :
    AWDNamedElement(name, name_len), AWDAttrElement(), AWDBlock(SKELETON_ANIM)
{
    this->frame_rate = frame_rate;
    this->num_frames = 0;
    this->first_frame = NULL;
    this->last_frame = NULL;
}


AWDSkeletonAnimation::~AWDSkeletonAnimation()
{
    AWD_skelanim_fr *cur;

    cur = this->first_frame;
    while (cur) {
        AWD_skelanim_fr *next = cur->next;
        cur->next = NULL;
        delete cur->pose;
        free(cur);
        cur = next;
    }

    this->first_frame = NULL;
    this->last_frame = NULL;
}


void
AWDSkeletonAnimation::set_next_frame_pose(AWDSkeletonPose *pose)
{
    AWD_skelanim_fr *fr;

    fr = (AWD_skelanim_fr *)malloc(sizeof(AWD_skelanim_fr));
    fr->pose = pose;

    if (this->first_frame == NULL) {
        this->first_frame = fr;
    }
    else {
        this->last_frame->next = fr;
    }

    this->num_frames++;
    this->last_frame = fr;
    this->last_frame->next = NULL;
}


awd_uint32
AWDSkeletonAnimation::calc_body_length(awd_bool wide)
{
    awd_uint32 len;

    len = 2 + this->get_name_length();              // Name varstr
    len += 3;                                       // num frames + frame rate
    len += (this->num_frames * sizeof(awd_baddr));  // Pose list
    len += this->calc_attr_length(true,true);       // Props and attributes
    
    return len;
}


void
AWDSkeletonAnimation::write_body(int fd, awd_bool wide)
{
    AWD_skelanim_fr *frame;
    awd_uint16 num_frames_be;

    awdutil_write_varstr(fd, this->get_name(), this->get_name_length());

    num_frames_be = UI16(this->num_frames);
    write(fd, &num_frames_be, sizeof(awd_uint16));
    write(fd, &this->frame_rate, sizeof(awd_uint8));

    this->properties->write_attributes(fd);

    frame = this->first_frame;
    while (frame) {
        awd_baddr addr_be = UI32(frame->pose->get_addr());
        write(fd, &addr_be, sizeof(awd_baddr));

        frame = frame->next;
    }

    this->user_attributes->write_attributes(fd);
}
