#include "cache.h"
#include <stdlib.h>

BlockCache::BlockCache(void)
{
	firstItem = NULL;
	lastItem = NULL;
}


BlockCache::~BlockCache(void)
{
}

void BlockCache::Set(void *key, void *val)
{
	BlockCacheItem *item;

	item = (BlockCacheItem *)malloc(sizeof(BlockCacheItem));
	item->key = key;
	item->val = val;
	item->next = NULL;

	if (!firstItem) {
		firstItem = item;
	}
	else {
		lastItem->next = item;
	}

	lastItem = item;
}

void *BlockCache::Get(void *key)
{
	BlockCacheItem *cur;

	cur = firstItem;
	while (cur) {
		if (cur->key == key)
			return cur->val;

		cur = cur->next;
	}

	return NULL;
}