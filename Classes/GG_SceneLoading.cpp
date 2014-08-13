#include "SimpleAudioEngine.h"
#include "GG_SceneMenu.h"
#include "GG_SceneGamePlay.h"
#include "GG_SceneGameOver.h"
#include "GG_SceneLoading.h"

using namespace CocosDenshion;
using namespace cocos2d;

bool GG_SceneLoading::init()
{	
	if(!CCScene::init()) { return false; }
	
	this->gg_p_count = 0;

	
	CCLabelTTF *lbl = CCLabelTTF::create("Nguyễn Đức Tuấn","walibi0615.ttf",48);
	lbl->setPosition( ccp(GG_SCR_W/2,GG_SCR_H/2 ) );
	lbl->setOpacity(0);	
	this->addChild(lbl);
	lbl->runAction( CCFadeIn::create(3.0f) );


	lbl = CCLabelTTF::create("VB2K16SE","walibi0615.ttf",24);
	lbl->setPosition( ccp(GG_SCR_W/2,GG_SCR_H/2-50 ) );
	lbl->setOpacity(0);	
	this->addChild(lbl);
	lbl->runAction(CCFadeIn::create(3.0f) );

	this->schedule(schedule_selector( GG_SceneLoading::gg_starGame ) , 0.2f   );

	return true;
}

void GG_SceneLoading::gg_starGame(float delay)
{	
	SimpleAudioEngine *media = SimpleAudioEngine::sharedEngine();
	
	this->gg_p_count++;
	//CCLOG( "%i" ,this->gg_p_count );
	switch( this->gg_p_count )
	{
		case 1: 
			media->preloadBackgroundMusic("doraemon.mp3");				
			break;	

		case 2: 
			media->preloadEffect("dinosaur_1.wav");	
			break;	

		case 3: 
			media->preloadEffect("dinosaur_2.wav");	
			break;	

		case 4: 
			media->preloadEffect("dinosaur_3.wav");
			break;	

		case 5: 
			media->preloadEffect("call_nobita.wav");
			break;	

		case 6: 
			media->preloadEffect("pie.wav");
			break;	

		case 7: 
			GG_LOAD_PLIST_FRAMES("dj.plist");	
			break;	

		case 8: 
			GG_LOAD_PLIST_FRAMES("chaien.plist");		
			break;	

		case 9: 
			GG_LOAD_PLIST_FRAMES("traffic.plist");	
			break;	

		case 10: 
			GG_LOAD_PLIST_FRAMES("turtle.plist");	
			break;	

		case 11: 
			GG_LOAD_PLIST_FRAMES("stars.plist");
			break;	

	}
	
	if( this->gg_p_count == 12 )
	{
		this->unscheduleAllSelectors();
		this->removeAllChildren();

		 CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create(0.5f,GG_SceneMenu::create() ) );


	}
}

