#include <cstdio>
#include <cstring>

#include "util.h"
#include "skeleton.h"

#ifdef WIN32
#include "awdw32.h"
#endif


AWDSkeletonJoint::AWDSkeletonJoint(const char *name, awd_uint16 name_len, awd_float64 *bind_mtx) :
    AWDNamedElement(name, name_len),
    AWDAttrElement()
{
    this->next = NULL;
    this->first_child = NULL;
    this->last_child = NULL;
    this->bind_mtx = bind_mtx;
    this->num_children = 0;

    if (this->bind_mtx == NULL) {
        this->bind_mtx = awdutil_id_mtx4(NULL);
    }
}


AWDSkeletonJoint::~AWDSkeletonJoint()
{
    AWDSkeletonJoint *cur;

    cur = this->first_child;
    while (cur) {
        AWDSkeletonJoint *next = cur->next;
        cur->next = NULL;
        delete cur; // Will remove it's children too (recursively)
        cur = next;
    }

    if (this->bind_mtx) {
        free(this->bind_mtx);
        this->bind_mtx = NULL;
    }

    this->num_children = 0;
    this->first_child = NULL;
    this->last_child = NULL;
}


awd_uint32
AWDSkeletonJoint::get_id()
{
    return this->id;
}


void
AWDSkeletonJoint::set_parent(AWDSkeletonJoint *joint)
{
    this->parent = joint;
}


AWDSkeletonJoint *
AWDSkeletonJoint::get_parent()
{
    return this->parent;
}



AWDSkeletonJoint *
AWDSkeletonJoint::add_child_joint(AWDSkeletonJoint *joint)
{
    if (joint != NULL) {
        if (joint->get_parent() != NULL) {
            // TODO: Remove from old parent
        }

        // Find place in list of children
        if (this->first_child == NULL) {
            this->first_child = joint;
        }
        else {
            this->last_child->next = joint;
        }

        joint->set_parent(this);
        this->last_child = joint;
        this->last_child->next = NULL;
        this->num_children++;
    }

    return joint;
}



int
AWDSkeletonJoint::calc_length()
{
    int len;
    AWDSkeletonJoint *child;
    
    len = sizeof(awd_uint16) + this->get_name_length() + 136;
    child = this->first_child;
    while (child) {
        len += child->calc_length();
        child = child->next;
    }

    return len;
}


int
AWDSkeletonJoint::calc_num_children()
{
    int num_children;
    AWDSkeletonJoint *child;

    num_children = this->num_children;
    child = this->first_child;
    while (child) {
        num_children += child->calc_num_children();
        child = child->next;
    }

    return num_children;
}


int
AWDSkeletonJoint::write_joint(int fd, awd_uint32 id)
{
    int num_written;
    awd_uint32 child_id;
    AWDSkeletonJoint *child;
    awd_uint32 par_id_be;
    awd_uint32 id_be;

    this->id = id;

    // Convert numbers to big-endian
    id_be = UI32(this->id);
    if (this->parent) 
        par_id_be = UI32(this->parent->id);
    else par_id_be = 0;

    // Write this joint
    write(fd, &id_be, sizeof(awd_uint32));
    write(fd, &par_id_be, sizeof(awd_uint32));
    awdutil_write_varstr(fd, this->get_name(), this->get_name_length());
    awdutil_write_mtx4(fd, this->bind_mtx);

    //  TODO: Write attributes

    // Write children
    child_id = id+1;
    num_written = 1;
    child = this->first_child;
    while (child) {
        int num_children_written;

        num_children_written = child->write_joint(fd, child_id);

        child_id += num_children_written;
        num_written += num_children_written;

        child = child->next;
    }

    return num_written;
}






AWDSkeleton::AWDSkeleton(const char *name, awd_uint16 name_len) :
    AWDBlock(SKELETON),
    AWDNamedElement(name, name_len), 
    AWDAttrElement()
{
    this->root_joint = NULL;
}


AWDSkeleton::~AWDSkeleton()
{
    if (this->root_joint) {
        delete this->root_joint;
        this->root_joint = NULL;
    }
}


awd_uint32
AWDSkeleton::calc_body_length(awd_bool wide)
{
    awd_uint32 len;

    len = sizeof(awd_uint16) + this->get_name_length() + sizeof(awd_uint16);
    len += this->calc_attr_length(true,true);

    if (this->root_joint != NULL)
        len += this->root_joint->calc_length();

    return len;
}


void
AWDSkeleton::write_body(int fd, awd_bool wide)
{
    awd_uint16 num_joints_be;

    num_joints_be = 0;
    if (this->root_joint != NULL)
        num_joints_be = UI16(1 + this->root_joint->calc_num_children());

    awdutil_write_varstr(fd, this->get_name(), this->get_name_length());
    write(fd, &num_joints_be, sizeof(awd_uint16));

    // Write optional properties
    this->properties->write_attributes(fd);

    // Write joints (if any)
    if (this->root_joint != NULL)
        this->root_joint->write_joint(fd, 1);

    // Write user attributes
    this->user_attributes->write_attributes(fd);
}



AWDSkeletonJoint *
AWDSkeleton::get_root_joint()
{
    return this->root_joint;
}


AWDSkeletonJoint *
AWDSkeleton::set_root_joint(AWDSkeletonJoint *joint)
{
    this->root_joint = joint;
    if (this->root_joint != NULL)
        this->root_joint->set_parent(NULL);

    return joint;
}

