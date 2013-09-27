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
	void onExitDoneBtn(CCObject* pSender);
	void onAboutBtn(CCObject* pSender);
	CCLayer* showAboutLayer();
	void onAboutDoneBtn(CCObject* pSender);
	void startPlay();
	CCMenu* mainMenu;
	CCLayer* aboutLayer;
	CCLayer* exitLayer;
	CCSprite* logo;
	void exitGame();
	virtual void keyBackClicked();//Android 后退键
	virtual void keyMenuClicked();//Android 菜单键
	CCLayer* showExitLayer();
	void onExitCancelBtn( CCObject* pSender );
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
class MainMenuLoadingScene : public CCLayer
{
public:
	static CCScene* scene();
	bool init();
	void loadSound(float delta);
	CREATE_FUNC(MainMenuLoadingScene);
};
