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
	
    
	CCLabelTTF *level_1 = CCLabelTTF::create("1","walibi0615.ttf",30);
	CCLabelTTF *level_2 = CCLabelTTF::create("2","walibi0615.ttf",35);
	CCLabelTTF *level_3 = CCLabelTTF::create("3","walibi0615.ttf",40);
	CCLabelTTF *level_4 = CCLabelTTF::create("4","walibi0615.ttf",50);
	CCLabelTTF *level_5 = CCLabelTTF::create("5","walibi0615.ttf",55);
    
	level_1->setColor( maxStage>=1?ccc3(255,222,109):ccc3(100,100,100)  );
	level_2->setColor( maxStage>=2?ccc3(255,222,109):ccc3(100,100,100)  );
	level_3->setColor( maxStage>=3?ccc3(255,222,109):ccc3(100,100,100)  );
	level_4->setColor( maxStage>=4?ccc3(255,222,109):ccc3(100,100,100)  );
	level_5->setColor( maxStage>=5?ccc3(255,222,109):ccc3(100,100,100)  );
    
    
	CCMenuItemLabel *itm_1 = CCMenuItemLabel::create(level_1,this,menu_selector(GG_SceneMenu::gg_levelSelection));
	CCMenuItemLabel *itm_2 = CCMenuItemLabel::create(level_2,this,menu_selector(GG_SceneMenu::gg_levelSelection));
	CCMenuItemLabel *itm_3 = CCMenuItemLabel::create(level_3,this,menu_selector(GG_SceneMenu::gg_levelSelection));
	CCMenuItemLabel *itm_4 = CCMenuItemLabel::create(level_4,this,menu_selector(GG_SceneMenu::gg_levelSelection));
	CCMenuItemLabel *itm_5 = CCMenuItemLabel::create(level_5,this,menu_selector(GG_SceneMenu::gg_levelSelection));
    
    
	itm_1->setAnchorPoint(CCPointZero);
	itm_2->setAnchorPoint(CCPointZero);
	itm_3->setAnchorPoint(CCPointZero);
	itm_4->setAnchorPoint(CCPointZero);
	itm_5->setAnchorPoint(CCPointZero);
    
	itm_1->setPosition(ccp(405,30));
	itm_2->setPosition( ccpAdd(itm_1->getPosition(),ccp(60,0)) );
	itm_3->setPosition( ccpAdd(itm_1->getPosition(),ccp(2*65,0)) );
	itm_4->setPosition( ccpAdd(itm_1->getPosition(),ccp(3*68,0)) );
	itm_5->setPosition( ccpAdd(itm_1->getPosition(),ccp(4*75,0)) );
	
	
	itm_1->setEnabled( maxStage>=1 );
	itm_2->setEnabled( maxStage>=2 );
	itm_3->setEnabled( maxStage>=3 );
	itm_4->setEnabled( maxStage>=4 );
	itm_5->setEnabled( maxStage>=5 );
    
	
	CCMenu *menu = CCMenu::create(itm_1,itm_2,itm_3,itm_4,itm_5,NULL);
	menu->setPosition(CCPointZero);
	menu->setAnchorPoint(CCPointZero);
	this->addChild(menu,0,10);
    
    
    
	char s_1[20] ={0};
	float hg_1 = user->getIntegerForKey("hg_1")/100.0f;
	hg_1>0?sprintf(s_1,"%.2fm",hg_1):sprintf(s_1,"");
    
	char s_2[20] ={0};
	float hg_2 = user->getIntegerForKey("hg_2")/100.0f;
	hg_2>0?sprintf(s_2,"%.2fm",hg_2):sprintf(s_2,"");
    
	char s_3[20] ={0};
	float hg_3 = user->getIntegerForKey("hg_3")/100.0f;
	hg_3>0?sprintf(s_3,"%.2fm",hg_3):sprintf(s_3,"");
    
	char s_4[20] ={0};
	float hg_4 = user->getIntegerForKey("hg_4")/100.0f;
	hg_4>0?sprintf(s_4,"%.2fm",hg_4):sprintf(s_4,"");
    
	char s_5[20] ={0};
	float hg_5 = user->getIntegerForKey("hg_5")/100.0f;
	hg_5>0?sprintf(s_5,"%.2fm",hg_5):sprintf(s_5,"");
    
	CCLabelTTF *lbl_stageinfo_1 = CCLabelTTF::create( s_1 ,"walibi0615.ttf",14);
	CCLabelTTF *lbl_stageinfo_2 = CCLabelTTF::create( maxStage>=2?s_2:"LOCKED" ,"walibi0615.ttf",14);
	CCLabelTTF *lbl_stageinfo_3 = CCLabelTTF::create( maxStage>=3?s_3:"LOCKED" ,"walibi0615.ttf",14);
	CCLabelTTF *lbl_stageinfo_4 = CCLabelTTF::create( maxStage>=4?s_4:"LOCKED" ,"walibi0615.ttf",14);
	CCLabelTTF *lbl_stageinfo_5 = CCLabelTTF::create( maxStage>=5?s_5:"LOCKED" ,"walibi0615.ttf",14);
    
	lbl_stageinfo_1->setPosition( ccpAdd(itm_1->getPosition(),ccp(12,40)) );
	lbl_stageinfo_2->setPosition( ccpAdd(itm_2->getPosition(),ccp(10,-5)) );
	lbl_stageinfo_3->setPosition( ccpAdd(itm_3->getPosition(),ccp(15,-5)) );
	lbl_stageinfo_4->setPosition( ccpAdd(itm_4->getPosition(),ccp(25,-5)) );
	lbl_stageinfo_5->setPosition( ccpAdd(itm_5->getPosition(),ccp(20,-5)) );
	
	lbl_stageinfo_1->setColor( maxStage>=1?ccc3(255,157,124):ccc3(100,100,100));
	lbl_stageinfo_2->setColor( maxStage>=2?ccc3(255,157,124):ccc3(100,100,100));
	lbl_stageinfo_3->setColor( maxStage>=3?ccc3(255,157,124):ccc3(100,100,100));
	lbl_stageinfo_4->setColor( maxStage>=4?ccc3(255,157,124):ccc3(100,100,100));
	lbl_stageinfo_5->setColor( maxStage>=5?ccc3(255,157,124):ccc3(100,100,100));
    
	this->addChild( lbl_stageinfo_1,1 );
	this->addChild( lbl_stageinfo_2,1 );
	this->addChild( lbl_stageinfo_3,1 );
	this->addChild( lbl_stageinfo_4,1 );
	this->addChild( lbl_stageinfo_5,1 );
    
    
    
    
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
	//CCLOG(  label->getString() );
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
    
	//CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
	//CCSpriteFrameCache::sharedSpriteFrameCache()->purgeSharedSpriteFrameCache();
    
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