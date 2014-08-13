#include "SimpleAudioEngine.h"
#include "GG_SceneMenu.h"
#include "GG_SceneGamePlay.h"

using namespace CocosDenshion;
using namespace cocos2d;

bool GG_SceneMenu::init()
{	
	if(!CCScene::init()) { return false; }
	
	CCUserDefault *user = CCUserDefault::sharedUserDefault();

	int maxStage = user->getIntegerForKey("max_stage",1);

	GG_Sprite *bg = GG_Sprite::create();
	bg->gg_initFile("menu_bg.png");
	bg->setAnchorPoint(CCPointZero);
	this->addChild(bg);
	

	CCLabelTTF *level_1 = CCLabelTTF::create("Play","walibi0615.ttf",30);


	level_1->setColor( maxStage>=1?ccc3(255,222,109):ccc3(100,100,100)  );


			
	CCMenuItemLabel *itm_1 = CCMenuItemLabel::create(level_1,this,menu_selector(GG_SceneMenu::gg_levelSelection));



	itm_1->setAnchorPoint(CCPointZero);

	itm_1->setPosition(ccp(405,30));

	itm_1->setEnabled( maxStage>=1 );


	
	CCMenu *menu = CCMenu::create(itm_1,NULL);
	menu->setPosition(CCPointZero);
	menu->setAnchorPoint(CCPointZero);
	this->addChild(menu,0,10);



	char s_1[20] ={0}; 
	float hg_1 = user->getIntegerForKey("hg_1")/100.0f;
	hg_1>0?sprintf(s_1,"%.2fm",hg_1):sprintf(s_1,"");



	CCLabelTTF *lbl_stageinfo_1 = CCLabelTTF::create( s_1 ,"walibi0615.ttf",14);	


	lbl_stageinfo_1->setPosition( ccpAdd(itm_1->getPosition(),ccp(12,40)) );		

	
	lbl_stageinfo_1->setColor( maxStage>=1?ccc3(255,157,124):ccc3(100,100,100));


	this->addChild( lbl_stageinfo_1,1 );





	////media menu
	CCMenuItemImage *itm = CCMenuItemImage::create("btn_music.png","btn_music_off.png",this,menu_selector(GG_SceneMenu::gg_menuButtonTapped) );
	itm->setTag(103);
	itm->setPosition( GG_SCR_W-60 , GG_SCR_H-120 );
	CCMenu *mediaMenu = CCMenu::create(itm,NULL);
	mediaMenu->setAnchorPoint(CCPointZero);
	mediaMenu->setPosition(CCPointZero);
	this->addChild(mediaMenu,3);

	
	unsigned int short bgSound = user->getIntegerForKey("BG_SOUND",1);
	(bgSound==1)?itm->unselected():itm->selected();

	SimpleAudioEngine *media = SimpleAudioEngine::sharedEngine();
	if( !media->isBackgroundMusicPlaying() )
	{		
		media->playBackgroundMusic("doraemon.mp3",true);
	}

	if( bgSound==0 )
	{
		media->pauseBackgroundMusic();
	}

	return true;	
}	

void GG_SceneMenu::gg_levelSelection(CCObject *obj)
{
	
	CCUserDefault *user = CCUserDefault::sharedUserDefault();

	CCMenuItemLabel* itm = (CCMenuItemLabel*)obj;
	CCLabelTTF *label = (CCLabelTTF*) itm->getLabel();

	CCSprite *chaien = CCSprite::create();
	CCSprite *nobita = CCSprite::create();
	chaien->initWithSpriteFrameName("chai_run_1.png");
	nobita->initWithSpriteFrameName("no_run_1.png");
	chaien->setPosition( ccpSub(itm->getPosition(),ccp(150,-17)) );
	nobita->setPosition( ccpSub(itm->getPosition(),ccp(100,-12)) );
		
	chaien->runAction( CCRepeatForever::create(GG_CREATE_ANIMATE_FROM_FRAMES("chai_run",1,9,0.03f)) );
	nobita->runAction( CCRepeatForever::create(GG_CREATE_ANIMATE_FROM_FRAMES("no_run",1,10,0.03f)) );

	this->addChild(nobita,2,11);
	this->addChild(chaien,2,12);


	
	int level = CCString::create(label->getString())->intValue();
	user->setIntegerForKey("playing_stage",level);
					

		
	((CCMenu*)this->getChildByTag(10))->setEnabled(false);
	this->schedule( schedule_selector(GG_SceneMenu::gg_levelSelectionInterval) );
}

void GG_SceneMenu::gg_levelSelectionInterval(float delay)
{
	this->getChildByTag(11)->setPositionX(this->getChildByTag(11)->getPositionX()+6);
	this->getChildByTag(12)->setPositionX(this->getChildByTag(12)->getPositionX()+6);
	if( (this->getChildByTag(12)->getPositionX()-100) > GG_SCR_W )
	{
		this->unscheduleAllSelectors();
		this->removeAllChildren();
		CCDirector::sharedDirector()->replaceScene( GG_SceneGamePlay::create() );	
	}
}


void GG_SceneMenu::gg_menuButtonTapped(CCObject *obj )
{					
	CCMenuItemImage *itm = ((CCMenuItemImage*)obj);	

	if(itm->getTag()==103)
	{
		CCUserDefault *user = CCUserDefault::sharedUserDefault();
		SimpleAudioEngine *media = SimpleAudioEngine::sharedEngine();
		unsigned int short bgSound = user->getIntegerForKey("BG_SOUND",1);
		bgSound = 1- bgSound;
		user->setIntegerForKey( "BG_SOUND" ,  bgSound  );	
		if(bgSound==1)
		{
			itm->unselected();
			media->resumeBackgroundMusic();					
		}
		else
		{
			itm->selected();
			media->pauseBackgroundMusic();
		}
	}
}