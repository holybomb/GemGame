﻿#include "GameScene.h"
#include "GameData.h"
#include "MainMenuScene.h"
USING_NS_CC;
USING_NS_CC_EXT;
static GameScene* gameScene = NULL;
GameScene* GameScene::shareGameScene()
{
	if (!gameScene)
	{
		gameScene = new GameScene();
	}
	return gameScene;
}
CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    // 'layer' is an autorelease object
	gameScene = GameScene::create();

    // add layer as a child to scene
    scene->addChild(gameScene);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(RESOURCE_PATH_AUDIO("loop.mp3"),true);
	GameData::shareData()->init();
	mTime = -4;
	mSelectBlock = NULL;
	CCSprite* bgSprite = CCSprite::create(RESOURCE_PATH_GAME("background.png"));
	bgSprite->setAnchorPoint(ccp(0.0f,0.0f));
    this->addChild(bgSprite);
	gameLayer = BlockPan::create();
	addChild(gameLayer);
	gameLayer->setAnchorPoint(ccp(0,0));
	gameLayer->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2-gameLayer->getContentSize().width/2,10));

	timeBoard = showTimerBoarder();
	timeBoard->setPosition(ccp(0,DESIGN_SCREEN_SIZE_H+158));
	timeBoard->runAction(CCMoveTo::create(0.3f,ccp(0,DESIGN_SCREEN_SIZE_H-158)));
	addChild(timeBoard,10);
    return true;
}
void GameScene::countTime(float dt)
{
	mTime++;
	CCLOG("countTime = %d",mTime);
	if (mTime == -3)
	{
		float totalInitTime = 0;
		for(int j = 0;j<BLOCK_PAN_SIZE_H;j++)
		{
			for (int i= 0;i<BLOCK_PAN_SIZE_W;i++)
			{
				Block* block = gameLayer->findBlockByPos(i,j);
				if(block)
				{
					block->setVisible(true);
					block->setPosition(ccp(block->getPositionX(),1200));
					CCMoveTo* moveTo = CCMoveTo::create(0.5f,block->mBlockPos->pos);
					CCEaseSineInOut * easeIn = CCEaseSineInOut::create(moveTo);
					block->runAction(CCSequence::create(CCDelayTime::create(0.1f*j),easeIn,NULL));
				}
			}
			totalInitTime+=0.5f+0.15f*j;
		}
	}
	if(mTime == -1)
	{
		showGo();

	}
	if(mTime==0)
	{
		gameLayer->showDebugTxt();
		pr->setPercentage(0);
		CCProgressTo* to = CCProgressTo::create(TOTAL_GAME_TIME,100);
		pr->setReverseProgress(true);
		pr->runAction(to);
		mFreeTime = 0;
		schedule(schedule_selector(GameScene::updateThinkTimer),1);
		gameLayer->schedule(schedule_selector(BlockPan::createStarSprite),0.5f);
	}
	
	if (mTime==TOTAL_GAME_TIME*80/100)
	{
		if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(false);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(RESOURCE_PATH_AUDIO("timer.wav"),true);
	}
	if (mTime>=TOTAL_GAME_TIME*80/100)
	{
		pr->runAction(CCBlink::create(0.3f,4));
	}
	if (mTime>=TOTAL_GAME_TIME)
	{
		showGameEnd();
		unschedule(schedule_selector(GameScene::countTime));
	}
}

void GameScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void GameScene::restartScene( CCObject* pSender )
{
	CCObject* obj;
	mFreeTime = -1;
	CCARRAY_FOREACH(BlockController::shareData()->hintSprites,obj)
	{
		CCNode* block = (CCNode*)obj;
		block->removeFromParent();
	}
	gameLayer->unschedule(schedule_selector(BlockPan::createStarSprite));
	gameLayer->mGameLayerEffect->removeAllChildren();
	gameLayer->showGameEnd();
	timeBoard->runAction(CCMoveTo::create(0.3f,ccp(0,DESIGN_SCREEN_SIZE_H+158)));
	this->runAction(CCSequence::create(CCDelayTime::create(1.5f),CCCallFunc::create(this,callfunc_selector(GameScene::backToMainMenu)),NULL));
	SimpleAudioEngine::sharedEngine()->preloadEffect( RESOURCE_PATH_AUDIO("click.wav"));
}
void GameScene::backToMainMenu(CCObject* obj)
{
	CCDirector::sharedDirector()->sharedDirector()->replaceScene(MainScene::scene());
}

void GameScene::backToMainMenu()
{
	backToMainMenu(NULL);
}

CCLayer* GameScene::showTimerBoarder()
{
	CCLayer* board = CCLayer::create();
	CCSprite* boardBG = CCSprite::create(RESOURCE_PATH_GAME("header.png"));
	boardBG->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,0));
	boardBG->setAnchorPoint(ccp(0.5f,0));
	board->addChild(boardBG);
	CCMenuItemImage* imageItem = CCMenuItemImage::create(RESOURCE_PATH_GAME("btn-pause.png"),RESOURCE_PATH_GAME("btn-pause-down.png"), this,menu_selector(GameScene::restartScene));
	imageItem->setAnchorPoint(ccp(0,1));
	CCMenu* menu = CCMenu::create(imageItem,NULL);
	menu->setAnchorPoint(ccp(0,1));
	menu->setPosition(ccp(0,158));
	pr = CCProgressTimer::create(CCSprite::create(RESOURCE_PATH_GAME("timer.png")));
	pr->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,16));
	pr->setAnchorPoint(ccp(0.5,0));
	pr->setPercentage(1.0f);
	pr->setType(kCCProgressTimerTypeBar);
	pr->setMidpoint(ccp(1,0));
	pr->setBarChangeRate(ccp(1,0));
	this->schedule(schedule_selector(GameScene::countTime),1.0f);

	scoreTxt = CCLabelBMFont::create("0",RESOURCE_PATH_FONT("scorefont.fnt"));
	scoreTxt->setAnchorPoint(ccp(1,0.5f));
	scoreTxt->setScale(1.0f);
	scoreTxt->setAlignment(kCCTextAlignmentRight);
	scoreTxt->setPosition(ccp(DESIGN_SCREEN_SIZE_W,110));
	board->addChild(scoreTxt,10);
	board->addChild(pr);
	board->addChild(menu);
	return board;
}
void GameScene::showGameEnd()
{
	CCObject* obj;
	mFreeTime = -1;
	CCARRAY_FOREACH(BlockController::shareData()->hintSprites,obj)
	{
		CCNode* block = (CCNode*)obj;
		block->removeFromParent();
	}
	gameLayer->unschedule(schedule_selector(BlockPan::createStarSprite));
	gameLayer->mGameLayerEffect->removeAllChildren();

	gameLayer->showGameEnd();
	timeBoard->runAction(CCMoveTo::create(0.3f,ccp(0,DESIGN_SCREEN_SIZE_H+158)));
	CCLayer* gameEndLayer = CCLayer::create();
	CCSprite* bgSprite = CCSprite::create(RESOURCE_PATH_MENU("about-fade.png"));
	scoreTxt = CCLabelBMFont::create(GameData::shareData()->getScoreString(),RESOURCE_PATH_FONT("scorefont.fnt"));
	scoreTxt->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,700));
	scoreTxt->setScale(0.0f);
	CCScaleTo* scaleTo = CCScaleTo::create(0.9f,2.5f);
	CCEaseElasticInOut* scoreEff = CCEaseElasticInOut::create(scaleTo);
	scoreTxt->runAction(scoreEff);
	if(!scoreTxt->getParent())
		gameEndLayer->addChild(scoreTxt);
	bgSprite->setAnchorPoint(ccp(0,0));
	gameEndLayer->addChild(bgSprite);
	CCLabelTTF* txt = CCLabelTTF::create("Game End",RESOURCE_PATH_FONT("Marker Felt.ttf"),40.0f);
	txt->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,DESIGN_SCREEN_SIZE_H/2));
	gameEndLayer->addChild(txt);
	CCMenuItemImage* imageItem = CCMenuItemImage::create("CloseNormal.png","CLoseSelected.png", this,menu_selector(GameScene::backToMainMenu));
	CCMenu* menu = CCMenu::create(imageItem,NULL);
	menu->setAnchorPoint(ccp(0,0));
	menu->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,300));
	imageItem->setScale(3.0f);
	gameEndLayer->addChild(menu);
	this->addChild(gameEndLayer,100,this->getChildrenCount());
	if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(false);
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(RESOURCE_PATH_AUDIO("endgame.wav"));
}
void GameScene::updateScore(float dt)
{
	scoreTxt->setCString(CCString::createWithFormat("%d",GameData::shareData()->getScore())->getCString());
}
GameScene::GameScene()
{

}

void GameScene::showGo()
{
	CCSprite* goSprite = CCSprite::create(RESOURCE_PATH_GAME("go.png"));
	goSprite->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,DESIGN_SCREEN_SIZE_H/2));
	this->addChild(goSprite);
	CCScaleTo* scaleBig = CCScaleTo::create(0.5f,1);
	CCScaleTo* scaleSmall = CCScaleTo::create(0.0001f,0);
	CCEaseBounceOut* eff = CCEaseBounceOut::create(scaleBig);
	eff->bounceTime(3);
	CCSequence* act = CCSequence::create(CCHide::create(),scaleSmall,CCShow::create(),eff,CCDelayTime::create(0.2f),CCScaleTo::create(0.5f,0),CCRemoveSelf::create(),NULL);
	goSprite->setVisible(false);
	goSprite->runAction(act);
	setTouchEnabled(true);
}


void GameScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-1,false);
}


bool GameScene::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	CCObject* obj;
	mFreeTime = -1;
	CCARRAY_FOREACH(BlockController::shareData()->hintSprites,obj)
	{
		CCNode* block = (CCNode*)obj;
		block->removeFromParent();
	}
	BlockController::shareData()->hintSprites->removeAllObjects();

	Block* block = gameLayer->findBlockByTouch(pTouch);
	BlockController::shareData()->selectBlock = CCArray::create();
	BlockController::shareData()->selectBlock->retain();
	if(block)
	{
		BlockController::shareData()->mCurType = block->blockType;
		block->setSelected(true);
		BlockController::shareData()->mStartFlip=true;
		if(BlockController::shareData()->selectBlock->count()>0)
			BlockController::shareData()->selectBlock->removeAllObjects();
		BlockController::shareData()->selectBlock->addObject(block);
		addTouchEffect(block);
		return true;
	}
	return false;
}

void GameScene::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if(BlockController::shareData()->mCurType<0)
		return;
	Block* block = gameLayer->findBlockByTouch(pTouch);
	if(block)
	{
		Block* lastBlock = (Block*)BlockController::shareData()->selectBlock->lastObject();
		if(BlockController::shareData()->selectBlock->containsObject(block)
			|| block->blockType!=BlockController::shareData()->mCurType
			|| (abs(lastBlock->mBlockPos->x-block->mBlockPos->x)>1 || abs(lastBlock->mBlockPos->y-block->mBlockPos->y)>1)
			|| (
					(lastBlock->mBlockPos->x%2==0) 
					&& abs(lastBlock->mBlockPos->x-block->mBlockPos->x)==1 
					&& (block->mBlockPos->y>lastBlock->mBlockPos->y)
				)
			|| (
					(lastBlock->mBlockPos->x%2!=0) 
					&& abs(lastBlock->mBlockPos->x-block->mBlockPos->x)==1 
					&& (block->mBlockPos->y<lastBlock->mBlockPos->y)
				)
			)
		{
			return;
		}
		else
		{
			block->setSelected(true);
			addTouchEffect(block);
			BlockController::shareData()->selectBlock->addObject(block);
		}
	}
}
void GameScene::addTouchEffect(Block* block)
{
	CCParticleFire* fire = CCParticleFire::create();
	fire->setDuration(0.2f);
	fire->setAutoRemoveOnFinish(true);
	fire->setContentSize(block->getContentSize());
	fire->setPosition(block->getPosition());
	gameLayer->addChild(fire);
}
void GameScene::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	if(!(BlockController::shareData()->mStartFlip)) return;
	Block* firstBlock = (Block*)BlockController::shareData()->selectBlock->objectAtIndex(0);
	bool isBomb = firstBlock->isBomb;
	CCObject* obj = NULL;
	CCArray* pSelects = CCArray::create();
	CCArray* blocks = gameLayer->mGameLayer->getChildren();
	if(isBomb)
	{
		BlockController::shareData()->selectBlock->removeAllObjects();
		int maxH = BLOCK_PAN_SIZE_H;
		if((firstBlock->mBlockPos->x)%2!=0)
			maxH--;
		for(int i=0;i<maxH;i++)
		{
			Block* block = gameLayer->findBlockByPos(firstBlock->mBlockPos->x,i);
			pSelects->addObject(block);
			BlockController::shareData()->selectBlock->addObject(block);
		}
	}
	else
	{
		CCARRAY_FOREACH(blocks,obj)
		{
			Block* block = (Block*)obj;
			if (pSelects->containsObject(block))
			{
				return;
			}
			if (block!=NULL && block->isSelected && !block->isRemoved)
			{
				pSelects->addObject(block);
			}
		}
	}
	BlockController::shareData()->mStartFlip = false;
	if(pSelects->count()<3)
	{
		BlockController::shareData()->resetSelect();
		SimpleAudioEngine::sharedEngine()->playEffect(RESOURCE_PATH_AUDIO("miss.wav"));
		return;
	}
	int blockNum = pSelects->count();
	GameData::shareData()->addScore(PER_BLOCK_SCORE*((blockNum*(blockNum+1))/2));
	updateScore(GameData::shareData()->getScore());
	if(BlockController::shareData()->selectBlock->count()<BLOCK_COMBO_NUM)
		GameData::shareData()->setCombo(0);
	else
		GameData::shareData()->addCombo(1);
	BlockController::shareData()->isCreateBomb = (GameData::shareData()->getCombo()>BOMB_COMBO_NUM);
	int i =0;
	CCARRAY_FOREACH(BlockController::shareData()->selectBlock,obj)
	{
		Block* block = (Block*)obj;
		int lastLine = gameLayer->findLastLineByCol(block->mBlockPos->x);
		int newX = block->mBlockPos->x;
		int newY = lastLine+1;
		int newCol = newX;
		if(!isBomb && BlockController::shareData()->isCreateBomb && i == 0)
		{
			newY = block->mBlockPos->y;
			pSelects->removeObject(block);
			block->blockRemove();
			BlockController::shareData()->bombSprite = gameLayer->createNewBlock(newX,newY,newCol,true);
			gameLayer->mGameLayer->addChild(BlockController::shareData()->bombSprite);
		}
		else
		{
			gameLayer->mGameLayer->addChild(gameLayer->createNewBlock(newX,newY,newCol,false));
		}
		i++;
	}
	blocksRemove(pSelects);
	mFreeTime = 0;
}

GameScene::~GameScene()
{
	
}

void GameScene::blocksRemove(CCArray* pSelects)
{
	CCObject* obj = NULL;
	Block* bombBlock = NULL;
	CCARRAY_FOREACH(pSelects,obj)
	{
		Block* block = (Block*)obj;
		if(block->isBomb)
		{
			bombBlock = block;
			break;
		}
	}
	CCArray* removedPos = CCArray::create();
	CCARRAY_FOREACH(pSelects,obj)
	{
		Block* block = (Block*)obj;

		BlockPos* pos = block->mBlockPos;
		removedPos->addObject(pos);
		int lastLine = gameLayer->findLastLineByCol(pos->x);
		if(!bombBlock && BlockController::shareData()->isCreateBomb)
		{
			CCPoint bombPos = BlockController::shareData()->bombSprite->getPosition();
			CCMoveTo* moveTo = CCMoveTo::create(0.2f,bombPos);
			CCCallFunc* removeFunc = CCCallFunc::create(block,callfunc_selector(Block::blockRemove));
			CCSequence* act = CCSequence::create(moveTo,removeFunc,NULL);
			block->runAction(act);
		}
		else if (bombBlock)
		{
			CCParticleSystem *meteor=CCParticleSystemQuad::create("particles/ExplodingRing.plist");
			gameLayer->addChild(meteor);
			meteor->setScale(2);
			meteor->setPosition(block->getPosition());
			if(block == bombBlock)
			{
				CCCallFunc* removeFunc = CCCallFunc::create(block,callfunc_selector(Block::blockRemove));
				CCSequence* act = CCSequence::create(removeFunc,NULL);
				block->runAction(act);
				
				//gameLayer->runAction(getShakeAction(gameLayer->getPosition()));
				ZhenPing(0.5f);
			}
			else
			{
				float offBlockX = CCRANDOM_MINUS1_1()*320.0f;
				float time = CCRANDOM_0_1()+0.6;
				int targetX = block->getPositionX()+offBlockX;
				
				CCJumpTo* eff = CCJumpTo::create(0.3f,ccp(targetX,block->getPositionY()-1136),500.0f,1);
				CCCallFunc* removeFunc = CCCallFunc::create(block,callfunc_selector(Block::blockRemove));
				CCSequence* act = CCSequence::create(eff,removeFunc,NULL);
				block->runAction(act);
			}
		}
		else
		{
			CCParticleSystem *meteor=CCParticleSystemQuad::create("particles/taken-gem.plist");
			gameLayer->addChild(meteor);
			meteor->setScale(2);
			meteor->setPosition(block->getPosition());
			block->blockRemove();
		}
		if(block)
			blockFallDown(block);
	}
	if(bombBlock == NULL)
	{
		int soundType = GameData::shareData()->getCombo()>4?4:GameData::shareData()->getCombo();
		CCString* soundFile = CCString::createWithFormat("gem-%d.wav",soundType);
		SimpleAudioEngine::sharedEngine()->playEffect(RESOURCE_PATH_AUDIO(soundFile->getCString()));
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->playEffect(RESOURCE_PATH_AUDIO("powerup.wav"));
	}
	//rechargBlocks(removedPos);
}
CCAction* GameScene::getShakeAction(CCPoint pointBg)
{
	CCPoint pointL=pointBg;
	CCPoint pointR=pointBg; 
	pointL.x-=3; 
	pointR.x+=3;
	CCMoveTo* moveLeft=CCMoveTo::create(0.08f, pointL); 
	CCMoveTo* moveRight=CCMoveTo::create(0.08f, pointR); 
	CCFiniteTimeAction* action= CCSequence::create(moveLeft,moveRight,NULL);
	CCActionInterval* actionShake=CCRepeat::create((CCActionInterval*)action,3); 
	return actionShake;

}
void GameScene::ZhenPing(int t)
{
	CCScene* f = CCDirector::sharedDirector()->getRunningScene();
	CCPoint size = gameLayer->getPosition();
	CCMoveTo* left1 = CCMoveTo::create(0.05f,ccp(size.x+5,size.y));
	CCMoveTo* right1 = CCMoveTo::create(0.05f,ccp(size.x-5,size.y));
	CCMoveTo* top1 = CCMoveTo::create(0.05f,ccp(size.x,size.y+5));
	CCMoveTo* rom1 = CCMoveTo::create(0.05f,ccp(size.x,size.y-5));
	CCFiniteTimeAction* action3 = CCSequence::create(left1,right1,top1,rom1,NULL);
	gameLayer->runAction(action3);
}
void GameScene::blockFallDown( CCObject *obj )
{
	setTouchEnabled(false);
	Block* block = (Block*)obj;
	CCLOG("fall block is %i,%i",block->mBlockPos->x,block->mBlockPos->y);
	int lastLine = gameLayer->findLastLineByCol(block->mBlockPos->x);
	int line = block->mBlockPos->y+1;
	CCArray* tFallDown = CCArray::create();
	Block* blockBase = block; 
	for(int i = line;i<=lastLine;i++)
	{
		Block* blockAbove =(Block*)gameLayer->findBlockByPos(block->mBlockPos->x,i);
		if(blockAbove !=NULL && !blockAbove->isRemoved)
		{
			tFallDown->addObject(blockAbove);

			//blockAbove->runAction(CCPlace::create(blockBase->getPosition()));
			blockBase = blockAbove;
		}
	}
	blockBase = block;
	CCObject* mBlockAbove = NULL;
	CCARRAY_FOREACH(tFallDown,mBlockAbove)
	{
		Block* blockAbove= (Block*)mBlockAbove;
		CCMoveBy* moveTo = CCMoveBy::create(0.1f,ccp(0,-gameLayer->sizeY));
		CCEaseSineInOut * easeIn = CCEaseSineInOut::create(moveTo);
		blockAbove->setBlockPosTile(blockAbove->mBlockPos->x,blockAbove->mBlockPos->y-1);
		CCCallFunc* callF = CCCallFunc::create(this,callfunc_selector(GameScene::moveIsDone));
		CCSequence* seq = CCSequence::create(CCDelayTime::create(0.2f),easeIn,callF,NULL);
		blockAbove->runAction(seq);
		//blockAbove->setBlockPos(blockAbove->mBlockPos->x,blockAbove->mBlockPos->y-2);
		//blockAbove->refreshTxt();
		blockBase = blockAbove;
	}

}

void GameScene::moveIsDone()
{
	BlockController::shareData()->mCurType = -1;
	setTouchEnabled(true);
	updateScore(0);
	BlockController::shareData()->selectBlock->removeAllObjects();
	BlockController::shareData()->movedBlock->removeAllObjects();
}
void GameScene::rechargBlocks(CCArray* basePos)
{
	CCObject* obj = NULL;
	CCArray* removedCol = CCArray::create();
	BlockController::shareData()->movedBlock = CCArray::create();
	for (int i =0;i<BLOCK_PAN_SIZE_W;i++)
	{
		BlockController::shareData()->movedBlock->addObject(CCArray::create());
	}
	CCARRAY_FOREACH(basePos,obj)
	{
		BlockPos* base = (BlockPos*) obj;
		int lastLen = gameLayer->findLastLineByCol(base->x);
		CCArray* row = CCArray::create();
		for(int i =base->y;i<= lastLen;i++)
		{
			Block* block = gameLayer->findBlockByPos(base->x,i);
			if (block)
			{
				row->addObject(block);
			}
		}
		if(((CCArray*)BlockController::shareData()->movedBlock->objectAtIndex(base->x))->count()<=0)
			BlockController::shareData()->movedBlock->replaceObjectAtIndex(base->x,row,false);
	}

	int rowNum = 0;
	CCARRAY_FOREACH(BlockController::shareData()->movedBlock,obj)
	{
		CCArray* row = (CCArray*)obj;
		if(!row || row->count()==0) continue;
		CCObject* item = NULL;
		int y = -1;
		int len= 0;
		Block* first =NULL;
		//find first block
		CCARRAY_FOREACH(row,item)
		{
			Block* block =(Block*)item;
			BlockPos* pos = block->mBlockPos;
			if(!first || pos->y < first->mBlockPos->y)
			{
				first = block;
			}
		}
		//find last Target in col
		for(unsigned int i =0;i<basePos->count();i++)
		{
			BlockPos* pos = (BlockPos* ) basePos->objectAtIndex(i);
			if(first->mBlockPos->x == pos->x)
			{
				if(pos->y>y)
					y = pos->y;
			}
		}
		
		item = NULL;
		len = (first->mBlockPos->y-y)*gameLayer->sizeY;
		CCLog("len = %d,row = %d",len,first->mBlockPos->x);
		int offIndex = first->mBlockPos->y-y;
		CCARRAY_FOREACH(row,item)
		{
			Block* block =(Block*)item;
			if (block)
			{
				CCMoveBy* moveTo = CCMoveBy::create(0.1f,ccp(0,-len));
				CCEaseSineInOut * easeIn = CCEaseSineInOut::create(moveTo);
				block->mBlockPos->y -= offIndex;
				block->runAction(easeIn);
			}
		}
		rowNum++;
	}
}

void GameScene::updateThinkTimer(float dt)
{
	if(mFreeTime < 0)
	{
		return;
	}

	mFreeTime++;
	if(mFreeTime>THINK_TIME)
	{
		gameLayer->CheckForHint();
		mFreeTime = -1;
	}
}

void GameScene::keyBackClicked()
{

}

void GameScene::keyMenuClicked()
{

}
