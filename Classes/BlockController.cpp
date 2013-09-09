#include "BlockController.h"
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
}
