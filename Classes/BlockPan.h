#pragma once
#include "cocos2d.h"
#include "Block.h"
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
};

