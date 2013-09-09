#include "BlockPan.h"
#include "GameDefine.h"
#include "Utils.h"


BlockPan::BlockPan(void)
{
}


BlockPan::~BlockPan(void)
{
}

bool BlockPan::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	CCLayer* layer= createGameLayer();
	addChild(layer);
	return true;
}

CCLayer* BlockPan::createGameLayer()
{
	CCLayer* mMainLayer = CCLayer::create();
	mMainLayer->setAnchorPoint(ccp(0,0));
	mMainLayer->setPosition(ccp(0,0));
	CCSprite* spriteBg = CCSprite::create(RESOURCE_PATH_CRYSTRAL("blocktiles.png"));
	sizeX = spriteBg->getContentSize().width;//w/GAME_LAYER_SIZE_W;
	sizeY = spriteBg->getContentSize().height;//h/GAME_LAYER_SIZE_H;
	spriteBg->release();
	CCSize size = CCSizeMake((sizeX*3/4)*BLOCK_PAN_SIZE_W+sizeX/4,sizeY*BLOCK_PAN_SIZE_H);
	this->setContentSize(size);
	mGameLayer = CCLayer::create();
	mGameLayer->setContentSize(size);
	mGameLayerBG = CCLayer::create();
	mGameLayerBG->setContentSize(size);
	for (int i =0;i<BLOCK_PAN_SIZE_W;i++)
	{
		int offH = 0;
		int maxH = BLOCK_PAN_SIZE_H;
		if(i%2!=0)
			maxH--;
		for(int j =0;j<maxH;j++)
		{
			CCSprite* spriteBg = CCSprite::create(RESOURCE_PATH_CRYSTRAL("blocktiles.png"));
			Block* sprite = createNewBlock(i,j,i);
			spriteBg->setPosition(sprite->getPosition());
			sprite->setVisible(false);
			mGameLayer->addChild(sprite);
			mGameLayerBG->addChild(spriteBg);
		}
	}
	mMainLayer->addChild(mGameLayerBG);
	mGameLayerBG->setAnchorPoint(ccp(0,0));
	mGameLayerBG->setPosition(ccp(0,0));
	mMainLayer->addChild(mGameLayer);
	mGameLayer->setAnchorPoint(ccp(0,0));
	mGameLayer->setPosition(ccp(0,0));
	return mMainLayer;
}

Block* BlockPan::createNewBlock(int x,int y,int col)
{
	CCPoint pos;
	int posX,posY;
	posX = x*(sizeX*3/4)+sizeX/2;
	posY = y*sizeY+sizeY/2;
	if(x%2!=0)
	{
		posY += sizeY/2;
	}
	pos = ccp(posX,posY);
	Block* newBlock = Block::create(x,y,col);
	newBlock->setPosition(pos);
	newBlock->setBlockPosTile(x,y);
	return newBlock;
}

Block* BlockPan::findBlockByPos(int x,int y)
{
	int index = 0;
	CCArray* blocks = mGameLayer->getChildren();
	CCObject* obj = NULL;
	CCARRAY_FOREACH(blocks,obj)
	{
		Block* block = (Block*)obj;
		if (block->mBlockPos->x==x && block->mBlockPos->y==y)
		{
			return block;
		}
	}
	return NULL;
}

Block* BlockPan::findBlockByTouch(CCTouch* pTouch)
{
	int index = 0;
	CCArray* blocks = mGameLayer->getChildren();
	CCObject* obj = NULL;
	CCARRAY_FOREACH(blocks,obj)
	{
		Block* block = (Block*)obj;
		CCRect rc = Utils::getRect(block);
		CCPoint touchPoint = block->convertTouchToNodeSpace(pTouch);
		if(rc.containsPoint(touchPoint))
		{
			return block;
		}
	}
	return NULL;
}
void BlockPan::showGameEnd()
{
	if(mGameLayer)
	{
		CCObject* obj = NULL;
		CCArray* blocks = mGameLayer->getChildren();
		srand(time(new time_t));
		CCARRAY_FOREACH(blocks,obj)
		{
			Block* block = (Block*)obj;
			float offBlockX = CCRANDOM_MINUS1_1()*320.0f;
			float len = CCRANDOM_0_1()*200.0f;
			float time = CCRANDOM_0_1()+0.6;
			ccBezierConfig bezier;
			bezier.controlPoint_1 = block->getPosition();
			int targetX = block->getPositionX()+offBlockX;
			CCLOG("offx = %f",offBlockX);
			bezier.controlPoint_2 = ccp(block->getPositionX()+(targetX-block->getPositionX())/2,block->getPositionY()+len);
			bezier.endPosition = ccp(targetX,block->getPositionY()-1136);
			CCBezierTo* bez = CCBezierTo::create(time,bezier);
			//CCEaseSineIn * eff = CCEaseSineIn ::create(bez);
			CCJumpTo* eff = CCJumpTo::create(time,ccp(targetX,block->getPositionY()-1136),500.0f,1);
			block->runAction(CCSequence::create(eff,NULL));
		}
	}
}