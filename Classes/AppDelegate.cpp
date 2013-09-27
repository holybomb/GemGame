#include "AppDelegate.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "GameDefine.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);
//	pEGLView->setFrameSize(480,800);
	pEGLView->setDesignResolutionSize(DESIGN_SCREEN_SIZE_W,DESIGN_SCREEN_SIZE_H, kResolutionShowAll);
    // turn on display FPS
    pDirector->setDisplayStats(!true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = MainMenuLoadingScene::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
	if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
