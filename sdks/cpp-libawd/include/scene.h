#ifndef _LIBAWD_SCENE_H
#define _LIBAWD_SCENE_H

#include "attr.h"
#include "name.h"
#include "block.h"
#include "awd_types.h"


class AWDSceneBlock : 
    public AWDBlock,
    public AWDNamedElement,
    public AWDAttrElement
{
    private:
        awd_float64 * transform_mtx;
        AWDSceneBlock *parent;
        AWDBlockList *children;

    protected:
        void write_scene_common(int);
        awd_uint32 calc_common_length();

    public:
        AWDSceneBlock(AWD_block_type, const char *, awd_uint16, awd_float64 *);
        ~AWDSceneBlock();

        void set_transform(awd_float64 *);

        AWDBlock *get_parent();
        void set_parent(AWDSceneBlock *);

        void add_child(AWDSceneBlock *);
        void remove_child(AWDSceneBlock *);

        AWDBlockIterator *child_iter();
};


class AWDContainer :
    public AWDSceneBlock
{
    private:

    protected:
        awd_uint32 calc_body_length(awd_bool);
        void write_body(int, awd_bool);

    public:
        AWDContainer(const char *, awd_uint16);
        ~AWDContainer();
};

#endif
