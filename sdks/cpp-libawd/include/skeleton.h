#ifndef _LIBAWD_SKELETON_H
#define _LIBAWD_SKELETON_H

#include "block.h"
#include "attr.h"



class AWDSkeletonJoint : 
    public AWDNamedElement,
    public AWDAttrElement
{
    private:
        awd_uint32 id;
        awd_float64 *bind_mtx;
        int num_children;

        AWDSkeletonJoint *parent;
        AWDSkeletonJoint *first_child;
        AWDSkeletonJoint *last_child;
        
    public:
        AWDSkeletonJoint *next;

        AWDSkeletonJoint(const char *, awd_uint16, awd_float64 *);

        int write_joint(int, awd_uint32);
        int calc_length();
        int calc_num_children();

        awd_uint32 get_id();
        void set_parent(AWDSkeletonJoint *);
        AWDSkeletonJoint *get_parent();
        AWDSkeletonJoint *add_child_joint(AWDSkeletonJoint *);
};


class AWDSkeleton : 
    public AWDBlock, 
    public AWDNamedElement,
    public AWDAttrElement
{
    private:
        AWDSkeletonJoint *root_joint;

    protected:
        awd_uint32 calc_body_length(awd_bool);
        void write_body(int, awd_bool);

    public:
        AWDSkeleton(const char *, awd_uint16);

        AWDSkeletonJoint *set_root_joint(AWDSkeletonJoint *);
        AWDSkeletonJoint *get_root_joint();
};

#endif
