#include "MainMenuScene.h"
#include "Block.h"
#include "GameScene.h"

MainMenuScene::MainMenuScene(void)
{
}


MainMenuScene::~MainMenuScene(void)
{
}


bool MainMenuScene::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	//this->addChild(fg);
	this->setKeypadEnabled(true);
	logo = CCSprite::create(RESOURCE_PATH_MENU("logo.png"));
	logo->setAnchorPoint(ccp(0.5,0));
	logo->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,DESIGN_SCREEN_SIZE_H-logo->getContentSize().height-30));
	CCScaleTo* scaleBig = CCScaleTo::create(0.5f,1);
	CCScaleTo* scaleSmall = CCScaleTo::create(0.0001f,0);
	CCEaseBounceOut* eff = CCEaseBounceOut::create(scaleBig);
	eff->bounceTime(3);
	CCSequence* act = CCSequence::create(CCHide::create(),scaleSmall,CCShow::create(),eff,NULL);
	logo->setVisible(false);
	logo->runAction(act);

	CCMenuItemImage* playBtn = CCMenuItemImage::create(RESOURCE_PATH_MENU("btn-play.png"),RESOURCE_PATH_MENU("btn-play-down.png"),this,menu_selector(MainMenuScene::onPlayBtn));
	CCMenuItemImage* aboutBtn = CCMenuItemImage::create(RESOURCE_PATH_MENU("btn-about.png"),RESOURCE_PATH_MENU("btn-about-down.png"),this,menu_selector(MainMenuScene::onAboutBtn));
	mainMenu = CCMenu::create(playBtn,aboutBtn,/*doneBtn,*/NULL);;
	int offY = 0;

	CCObject* obj = NULL;
	int i =1;
	CCARRAY_FOREACH(mainMenu->getChildren(),obj)
	{
		((CCNode* )obj)->setVisible(false);
		((CCNode* )obj)->runAction(CCSequence::create(CCDelayTime::create(0.2*i),(CCAction*)act->copy(),NULL));
		i++;
	}
	exitLayer = NULL;
	addChild(mainMenu);
	mainMenu->alignItemsVerticallyWithPadding(offY);
	mainMenu->setAnchorPoint(ccp(0.5,0.5));
	mainMenu->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,350));
	addChild(logo);
	CCLabelBMFont* highScoreTxt = CCLabelBMFont::create(GameData::shareData()->getHighScoreString(),"fonts/scorefont.fnt");
	highScoreTxt->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,100));
	addChild(highScoreTxt);
	CCLabelTTF* highScoreHint = CCLabelTTF::create("High Socre","",30);
	addChild(highScoreHint);
	highScoreHint->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,50));
	return true;
}
void MainMenuScene::onPlayBtn(CCObject* pSender)
{
	CCObject* obj = NULL;
	int i =1;
	CCScaleTo* act = CCScaleTo::create(0.3f,0);
	logo->runAction(act);
	CCARRAY_FOREACH(mainMenu->getChildren(),obj)
	{
		((CCNode* )obj)->runAction((CCAction*)act->copy());
	}
	this->runAction(
		CCSequence::create(
			CCDelayTime::create(0.3f),
			CCCallFunc::create(
				this,
				callfunc_selector(MainMenuScene::startPlay)
			),
			NULL
		)
	);
}

void MainMenuScene::startPlay()
{
	CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}
void MainMenuScene::onExitDoneBtn( CCObject* pSender )
{
	exitGame();
}
void MainMenuScene::onExitCancelBtn( CCObject* pSender )
{
	CCScaleTo* scaleSmall = CCScaleTo::create(0.2f,0);
	CCSequence* act = CCSequence::create(scaleSmall,CCRemoveSelf::create(true),NULL);
	exitLayer->runAction(scaleSmall);
	exitLayer = NULL;
	mainMenu->setEnabled(true);
}

void MainMenuScene::onAboutBtn( CCObject* pSender )
{
	mainMenu->setEnabled(false);
	CCScaleTo* scaleBig = CCScaleTo::create(0.5f,1);
	CCScaleTo* scaleSmall = CCScaleTo::create(0.0001f,0);
	CCEaseBounceOut* eff = CCEaseBounceOut::create(scaleBig);
	eff->bounceTime(3);
	CCSequence* act = CCSequence::create(CCHide::create(),scaleSmall,CCShow::create(),eff,NULL);
	aboutLayer = showAboutLayer();
	aboutLayer->setVisible(false);
	aboutLayer->runAction(act);
	addChild(aboutLayer);
}
CCLayer* MainMenuScene::showExitLayer()
{
	CCLayer* exiter = CCLayer::create();
	CCSprite*bg = CCSprite::create(RESOURCE_PATH_MENU("about-bg.png"));
	bg->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,DESIGN_SCREEN_SIZE_H/2));
	CCLabelTTF* aboutTxt = CCLabelTTF::create(
		"Want to exit ?",RESOURCE_PATH_FONT("Marker Felt.ttf"),60);
	aboutTxt->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,590));
	CCMenuItemFont* yes = CCMenuItemFont::create("YES",this,menu_selector(MainMenuScene::onExitDoneBtn));
	yes->setFontSize(50);
	yes->setFontName(RESOURCE_PATH_FONT("Marker Felt.ttf"));
	CCMenuItemFont* no = CCMenuItemFont::create("NO",this,menu_selector(MainMenuScene::onExitCancelBtn));
	no->setFontSize(50);
	no->setFontName(RESOURCE_PATH_FONT("Marker Felt.ttf"));
	CCMenu* menu = CCMenu::create(yes,no,NULL);
	menu->alignItemsVerticallyWithPadding(50);
	menu->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,DESIGN_SCREEN_SIZE_H/2-50));
	exiter->addChild(bg);
	exiter->addChild(aboutTxt);
	exiter->addChild(menu);
	return exiter;
}
CCLayer* MainMenuScene::showAboutLayer()
{
	CCLayer* about = CCLayer::create();
	CCSprite*bg = CCSprite::create(RESOURCE_PATH_MENU("about-bg.png"));
	CCLabelTTF* aboutTxt = CCLabelTTF::create(
		"GemGame is created with cocos2d by lihu","font/Marker Felt.ttf",30);
	aboutTxt->setPosition(ccp(40,690));
	aboutTxt->setHorizontalAlignment(kCCTextAlignmentLeft);
	aboutTxt->setVerticalAlignment(kCCVerticalTextAlignmentTop);
	aboutTxt->setAnchorPoint(ccp(0,1));
	aboutTxt->setColor(ccc3(255,255,255));
	aboutTxt->setDimensions(CCSizeMake(440,750));
	bg->addChild(aboutTxt);
	bg->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,DESIGN_SCREEN_SIZE_H/2));
	about->addChild(bg);
	CCMenuItemImage* doneBtn = CCMenuItemImage::create(RESOURCE_PATH_MENU("btn-done.png"),RESOURCE_PATH_MENU("btn-done-down.png"),this,menu_selector(MainMenuScene::onAboutDoneBtn));
	CCMenu* menu = CCMenu::create(doneBtn,NULL);
	menu->setPosition(ccp(DESIGN_SCREEN_SIZE_W/2,bg->getPositionY()-bg->getContentSize().height/2));
	menu->alignItemsVertically();
	about->addChild(menu);
	return about;
}
void MainMenuScene::onAboutDoneBtn(CCObject* pSender)
{
	CCScaleTo* scaleSmall = CCScaleTo::create(0.2f,0);
	CCSequence* act = CCSequence::create(scaleSmall,CCRemoveSelf::create(true),NULL);
	aboutLayer->runAction(scaleSmall);
	mainMenu->setEnabled(true);
}

void MainMenuScene::keyBackClicked()
{
	if (exitLayer)
	{
		onExitCancelBtn(this);
		return;
	}
	CCScaleTo* scaleBig = CCScaleTo::create(0.5f,1);
	CCScaleTo* scaleSmall = CCScaleTo::create(0.0001f,0);
	CCEaseBounceOut* eff = CCEaseBounceOut::create(scaleBig);
	eff->bounceTime(3);
	CCSequence* act = CCSequence::create(CCHide::create(),scaleSmall,CCShow::create(),eff,NULL);
	exitLayer = showExitLayer();
	exitLayer->setVisible(false);
	exitLayer->runAction(act);
	addChild(exitLayer);
}

void MainMenuScene::keyMenuClicked()
{

}

void MainMenuScene::exitGame()
{
	CCDirector::sharedDirector()->end();
	exit(0);
}

CCScene* MainScene::scene()
{
	CCScene* scene = CCScene::create();
	MainMenuBGLayer* bgLayer = MainMenuBGLayer::create();
	MainMenuScene* fgLayer = MainMenuScene::create();
	scene->addChild(bgLayer);
	scene->addChild(fgLayer);
	return scene;
}

bool MainMenuBGLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	CCSprite* bgSprite = CCSprite::create(RESOURCE_PATH_GAME("background.png"));
	bgSprite->setAnchorPoint(ccp(0.0f,0.0f));
	mBgEffectLayer = createBGEffect();
	mBgEffectLayer->setContentSize(CCSizeMake(DESIGN_SCREEN_SIZE_W,DESIGN_SCREEN_SIZE_H));
	this->addChild(bgSprite);
	mBgParticleLayer = CCLayer::create();
	CCParticleSystem* tempSystem = CCParticleSystemQuad::create(RESOURCE_PATH_PARTICLES("bg-stars.plist"));
	int posX = CCRANDOM_0_1()*DESIGN_SCREEN_SIZE_W;
	int posY = CCRANDOM_0_1()*DESIGN_SCREEN_SIZE_H;
	tempSystem->setPosition(ccp(posX,posY));
	tempSystem->setScale(2);
	tempSystem->setAutoRemoveOnFinish(true);
	mBgParticleLayer->addChild(tempSystem);
	this->addChild(mBgParticleLayer,1);
	this->addChild(mBgEffectLayer,1);
	this->schedule(schedule_selector(MainMenuBGLayer::blockCreator),1);
	return true;
}
void MainMenuBGLayer::blockCreator(float delta)
{
	if(!mBgEffectLayer)
		return;
	if (mBgEffectLayer->getChildrenCount()<MAX_MENU_BG_STAR_NUM)
	{
		mBgEffectLayer->addChild(createOneStar(),10);
	}
}
CCLayer* MainMenuBGLayer::createBGEffect()
{
	CCLayer* effect = CCLayer::create();
	//time_t* t = new time_t;
	srand(time(NULL)*time(NULL));
	//for(int i =0;i<MAX_MENU_BG_STAR_NUM;i++)
	{
		effect->addChild(createOneStar(),10);
	}
	return effect;
}

Block* MainMenuBGLayer::createOneStar()
{
	int startX,endX,
		startY,endY,
		startLen,endLen,
		startTime,endTime;
	float startScale,endScale;
	startX = 40;
	endX = DESIGN_SCREEN_SIZE_W-40;
	startY = 1200;
	endY = 1200;
	startTime = 7;
	endTime = 10;
	startLen = -200;
	endLen = 200;
	startScale = 0.2f;
	endScale = 0.5f;
	int posX = CCRANDOM_0_1()*endX+startX;
	int posY = startY;
	//CCLOG("%d,%d",posX,posY);
	int len =  CCRANDOM_0_1()*endLen +startLen;
	float time =  CCRANDOM_0_1()*endTime+startTime;
	float scale =  CCRANDOM_0_1()*endScale+startScale;
	Block* block = Block::create(0,0,0);
	block->setPosition(ccp(posX,posY));
	block->setAnchorPoint(ccp(0,0));
	block->setScale(scale);
	block->runAction(
		CCSequence::create(
			CCMoveTo::create(time,ccp(posX,0)),
			CCCallFunc::create(block,callfunc_selector(Block::removeFromParent)),
			NULL)
	);
	return block;
}
