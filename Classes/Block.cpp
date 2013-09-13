#include "Block.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Utils.h"
#include "GameDefine.h"
USING_NS_CC_EXT;
Block* Block::create(int x,int y,int col)
{
	return Block::create(x,y,col,false);
}
Block* Block::create(int x,int y,int col,bool isBomb)
{
	Block *testSprite= new Block();
	if(testSprite&&testSprite->init()){
		testSprite->autorelease();
		testSprite->customInit(x,y,col,isBomb);//
		return testSprite;
	}
	CC_SAFE_DELETE(testSprite);
	return testSprite;
}
void Block::customInit(int x,int y,int col)
{
	customInit(x,y,col,false);
}
void Block::customInit(int x,int y,int col,bool mIsBomb)
{
	this->isBomb = mIsBomb;
	if (mIsBomb)
	{
		blockType = -2;
		initWithFile(RESOURCE_PATH_CRYSTRAL("bomb.png"));
		CCSprite* hi = CCSprite::create(RESOURCE_PATH_CRYSTRAL("bomb-hi.png"));
		hi->setPosition(getContentSize()/2);
		addChild(hi);
		CCFadeOut* act = CCFadeOut::create(1);
		CCSequence* eff = CCSequence::create(act,act->reverse(),NULL);
		CCRepeatForever* rep = CCRepeatForever::create(eff);
		hi->runAction(rep);
		CCLOG("bomb create");
	}
	else
	{
		int start = 0;
		int end = 4;
		float rnd = CCRANDOM_0_1();
		int index = rnd*end+start;
		CCString* file = CCString::createWithFormat("%i.png",index);
		blockType = index;
		initWithFile(RESOURCE_PATH_CRYSTRAL(file->getCString()));
		CCLOG("block create");
	}
	mBlockPos = BlockPos::create(ccp(x,y),getPosition());
	this->col = col;
//	this->addChild(pBgSprite,-1,0);
}

Block::Block()
{
	isSelected = false;
	isRemoved = false;
}

Block::~Block()
{

}

void Block::blockRemove()
{
	if(!isRemoved)
	{
		//BlockPan* pan = (BlockPan*)getParent();
		//if(pan)
		//CCParticleSystem *meteor=CCParticleSystemQuad::create("particles/taken-gem.plist
		{
			this->removeFromParentAndCleanup(true);
			//block->runAction(CCCallFunc::create(this,callfunc_selector(Block::removeFromParent)));
		}
		isRemoved = true;
	}
}

void Block::setBlockPosTile( int x,int y )
{
	mBlockPos->setPos(ccp(x,y),getPosition());
}

void Block::setBlockPosTile(CCObject* obj)
{
	BlockPos* p = (BlockPos*)obj;
	setBlockPosTile(p->x,p->y);
}

void Block::setSelected( bool select )
{
	isSelected = select;
	setOpacity(select?150:255);
}


BlockPos* BlockPos::create(CCPoint pTile,CCPoint pPos)
{
	BlockPos* pos = new BlockPos();
	if(pos){
		pos->autorelease();
		pos->x = pTile.x;
		pos->y = pTile.y;
		pos->pos = pPos;
		pos->retain();
		return pos;
	}
	CC_SAFE_DELETE(pos);

	return pos;

}

void BlockPos::setPos(  CCPoint pTile,CCPoint pPos)
{
	this->x = pTile.x;
	this->y = pTile.y;
	pos = pPos;
}
