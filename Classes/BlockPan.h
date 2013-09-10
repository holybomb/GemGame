#pragma once
#include "cocos2d.h"
#include "Block.h"
#define BLOCK_FOREACH_CHILD(__LAYER__) 	CCArray* blocks = (__LAYER__)->getChildren();	CCObject* obj = NULL; CCARRAY_FOREACH(blocks,obj)
USING_NS_CC;
class BlockPan:public CCLayer
{
public:
	CCLayer* mGameLayerBG;
	CCLayer* mGameLayer;
	int sizeX;
	int sizeY;
public:
	BlockPan(void);
	~BlockPan(void);
	bool init();
	CREATE_FUNC(BlockPan);
	CCLayer* createGameLayer();
	Block* createNewBlock(int x,int y,int col);
	Block* findBlockByPos(int x,int y);
	void showGameEnd();
	Block* findBlockByTouch(CCTouch* pTouch);
	void showDebugTxt();
	int findLastLineByCol(int col);
};

