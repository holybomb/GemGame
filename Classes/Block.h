#pragma once
#define BLOCK_LABEL_TAG 10
#define PER_BLOCK_SCORE 100
#define ccb(__X__,__Y__) BlockPos::create(ccp((float)(__X__), (float)(__Y__)))
//#include "BlockPan.h"
#include "cocos2d.h"
USING_NS_CC;


class BlockPos : public CCObject
{
public:
	int x,y;
	CCPoint pos;
public:
	void setPos(CCPoint pTile,CCPoint pPos);
	static BlockPos* create(CCPoint pTile,CCPoint pPos);
};

class Block : public CCSprite
{
public:
	bool isSelected;
	bool isRemoved;
	bool isBomb;
	BlockPos * mBlockPos;
	int col;
	int blockType;
public:
	Block();
	~Block();
	static Block* create(int x,int y,int col);
	static Block* create(int x,int y,int col,bool isBomb);
	void setBlockPosTile(int x,int y);
	void setBlockPosTile(CCObject* point);
	void setSelected(bool select);
	void customInit(int x,int y,int col);
	void customInit(int x,int y,int col,bool isBomb);
	void blockRemove();
};