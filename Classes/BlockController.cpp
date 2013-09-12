#include "BlockController.h"
#include "GameScene.h"
#include "GameDefine.h"
static BlockController* controller = NULL;
BlockController* BlockController::shareData()
{
	if (!controller)
	{
		controller = new BlockController();
		controller->init();
	}
	return controller;
}

void BlockController::init()
{
	mStartFlip = false;
	mCurType = -1;
	selectBlock = CCArray::create();
	selectBlock->retain();
	movedBlock = CCArray::create();
	movedBlock->retain();
	hintBlocks = CCArray::create();
	hintBlocks->retain();
	hintSprites = CCArray::create();
	hintSprites->retain();
	bombSprite = NULL;
}

void BlockController::update( float dt )
{
	switch (mMainState)
	{
	case BLOCK_MAIN_STATE_INIT:
		{
			switch(mSubState)
			{
				case BLOCK_SUB_STATE_INIT_CREATE:
					GameScene::shareGameScene()->timeBoard = GameScene::shareGameScene()->showTimerBoarder();
					GameScene::shareGameScene()->timeBoard->setPosition(ccp(0,DESIGN_SCREEN_SIZE_H+158));
					GameScene::shareGameScene()->timeBoard->runAction(CCMoveTo::create(0.3f,ccp(0,DESIGN_SCREEN_SIZE_H-158)));
					GameScene::shareGameScene()->addChild(GameScene::shareGameScene()->timeBoard,10);
					setSubState(BLOCK_SUB_STATE_INIT_CREATEING);
					break;
				case BLOCK_SUB_STATE_INIT_SHOW_GO:
					break;
			}
		}
		break;
	case BLOCK_MAIN_STATE_RUN:
		{
			switch(mSubState)
			{
			case BLOCK_SUB_STATE_RUN_WAIT_CHOOSE:
				break;
			case BLOCK_SUB_STATE_RUN_CHECK:
				break;
			case BLOCK_SUB_STATE_RUN_REMOVE_CREATE:
				break;
			}
		}
		break;
	case BLOCK_MAIN_STATE_END:
		{
			switch(mSubState)
			{
			case BLOCK_SUB_STATE_END_SHOW_ANIM:
				GameScene::shareGameScene()->showGameEnd();
				break;
			case BLOCK_SUB_STATE_END_SHOW_END:
				break;
			}
		}
		break;
	}
}

void BlockController::setState( int mainState,int subState )
{
	mMainState = mainState;
	mSubState = subState;
}

void BlockController::setSubState(int subState )
{
	mSubState = subState;
}

void BlockController::resetSelect()
{
	CCObject* obj= NULL;
	CCARRAY_FOREACH(selectBlock,obj)
	{
		Block* block = (Block*)obj;
		block->setSelected(false);
	}
}
