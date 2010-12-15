#include <stdlib.h>

#include "libawd.h"

void 
_awd_append_block(AWD_block_list *list, AWD_block *block)
{
    if (list->first_block == NULL) {
        list->first_block = block;
    }
    else {
        AWD_block *next;

        next = list->first_block;
        while (1) {
            if (next->next == NULL) {
                next->next = block;
                break;
            }
            else next = next->next;
        }
    }
}

awd_bool 
_awd_has_block(AWD_block_list *list, AWD_block *block)
{
    AWD_block *next;

    next = list->first_block;
    while (next) {
        if (next == block)
            return AWD_TRUE;

        next = next->next;
    }

    return AWD_FALSE;
}

awd_uint16 
_awd_get_block_id_by_data(AWD_block_list *list, void *data)
{
    AWD_block *block;

    block = _awd_get_block_by_data(list, data);
    if (block != NULL)
        return block->id;
    else return 0;
}

AWD_block *
_awd_get_block_by_data(AWD_block_list *list, void *data)
{
    if (list != NULL) {
        AWD_block *next;

        next = list->first_block;
        while (next) {
            if (next->data == data)
                return next;

            next = next->next;
        }
    }

    return AWD_NULL;
}

AWD_block *
_awd_create_block(AWD_block_type type, void *data)
{
    AWD_block *block;

    block = malloc(sizeof(AWD_block));
    block->type = type;
    block->data = data;
    block->next = NULL;

    return block;
}

AWD_block_list *
_awd_alloc_block_list()
{
    AWD_block_list *list;

    list = malloc(sizeof(AWD_block_list));
    list->first_block = NULL;

    return list;
}
