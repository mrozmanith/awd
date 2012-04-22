#include "cache.h"
#include <stdlib.h>

BlockCache::BlockCache(void)
{
	firstItem = NULL;
	lastItem = NULL;
}


BlockCache::~BlockCache(void)
{
	// TODO: Free all items
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



ColorMaterialCache::ColorMaterialCache(void)
{
	firstItem = NULL;
	lastItem = NULL;
}


ColorMaterialCache::~ColorMaterialCache(void)
{
	// TODO: Free all items
}

void ColorMaterialCache::Set(awd_color color, AWDMaterial *mtl)
{
	ColorMaterialCacheItem *item;

	item = (ColorMaterialCacheItem *)malloc(sizeof(ColorMaterialCacheItem));
	item->color = color;
	item->mtl = mtl;
	item->next = NULL;

	if (!firstItem) {
		firstItem = item;
	}
	else {
		lastItem->next = item;
	}

	lastItem = item;
}

AWDMaterial *ColorMaterialCache::Get(awd_color color)
{
	ColorMaterialCacheItem *cur;

	cur = firstItem;
	while (cur) {
		if (cur->color == color)
			return cur->mtl;

		cur = cur->next;
	}

	return NULL;
}




SkeletonCacheItem::SkeletonCacheItem(INode *maxRootBone)
{
	next = NULL;
	rootBone = maxRootBone;
	firstJoint = NULL;
	lastJoint = NULL;
	numJoints = 0;

	// Create AWD skeleton structure and begin
	// gathering joints recursively
	char *name = rootBone->GetName();
	awdSkel = new AWDSkeleton(name, strlen(name));
	GatherJoint(rootBone, NULL);
}


SkeletonCacheItem::~SkeletonCacheItem(void)
{
	// TODO: Delete all joints
}


void SkeletonCacheItem::AppendCacheJoint(SkeletonCacheJoint *cacheJoint)
{
	if (!firstJoint) {
		firstJoint = cacheJoint;
	}
	else {
		lastJoint->next = cacheJoint;
	}

	lastJoint = cacheJoint;
	lastJoint->index = numJoints++;
	lastJoint->next = NULL;
}

/**
 * Gather joints using the same order that the AWD SDK uses, so that the
 * binding code can then look-up indices using this structure.
*/
void SkeletonCacheItem::GatherJoint(INode *bone, AWDSkeletonJoint *awdParent)
{
	int i;
	char *name;
	SkeletonCacheJoint *cacheJoint;
    AWDSkeletonJoint *awdJoint;

    name = bone->GetName();
    awdJoint = new AWDSkeletonJoint(name, strlen(name), NULL);

    if (awdParent != NULL) {
		awdParent->add_child_joint(awdJoint);
    }
    else {
        awdSkel->set_root_joint(awdJoint);
    }

	// Add to cache
	cacheJoint = (SkeletonCacheJoint*)malloc(sizeof(SkeletonCacheJoint));
	cacheJoint->awdJoint = awdJoint;
	cacheJoint->maxBone = bone;
	AppendCacheJoint(cacheJoint);

	for (i=0; i<bone->NumberOfChildren(); i++) {
		INode *child = bone->GetChildNode(i);
		Object *obj = bone->GetObjectRef();
		if (obj && obj->ClassID()==BONE_OBJ_CLASSID) {
			GatherJoint(child, awdJoint);
		}
	}
}


SkeletonCache::SkeletonCache(void)
{
	firstItem = NULL;
	lastItem = NULL;
	cur = NULL;
}


SkeletonCache::~SkeletonCache(void)
{
	// TODO: Free all items
}


AWDSkeleton *SkeletonCache::Add(INode *rootBone)
{
	SkeletonCacheItem *item;

	item = new SkeletonCacheItem(rootBone);

	if (!firstItem) {
		firstItem = item;
	}
	else {
		lastItem->next = item;
	}

	lastItem = item;

	// Return skeleton
	return item->awdSkel;
}


bool SkeletonCache::HasItems()
{
	return (firstItem != NULL);
}


void SkeletonCache::IterReset()
{
	cur = firstItem;
}


SkeletonCacheItem *SkeletonCache::IterNext()
{
	// Stop if end was reached
	if (!cur) 
		return NULL;

	SkeletonCacheItem *ret = cur;
	cur = cur->next;
	return ret;
}