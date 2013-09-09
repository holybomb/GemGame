#pragma once
#include "cocos2d.h"
#include "GameDefine.h"
USING_NS_CC;

class BlockController
{
public:
	int mCurType;
	bool mStartFlip;
	CCArray* selectBlock;
	CCArray* movedBlock;
public:
	static BlockController* shareData();
	void init();
};