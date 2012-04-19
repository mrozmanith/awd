#pragma once

typedef struct BlockCacheItem_struct {
	void *key;
	void *val;
	BlockCacheItem_struct *next;
} BlockCacheItem;

class BlockCache
{
private:
	BlockCacheItem *firstItem;
	BlockCacheItem *lastItem;

public:
	BlockCache(void);
	~BlockCache(void);

	void Set(void *key, void *val);
	void *Get(void *key);
};

