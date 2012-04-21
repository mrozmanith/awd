#pragma once

#include "Max.h"
#include "awd/awd.h"

/**
 * BlockCache
 * Cache that is used generically to store any type of block or 
 * sub-element in a look-up table by MAX object.
*/
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


/**
 * ColorMaterialCache
 * Cache that is used for "default" color materials applied to blocks
 * that have no material assigned. AWDMaterial blocks are stored in a
 * dictionary cache by color.
*/
typedef struct ColorMaterialCacheItem_struct {
	awd_color color;
	AWDMaterial *mtl;
	ColorMaterialCacheItem_struct *next;
} ColorMaterialCacheItem;

class ColorMaterialCache
{
private:
	ColorMaterialCacheItem *firstItem;
	ColorMaterialCacheItem *lastItem;

public:
	ColorMaterialCache(void);
	~ColorMaterialCache(void);

	void Set(awd_color color, AWDMaterial *mtl);
	AWDMaterial *Get(awd_color color);
};



/**
 * SkeletonCache
 * Used by animation export to look up skeletons that need to be sampled
 * for animation export.
*/
typedef struct SkeletonCacheItem_struct {
	AWDSkeleton *awdSkel;
	INode *maxRootBone;
	struct SkeletonCacheItem_struct *next;
} SkeletonCacheItem;

class SkeletonCache
{
private:
	SkeletonCacheItem *cur;
	SkeletonCacheItem *firstItem;
	SkeletonCacheItem *lastItem;

public:
	SkeletonCache(void);
	~SkeletonCache(void);

	void Add(AWDSkeleton *awdSkel, INode *maxRootBone);
	bool HasItems();
	void IterReset();
	SkeletonCacheItem *IterNext();
};
