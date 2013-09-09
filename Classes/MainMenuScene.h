#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameData.h"
#include "Block.h"
#define MAX_MENU_BG_STAR_NUM 20

#define MENU_EFF_LAYER_TAG 100
USING_NS_CC;
USING_NS_CC_EXT;
class MainScene
{
public:
	MainScene();
	~MainScene();
	static CCScene* scene();
};
class MainMenuScene : public CCLayer
{
public:
	MainMenuScene(void);
	~MainMenuScene(void);
	bool init();
	CREATE_FUNC(MainMenuScene);
	void onPlayBtn(CCObject* pSender);
	void onDoneBtn(CCObject* pSender);
	void onAboutBtn(CCObject* pSender);
	CCLayer* showAboutLayer();
	void onAboutDoneBtn(CCObject* pSender);
	void startPlay();
	CCMenu* mainMenu;
	CCLayer* aboutLayer;
	CCSprite* logo;
};
class MainMenuBGLayer : public CCLayer
{
public:
	bool init();
	CREATE_FUNC(MainMenuBGLayer);
	Block* createOneStar();
	CCLayer* createBGEffect();
	void blockCreator(float delta);
	CCLayer* mBgEffectLayer;
	CCLayer* mBgParticleLayer;
};

