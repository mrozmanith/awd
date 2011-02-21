#include "libawd.h"
#ifdef WIN32
#include "awdw32.h"
#else
#include <unistd.h>
#endif

AWDBlock::AWDBlock()
{
    this->next = NULL;
}


void
AWDBlock::prepare_write()
{
    // Does nothing by default. Can be optionally
    // overriden by sub-classes to take any actions
    // that need to happen before length is calculated
}

size_t
AWDBlock::write_block(int fd, awd_bool wide, awd_baddr addr)
{
    awd_uint8 ns_addr;
    awd_uint32 length;
    awd_uint32 length_be;
    awd_baddr block_addr_be;

    this->addr = addr;

    this->prepare_write();
    length = this->calc_body_length(wide);

    //TODO: Get addr of actual namespace
    ns_addr = 0;

    // Convert to big-endian if necessary
    block_addr_be = UI32(this->addr);
    length_be = UI32(length);

    // Write header
    write(fd, &block_addr_be, sizeof(awd_baddr));
    write(fd, &ns_addr, sizeof(awd_uint8));
    write(fd, &this->type, sizeof(awd_uint8));
    write(fd, &length_be, sizeof(awd_uint32));

    // Write body using concrete implementation
    // in block sub-classes
    this->write_body(fd, wide);


    return (size_t)length + 10;
}



awd_baddr
AWDBlock::get_addr()
{
    return this->addr;
}




AWDBlockList::AWDBlockList()
{
    this->first_block = NULL;
    this->last_block = NULL;
    this->num_blocks = 0;
}


void 
AWDBlockList::append(AWDBlock *block)
{
    if (this->first_block == NULL) {
        this->first_block = block;
    }
    else {
        this->last_block->next = block;
    }

    this->last_block = block;
    this->num_blocks++;
}


awd_bool
AWDBlockList::contains(AWDBlock *block)
{
    AWDBlock *cur;

    cur = this->first_block;
    while (cur) {
        if (cur == block)
            return AWD_TRUE;

        cur = cur->next;
    }

    return AWD_FALSE;
}





AWDBlockIterator::AWDBlockIterator(AWDBlockList *list)
{
    this->list = list;
    this->cur_block = NULL;
    this->reset();
}


void
AWDBlockIterator::reset()
{
    if (this->list != NULL)
        this->cur_block = this->list->first_block;
}

AWDBlock *
AWDBlockIterator::next()
{
    AWDBlock *tmp;

    tmp = this->cur_block;
    if (this->cur_block != NULL)
        this->cur_block = this->cur_block->next;

    return tmp;
}
