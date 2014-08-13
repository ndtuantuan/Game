#include "cocos2d.h"
#include "CCEGLView.h"
#include "SimpleAudioEngine.h"
#include "AppDelegate.h"

#include "GG_SceneGamePlay.h"
#include "GG_SceneMenu.h"
#include "GG_SceneGameOver.h"
#include "GG_SceneLoading.h"
#include "GG_HudLayer.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{	
	srand( time(NULL) );

	// initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize( 800,480, kResolutionShowAll );

    // turn on display FPS
   // pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    // run
	//pDirector->runWithScene( GG_SceneGamePlay::create() );
	//pDirector->runWithScene( GG_SceneMenu::create() );
	//pDirector->runWithScene(GG_SceneGameOver::create());
	pDirector->runWithScene( GG_SceneLoading::create() );
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
