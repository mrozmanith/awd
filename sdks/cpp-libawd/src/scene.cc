
#include "scene.h"
#include "util.h"

#ifdef WIN32
#include "awdw32.h"
#else
#include <unistd.h>
#endif


AWDSceneBlock::AWDSceneBlock(AWD_block_type type, const char *name, awd_uint16 name_len, awd_float64 *mtx) :
    AWDBlock(type),
    AWDNamedElement(name, name_len),
    AWDAttrElement()
{
    this->parent = NULL;
    this->children = new AWDBlockList();

    if (mtx == NULL);
        mtx = awdutil_id_mtx4(NULL);
    this->set_transform(mtx);
}


AWDSceneBlock::~AWDSceneBlock()
{
    if (this->transform_mtx) {
        free(this->transform_mtx);
        this->transform_mtx = NULL;
    }
}



void
AWDSceneBlock::write_scene_common(int fd)
{
    awd_baddr parent_addr;

    // Get IDs for references, verify byte-order
    parent_addr = 0;
    if (this->parent != NULL)
        parent_addr = UI32(this->parent->get_addr());

    // Write scene block common fields
    // TODO: Move this to separate base class
    write(fd, &parent_addr, sizeof(awd_baddr));
    awdutil_write_mtx4(fd, this->transform_mtx);
    awdutil_write_varstr(fd, this->get_name(), this->get_name_length());
}


awd_uint32
AWDSceneBlock::calc_common_length()
{
    return 132 + sizeof(awd_uint16) + this->get_name_length();
}


void
AWDSceneBlock::set_transform(awd_float64 *mtx)
{
    this->transform_mtx = mtx;
}


AWDBlock *
AWDSceneBlock::get_parent()
{
    return this->parent;
}

void
AWDSceneBlock::set_parent(AWDSceneBlock *parent)
{
    if (parent != this->parent) {
        this->parent = parent;
        if (this->parent) {
            this->parent->add_child(this);
        }
    }
}



void
AWDSceneBlock::add_child(AWDSceneBlock *child)
{
    if (child && !this->children->contains(child)) {
        this->children->append(child);
        child->set_parent(this);
    }
}


void
AWDSceneBlock::remove_child(AWDSceneBlock *child)
{
    // TODO: Implement remove() in BlockList
    //this->children->remove(child);
}



AWDBlockIterator *
AWDSceneBlock::child_iter()
{
    return new AWDBlockIterator(this->children);
}






AWDContainer::AWDContainer(const char *name, awd_uint16 name_len) :
    AWDSceneBlock(CONTAINER, name, name_len, NULL)
{
}


AWDContainer::~AWDContainer()
{
}


awd_uint32
AWDContainer::calc_body_length(awd_bool wide)
{
    return this->calc_common_length() + this->calc_attr_length(true,true);
}


void
AWDContainer::write_body(int fd, awd_bool wide)
{
    this->write_scene_common(fd);
    this->properties->write_attributes(fd);
    this->user_attributes->write_attributes(fd);
}


