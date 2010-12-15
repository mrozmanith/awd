#ifndef _BLOCKS_H
#define _BLOCKS_H

void            _awd_append_block(AWD_block_list *, AWD_block *);
AWD_block_list  *_awd_alloc_block_list();
awd_uint16      _awd_get_block_id_by_data(AWD_block_list *, void *);
AWD_block       *_awd_get_block_by_data(AWD_block_list *, void *);
AWD_block       *_awd_create_block(AWD_block_type, void *);

#endif
