#include "SimpleAudioEngine.h"
#include "GG_SceneGameOver.h"
#include "GG_SceneMenu.h"
#include "GG_SceneGamePlay.h"

using namespace CocosDenshion;
using namespace cocos2d;

bool GG_SceneGameOver::init()
{
	if(!CCScene::init()) { return false; }
	static CCUserDefault *user = CCUserDefault::sharedUserDefault();
	unsigned int short re = user->getIntegerForKey("GAMEOVER"); //re=2;
	GG_Sprite *bg = GG_Sprite::create();
	
	int playingStage = user->getIntegerForKey("playing_stage",1); //playingStage=1;
    
	if( re==1 ) //lose
	{
        bg->gg_initFile("gameover.png");
		
	}
	else
	{ //win
		bg->gg_initFile("next_stage.png");
        
		CCLabelTTF *lbl_score = CCLabelTTF::create("DISTANCE","walibi0615.ttf",16);
		float distance =  user->getIntegerForKey("DISTANCE");   //distance=7000;
		char s[50]={0}; sprintf(s,"%.2fm",distance/100.0f);
		CCLabelTTF *lbl_scoreNumber = CCLabelTTF::create(s,"walibi0615.ttf",16);
		lbl_score->setColor( ccc3(255,222,109) );
		//lbl_score->setPosition(ccp(340,260));
		lbl_score->setPosition(ccp(340,GG_SCR_H+100));
		lbl_score->runAction( CCSequence::create(CCDelayTime::create(0.5f) , CCEaseBounceOut::create( CCMoveTo::create(0.5f,ccp(340,260)) ),NULL)  );
		//lbl_scoreNumber->setPosition(ccp( 340,240) );
		lbl_scoreNumber->setPosition(ccp( 300 ,240) );
		lbl_scoreNumber->runAction( CCEaseElasticOut::create( CCMoveTo::create(1.0f, ccp(340,240) ) ) );
		this->addChild(lbl_score,1);
		this->addChild(lbl_scoreNumber,1);
        
		//Update highdistance
		sprintf(s,"hg_%i",playingStage);
		int hg = user->getIntegerForKey(s,0);
		if( hg<distance )
		{
			user->setIntegerForKey(s,distance);
            
			CCLabelTTF *lbl_record = CCLabelTTF::create("New Record","walibi0615.ttf",14);
			lbl_record->setColor(ccc3(255,157,124));
			lbl_record->setPosition(ccp(340,GG_SCR_H+100));
			lbl_record->runAction( CCSequence::create(CCDelayTime::create(1.0f) , CCEaseBounceOut::create( CCMoveTo::create(0.5f,ccp(340,275)) ),NULL)  );
			this->addChild(lbl_record,1);
			if( user->getIntegerForKey("BG_SOUND",1) == 1 )
			{
				SimpleAudioEngine::sharedEngine()->playEffect("pie.wav");
			}
            
		}
		////////////////
        
        
		if( playingStage < 5 )
		{
			int nextStage = playingStage+1;
			int maxStage = user->getIntegerForKey("max_stage",1);
			if( nextStage > maxStage )
			{
				user->setIntegerForKey( "max_stage" , nextStage );
			}
            
			user->setIntegerForKey("playing_stage", nextStage );
		}
		else	//win last stage
		{
			CCLabelTTF *lbl_thank = CCLabelTTF::create( "THANK YOU FOR YOUR PLAYING!" ,"walibi0615.ttf" , 24   );
			
			lbl_thank->setPosition( ccp( GG_SCR_W/2,GG_SCR_H-120 ) );
			lbl_thank->setScale(0.2f);
			lbl_thank->setOpacity(0);
			this->addChild( lbl_thank ,2 );
			lbl_thank->runAction( CCSpawn::create(
                                                  CCFadeIn::create(1.0f),
                                                  CCScaleTo::create(1.0f,1.0f),
                                                  NULL ) );
            
			
			CCLabelTTF *lbl_copy = CCLabelTTF::create( "Doraemon Comic" ,"walibi0615.ttf" , 16   );
			CCLabelTTF *lbl_copy2 = CCLabelTTF::create( "Fujiko F.Fujio" ,"walibi0615.ttf" , 14   );
			lbl_copy->setAnchorPoint(CCPointZero);
			lbl_copy2->setAnchorPoint(CCPointZero);
			lbl_copy->setPosition(ccp(10,30));
			lbl_copy2->setPosition( ccpAdd(lbl_copy->getPosition(),ccp(0,-15)) );
			this->addChild(lbl_copy,2);
			this->addChild(lbl_copy2,2);
            
		}
		
	}
    
    
	bg->setAnchorPoint(CCPointZero);
	this->addChild(bg);
    
	
	CCMenuItemImage *itm1 =NULL;
    if( re==1)
    {
		itm1 = CCMenuItemImage::create("btn_replay.png","btn_replay.png",this,menu_selector(GG_SceneGameOver::gg_menuButtonTapped) );
		itm1->setPosition(ccp(430,350));
    }
    else
    {
		
		
		if( playingStage<5)
		{
			itm1 = CCMenuItemImage::create("btn_next.png","btn_next.png",this,menu_selector(GG_SceneGameOver::gg_menuButtonTapped) );
		}
		else
		{
			itm1 = CCMenuItemImage::create("btn_replay.png","btn_replay.png",this,menu_selector(GG_SceneGameOver::gg_menuButtonTapped) );
		}
		itm1->setPosition(ccp(290,170));
		
    }
	
	CCMenuItemImage *itm2 = CCMenuItemImage::create("btn_menu.png","btn_menu.png",this,menu_selector(GG_SceneGameOver::gg_menuButtonTapped) );
	CCMenuItemImage *itm3 = CCMenuItemImage::create("btn_music.png","btn_music_off.png",this,menu_selector(GG_SceneGameOver::gg_menuButtonTapped) );
	CCMenuItemLabel *itm_rate = CCMenuItemLabel::create( CCLabelTTF::create("RATE","walibi0615.ttf",48) ,this,menu_selector(GG_SceneGameOver::gg_menuButtonTapped) );
	
	itm2->setPosition( ccpAdd(itm1->getPosition(),ccp(120,0)) );
	itm3->setPosition( ccpAdd(itm1->getPosition(),ccp(2*120,0)) );
	itm_rate->setPosition( GG_SCR_W/2 , 50 );
	CCMenu *menu = CCMenu::create(itm1,itm2,itm3,itm_rate,NULL);
	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);
	itm1->setTag(101);
	itm2->setTag(102);
	itm3->setTag(103);
	itm_rate->setTag(104);
	this->addChild(menu,2);
    
	
	unsigned int short bgSound = user->getIntegerForKey("BG_SOUND",1);
	bgSound?itm3->unselected():itm3->selected();
    
    
    
	CCLabelTTF *lbl_replay = CCLabelTTF::create( ((re==1||playingStage==5)?"Replay":"Next Stage"),"walibi0615.ttf",18);
	CCLabelTTF *lbl_menu = CCLabelTTF::create("Menu","walibi0615.ttf",18);
	CCLabelTTF *lbl_sound = CCLabelTTF::create("Sound","walibi0615.ttf",18);
	lbl_replay->setPosition( ccpAdd(itm1->getPosition(),ccp(0,-45)) );
	lbl_menu->setPosition( ccpAdd(itm2->getPosition(),ccp(0,-45)) );
	lbl_sound ->setPosition( ccpAdd(itm3->getPosition(),ccp(0,-45)) );
	this->addChild(lbl_replay);
	this->addChild(lbl_menu);
	this->addChild(lbl_sound);
    
    
    
	///RATE
    
	return true;
}

void GG_SceneGameOver::gg_menuButtonTapped(CCObject *obj )
{
	int tag =  ((CCMenuItemImage*)obj)->getTag();
    
	if(tag==103)
	{
		CCUserDefault *user = CCUserDefault::sharedUserDefault();
		SimpleAudioEngine *media = SimpleAudioEngine::sharedEngine();
		unsigned int short bgSound = user->getIntegerForKey("BG_SOUND",1);
		bgSound = 1- bgSound;
		user->setIntegerForKey( "BG_SOUND" ,  bgSound  );
		if(bgSound==1)
		{
			((CCMenuItemImage*)obj)->unselected();
            media->resumeBackgroundMusic();
		}
		else
		{
			((CCMenuItemImage*)obj)->selected();
            media->pauseBackgroundMusic();
		}
        
	}
	else
	{
		if(tag==102)
		{
			this->removeAllChildren();
			CCDirector::sharedDirector()->replaceScene( GG_SceneMenu::create() );
		}
		else
		{
			if(tag==101)
			{
				this->removeAllChildren();
				CCDirector::sharedDirector()->replaceScene( GG_SceneGamePlay::create() );
			}
			else
			{
                CCLOG("RATE");
			}
		}
	}
	
}

