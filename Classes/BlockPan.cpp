#include "BlockPan.h"
#include "GameDefine.h"
#include "Utils.h"
#include "BlockController.h"


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
void BlockPan::showDebugTxt()
{
	if (!CCDirector::sharedDirector()->isDisplayStats())
	{
		return;
	}
	/*BLOCK_FOREACH_CHILD(mGameLayer)*/
	CCArray* blocks = mGameLayer->getChildren();
	blocks->retain();
	CCObject* obj = NULL;
	CCARRAY_FOREACH(blocks,obj)
	{
		Block* block = (Block*)obj;
		if(!block)
			continue;
		int x = block->mBlockPos->x;
		int y = block->mBlockPos->y;
		CCLabelTTF* txt = CCLabelTTF::create(CCString::createWithFormat("%d,%d",x,y)->getCString(),
			"",50);
		txt->setColor(ccc3(255,0,0));
		txt->setHorizontalAlignment(kCCTextAlignmentCenter);
		txt->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		CCLayerColor* layer = CCLayerColor::create(ccc4(255,255,255,100));
		layer->addChild(txt);
		layer->setAnchorPoint(ccp(0,0));
		CCRect rect = Utils::getRect(block,mGameLayer);
		layer->setPosition(ccp(rect.origin.x,rect.origin.y));
		layer->setContentSize(block->getContentSize());
		txt->setPosition(ccp(layer->getContentSize().width/2,layer->getContentSize().height/2));
		this->addChild(layer,10);
	}
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

int BlockPan::findLastLineByCol(int col)
{
	int line = 0;
	CCArray* blocks = mGameLayer->getChildren();
	CCObject* obj = NULL;
	CCARRAY_FOREACH(blocks,obj)
	{
		Block* block = (Block*)obj;
		if (block->mBlockPos->x==col)
		{
			if (line<block->mBlockPos->y)
			{
				line = block->mBlockPos->y;
			}
		}
	}
	return line;
}

Block* BlockPan::findBlockByTouch(CCTouch* pTouch)
{
	int index = 0;
	CCArray* blocks = mGameLayer->getChildren();
	CCObject* obj = NULL;
	CCARRAY_FOREACH(blocks,obj)
	{
		Block* block = (Block*)obj;
		CCRect rect = Utils::getRect(block,mGameLayer);
		CCPoint touchPoint = mGameLayer->convertTouchToNodeSpaceAR(pTouch);
		if(rect.containsPoint(touchPoint))
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

void BlockPan::CheckForHint()
{
	BlockController::shareData()->hintBlocks->removeAllObjects();
	BLOCK_FOREACH_CHILD(mGameLayer)
	{
		Block* block = (Block*)obj;
		int findNum = 0;
		if(BlockController::shareData()->hintBlocks->count()==0)
		{
			BlockController::shareData()->hintBlocks->addObject(block);
		}
		Block* target = NULL;
		Block* centre = block;
		do 
		{
			target = findBlockByCentre(centre,BlockController::shareData()->hintBlocks);
			if(BlockController::shareData()->hintBlocks->containsObject(target))
			{
				target = NULL;
			}
			if(target)
			{
				BlockController::shareData()->hintBlocks->addObject(target);
				centre = target;
				if(BlockController::shareData()->hintBlocks->count()>2)
					break;
			}
		} while (target !=NULL);
		if(BlockController::shareData()->hintBlocks->count()>2)
		{
			break;
		}
		else
		{
			BlockController::shareData()->hintBlocks->removeAllObjects();
		}
	}
	if(BlockController::shareData()->hintBlocks->count()==0)
		return;
	BlockController::shareData()->hintSprites->removeAllObjects();
	CCARRAY_FOREACH(BlockController::shareData()->hintBlocks,obj)
	{
		Block* block = (Block*) obj;
		CCSprite* hint = CCSprite::create(RESOURCE_PATH_CRYSTRAL("hint.png"));
		hint->setPosition(block->getPosition());
		this->addChild(hint);
		CCFadeOut* eff = CCFadeOut::create(0.5f);
		CCRepeatForever* act = CCRepeatForever::create(eff);
		hint->runAction(act);
		BlockController::shareData()->hintSprites->addObject(hint);
	}

}
Block* BlockPan::findBlockByCentre(Block* centre,CCArray* ignorBlocks)
{
	int startLine = centre->mBlockPos->y-1;
	int endLine = centre->mBlockPos->y+1;
	int startCol = centre->mBlockPos->x-1;
	int endCol = centre->mBlockPos->x+1;
	for(int i = startLine;i<=endLine;i++)
	{
		if( i<0 || i== BLOCK_PAN_SIZE_H) continue;
		for(int j = startCol;j<=endCol;j++)
		{
			if( j<0 || j== BLOCK_PAN_SIZE_W) continue;
			Block* target = findBlockByPos(j,i);

			if(	(centre->mBlockPos->x%2==0) 
				&& abs(centre->mBlockPos->x - target->mBlockPos->x)==1 
				&& (target->mBlockPos->y > centre->mBlockPos->y)
				)
				continue;
			if(target->blockType == centre->blockType && target!=centre)
			{
				if(!ignorBlocks->containsObject(target))
					return target;
			}
		}
	}
	return NULL;
}
