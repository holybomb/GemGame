#pragma once
#include "cocos2d.h"
#include "GameDefine.h"
USING_NS_CC;
#define BLOCK_MAIN_STATE_INIT 1
#define BLOCK_MAIN_STATE_RUN 2
#define BLOCK_MAIN_STATE_END 3

#define BLOCK_SUB_STATE_INIT_CREATE 100
#define BLOCK_SUB_STATE_INIT_CREATEING 101
#define BLOCK_SUB_STATE_INIT_SHOW_GO 102
#define BLOCK_SUB_STATE_INIT_SHOW_GOINT 103

#define BLOCK_SUB_STATE_RUN_WAIT_CHOOSE 200
#define BLOCK_SUB_STATE_RUN_CHECK 201
#define BLOCK_SUB_STATE_RUN_REMOVE_CREATE 202

#define BLOCK_SUB_STATE_END_SHOW_ANIM 300
#define BLOCK_SUB_STATE_END_SHOW_END 301

class BlockController
{
public:
	int mCurType;
	bool mStartFlip;
	CCArray* selectBlock;
	CCArray* movedBlock;

	int mMainState;
	int mSubState;
	CCArray* hintBlocks;
	CCArray* hintSprites;
public:
	static BlockController* shareData();
	void init();
	void update(float dt);
	void setState(int mainState,int subState);
	void setSubState(int subState );
	void resetSelect();
};