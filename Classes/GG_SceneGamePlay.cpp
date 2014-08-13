#include "SimpleAudioEngine.h"
#include "GG_SceneMenu.h"
#include "GG_SceneGamePlay.h"
#include "GG_SceneGameOver.h"
#include "GG_HudLayer.h"
using namespace cocos2d;
using namespace CocosDenshion;

#define GG_ON_GROUND_DELTA 25.0f
#define GG_JUMP_POWER 17.5f
#define GG_GRAVITY 1.0f
#define GG_RUN_SPEED 7.0f
#define GG_TRAFFIC_SPEED 210.0f
#define GG_CHAIEN_BASE_JUMP_SPEED 439.0f


bool GG_SceneGamePlay::init()
{
	static CCNotificationCenter *notifier = CCNotificationCenter::sharedNotificationCenter();

	if(!CCScene::init()) { return false; }
	
    hud = new GG_HudLayer();
    hud->setAnchorPoint(CCPointZero);
    hud->setPosition(CCPointZero);
    
    
	////////////////
	CCUserDefault *user = CCUserDefault::sharedUserDefault();
    CCUserDefault::sharedUserDefault()->setIntegerForKey("Health", 100);
	CCLOG("--------- STAGE %i ---------",user->getIntegerForKey("playing_stage") );
	///////////////
	
	this->gg_p_background = CCSpriteBatchNode::createWithTexture( CCTextureCache::sharedTextureCache()->addImage("background.png") , 2);
	CCSprite *bg_1 = CCSprite::createWithTexture(this->gg_p_background->getTexture());
	CCSprite *bg_2 = CCSprite::createWithTexture(this->gg_p_background->getTexture());
		
	bg_1->setAnchorPoint(CCPointZero);
	bg_2->setAnchorPoint(CCPointZero);
		
	bg_1->setPositionX(0);
	bg_2->setPositionX(bg_1->getContentSize().width-2);
		
	this->gg_p_background->addChild(bg_1);
	this->gg_p_background->addChild(bg_2);
	
	this->gg_p_background->setAnchorPoint(CCPointZero);
	this->gg_p_background->setPosition(CCPointZero);
	this->addChild(this->gg_p_background);
	
	this->gg_p_layerPlay = CCLayer::create();

	this->gg_p_isNobitaFalling = false;
	this->gg_p_isNobitaJumpping = false;
	this->gg_p_NobitaJumpingTime = 0;
	this->gg_p_isNobitaWalled = false;
	
	//init map
       
        this->gg_p_map = CCTMXTiledMap::create(CCString::createWithFormat("map/map_%d.tmx",user->getIntegerForKey("playing_stage"))->getCString());
    CCLog("---%s",CCString::createWithFormat("map/map_%d.tmx",user->getIntegerForKey("playing_stage"))->getCString());
	
	
	///cache map properties
	CCSize size = this->gg_p_map->getMapSize();
	this->gg_p_mapWidth = size.width;
	this-> gg_p_mapHeight = size.height;
	
	size = this->gg_p_map->getTileSize();
	this->gg_p_tileWidth =size.width;
	this->gg_p_tileHeight =size.height;

	this->gg_p_mapPixelWidth = this->gg_p_mapWidth*this->gg_p_tileWidth;
	
	this->gg_p_layerBump = this->gg_p_map->layerNamed("collision");
	this->gg_p_layerWorld = this->gg_p_map->layerNamed("ground_1_1");

	this->gg_p_layerBump->setVisible(false);
	this->gg_p_layerPlay->addChild( this->gg_p_map ,2);
	///////////////////////////////////
	
	CCTMXObjectGroup *layerObject = this->gg_p_map->objectGroupNamed("objects");
	CCDictionary *obj =  layerObject->objectNamed("nobita");
	float x = obj->valueForKey("x")->intValue();
	float y = obj->valueForKey("y")->intValue();
	this->gg_p_nobita = GG_Sprite::create();	

	this->gg_p_nobita->initWithSpriteFrameName("no_run_1.png");	
	this->gg_p_nobita->setAnchorPoint(ccp(0.5f,0.0f));
	this->gg_p_nobita->setPosition(ccp(x,y-GG_ON_GROUND_DELTA));
		
	this->gg_p_layerPlay->addChild( this->gg_p_nobita,0 );

	
	this->gg_p_nobitaRunAnimate = CCRepeatForever::create( GG_CREATE_ANIMATE_FROM_FRAMES("no_run",1,10,0.04f));
	this->gg_p_nobitaRunAnimate->retain();


	this->gg_p_nobitaCollisionAnimate = GG_CREATE_ANIMATE_FROM_FRAMES("no_coll",1,9,0.03f);
	this->gg_p_nobitaCollisionAnimate->retain();

	
	//////////// CHAIEN
    obj =  layerObject->objectNamed("chaien");
	x = obj->valueForKey("x")->intValue();
	y = obj->valueForKey("y")->intValue();
	this->gg_p_chaien = GG_Sprite::create();	

	this->gg_p_chaien->initWithSpriteFrameName("chai_run_1.png");	
	this->gg_p_chaien->setAnchorPoint(ccp(0.5f,0.0f));
	this->gg_p_chaien->setPosition(ccp(x,y-GG_ON_GROUND_DELTA));
	
	this->gg_p_layerPlay->addChild( this->gg_p_chaien , 1 );

	
	this->gg_p_chaienRunAnimate = CCRepeatForever::create( GG_CREATE_ANIMATE_FROM_FRAMES("chai_run",1,9,0.03f));
	this->gg_p_chaienRunAnimate->retain();
	///////////////////////////////////
    this->addChild(hud,0);

	this->addChild(this->gg_p_layerPlay,1);
	
				
	///CHAIEN
	this->gg_p_isChaienJumpping = false;	
	this->gg_p_chaienNextJumpPos = 0;
	this->gg_p_ChaienJumpPos = this->gg_p_map->objectGroupNamed("chaien_jump_pos");	
	////////////////////
	
	/////////// Traffic //////////////////////	
	CCArray *objs = layerObject->getObjects();
	CCObject *fetch;
	CCARRAY_FOREACH( objs , fetch )
	{
	
		CCDictionary *dict = (CCDictionary*)fetch;						
		if( dict->valueForKey("name")->compare("nobita") != 0  &&  dict->valueForKey("name")->compare("chaien") != 0   )
		{
			GG_traffic *traffic = GG_traffic::create();
			traffic->gg_initWithDict(dict);
			this->gg_p_layerPlay->addChild( traffic , 1 );						
		}
	}
	
	/////////////////////////////////////////
	
	this->gg_p_isNobitaCollisionTraffic = 0;
	this->gg_p_isChaienCollisionTrafic = false;
	notifier->addObserver( this, callfuncO_selector(GG_SceneGamePlay::GG__nobitaCollisionResponse) , "NOBITA_COLLISION" , NULL );
	notifier->addObserver( this, callfuncO_selector(GG_SceneGamePlay::GG__chaienCollisionResponse ) , "CHAIEN_COLLISION" , NULL );
	




	 this->gg_p_chaienJumpSpeed = GG_CHAIEN_BASE_JUMP_SPEED;
	 this->gg_p_gameOver = 0;


	 this->gg_run();
	 GG_ADD_TARGET_TOUCH_DELEGATE(this);
	


	 

	 ///SOUND	
	 this->gg_p_isSoundOn = user->getIntegerForKey("BG_SOUND",1)==1;
	 if( this->gg_p_isSoundOn )
	 {
		 SimpleAudioEngine *media = SimpleAudioEngine::sharedEngine();
		 media->playEffect("call_nobita.wav");
	 }

	return true;
}

void GG_SceneGamePlay::gg_buttomMenuTapped( CCObject *obj )
{
	this->gg_cleanUp();	
	CCDirector::sharedDirector()->replaceScene( GG_SceneMenu::create() );	
	
}

void GG_SceneGamePlay::gg_run()
{	
	this->gg_p_nobita->runAction( CCRepeatForever::create( CCMoveBy::create(1.0f / 60.0f ,ccp(GG_RUN_SPEED,0)) ) );
	this->gg_p_chaien->runAction( CCRepeatForever::create( CCMoveBy::create(1.0f / 60.0f ,ccp(GG_RUN_SPEED,0)) ) );

	this->gg_p_nobita->runAction( this->gg_p_nobitaRunAnimate  );	
	this->gg_p_chaien->runAction( this->gg_p_chaienRunAnimate  );	

	this->schedule( schedule_selector( GG_SceneGamePlay::gg_runInterVal )   );
	
}

void GG_SceneGamePlay::gg_runInterVal(float delay)
{
    if(CCUserDefault::sharedUserDefault()->getIntegerForKey("Health")>100)
        CCUserDefault::sharedUserDefault()->setIntegerForKey("Health", 100);
	hud->updateHealth(CCUserDefault::sharedUserDefault()->getIntegerForKey("Health"));
	if( this->gg_p_gameOver==0 )	
    {
	    			
		CCPoint nobitaPos = this->gg_p_nobita->getPosition();
		CCPoint chaienPos = this->gg_p_chaien->getPosition();		

		float nobitaTargetY = nobitaPos.y;


		static CCNotificationCenter *notifier = CCNotificationCenter::sharedNotificationCenter();
		static unsigned int short count = 0;
		if( ++count == 6 )
		{
			notifier->postNotification("CHECK_COLLISION",CCArray::create(this->gg_p_nobita, CCInteger::create(this->gg_p_chaien->getPositionX()) ,NULL));
			count=0;			
		}

		/////////////////////////		

		if( this->gg_p_isNobitaJumpping )
		{				
			nobitaTargetY = this->gg_p_nobitaJumpPosY + 
			GG_JUMP_POWER*this->gg_p_NobitaJumpingTime - 
			((GG_GRAVITY*this->gg_p_NobitaJumpingTime*this->gg_p_NobitaJumpingTime)/2);

			this->gg_p_NobitaJumpingTime ++;	
							
			if( this->gg_p_NobitaJumpingTime > 30  )
			{																					 
				int  bump = this->gg_detechBump( ccp(nobitaPos.x+30, nobitaTargetY) );
				if( bump > 0 )
					{
						 int groundY = this->gg_p_layerBump->positionAt( this->gg_coordForPos( ccp(nobitaPos.x+20, nobitaTargetY) ) ).y;
						 nobitaTargetY = ( groundY + this->gg_p_tileHeight - GG_ON_GROUND_DELTA );
										
						  this->gg_p_nobita->runAction( this->gg_p_nobitaRunAnimate );
					
						  this->gg_p_isNobitaJumpping=false;
					 }
					else
					 {
						 if( bump < 0 )
						  {
								//CCLOG("DIE ON JUMP");								
								this->gg_p_chaien->stopAllActions();
								this->gg_p_nobita->stopAllActions();
								this->gg_p_nobita->initWithSpriteFrameName("no_fall.png");
								this->gg_p_nobita->setAnchorPoint(ccp(0.5f,0.0f));
								this->gg_p_nobita->runAction( CCSequence::create(
									CCDelayTime::create(0.2f),
									CCFlipX::create(true) , 
									CCDelayTime::create(0.2f) , 
									CCMoveTo::create(0.1f,ccp( nobitaPos.x ,-200)),NULL) );
								
									this->gg_p_gameOver = 1;
									return;
						   }
					   }
					
				 }				
		  }		
		 else  // not jumpping
		{									
				float fallToY =  nobitaPos.y-8;
				int  bump = this->gg_detechBump(ccp(nobitaPos.x,fallToY));						
				if( bump==0 )
				{		
					nobitaTargetY = fallToY ;
					this->gg_p_isNobitaFalling = true;
				}
				else
				{																	
					if( this->gg_p_isNobitaFalling )
					{										
						if( bump > 0 )
						{
					
							float groundY = this->gg_p_layerBump->positionAt( this->gg_coordForPos( ccp(nobitaPos.x,fallToY) ) ).y;
							nobitaTargetY = ( groundY + this->gg_p_tileHeight - GG_ON_GROUND_DELTA );
							this->gg_p_isNobitaFalling = false;							
						}
						else
						{
				
							//CCLOG("DIE");												
							this->gg_p_chaien->stopAllActions();
							this->gg_p_nobita->stopAllActions();
							this->gg_p_nobita->initWithSpriteFrameName("no_fall.png");
							this->gg_p_nobita->setAnchorPoint(ccp(0.5f,0.0f));
							this->gg_p_nobita->runAction( CCSequence::create(
									CCDelayTime::create(0.2f),
									CCFlipX::create(true) , 
									CCDelayTime::create(0.2f) , 
									CCMoveTo::create(0.1f,ccp( nobitaPos.x ,-200)),NULL) );
						
							this->gg_p_gameOver = 1;

							return;
						}
					}
					else
					{
														
							//check bridge
							CCDictionary *dict =  this->gg_p_map->propertiesForGID(bump);
							if( dict && (dict->valueForKey("isBridge")->intValue() == 1) )
							{																																
									CCSprite *bridgeTile = this->gg_p_layerWorld->tileAt( this->gg_coordForPos(ccp(nobitaPos.x,nobitaPos.y)) );
									if( bridgeTile->numberOfRunningActions() == 0 )
									{
										//CCLOG("Bridge Tile Animate");
										bridgeTile->runAction( CCSequence::create(
											CCMoveBy::create(0.2f,ccp(0,-10.0f)),										
											CCMoveBy::create(0.1f,ccp(0,10.0f)),
											NULL) );
									}
								
							}
						
					}
				}
		  }


		 /////////////////////////////////////////////////

							   
		if( nobitaPos.x > 200 &&  nobitaPos.x < (this->gg_p_mapPixelWidth-GG_SCR_W-120) )
		{
			this->gg_p_layerPlay->setPositionX( 200-nobitaPos.x );
			
			int bgX = this->gg_p_background->getPositionX()-1;				
			this->gg_p_background->setPositionX( ( bgX!=-1000?bgX:-2) );			
		}
		else
		{			
			if( nobitaPos.x > (this->gg_p_mapPixelWidth-65) )
			 {				 
				 this->gg_p_chaien->stopAllActions();
				 this->gg_p_nobita->stopAllActions();				 
				// CCLOG("FINISH");
				 this->gg_p_gameOver = 2;
				 return;
			 }
		}

		//////////////////////////////////////////////

		int bump = this->gg_detechBump(ccp(nobitaPos.x +30, nobitaTargetY+30));
	
						  
		//traffic 1:fire,grass , 2 animal , 3 turtle stone , -1 pie , 0 nothing
		if( bump == 0 && this->gg_p_isNobitaCollisionTraffic < 1 )
		{			
			//nobita run
			this->gg_p_nobita->setPositionY(nobitaTargetY);
			//pie			
			if( this->gg_p_isNobitaCollisionTraffic == -1 )
			{
				this->gg_p_chaien->setPositionX(  chaienPos.x-20 );
               // this->gg_p_nobita->setPositionX(nobitaPos.x+30);
                CCLog("an bánh");
                CCUserDefault::sharedUserDefault()->setIntegerForKey("Health", (CCUserDefault::sharedUserDefault()->getIntegerForKey("Health"))+10);
				this->gg_p_isNobitaCollisionTraffic = 0;

				 if( this->gg_p_isSoundOn )
				 {
					static SimpleAudioEngine *media = SimpleAudioEngine::sharedEngine();
					media->playEffect( "pie.wav" );
				 }
			}
									
			//chaien run
			int chaienBump =  this->gg_detechBump( chaienPos );
			CCDictionary *chaienDict = this->gg_p_map->propertiesForGID(chaienBump);
			//chaien running on a bridge
			if( chaienDict  && (chaienDict->valueForKey("isBridge")->intValue() == 1) )
			 {				 				 				  								
					  CCSprite *bridgeTile = this->gg_p_layerWorld->tileAt( this->gg_coordForPos(chaienPos) );
					  if( bridgeTile->numberOfRunningActions() == 0 )
						{
							//CCLOG("Bridge Tile Animate");
							 bridgeTile->runAction( CCSequence::create(
									CCMoveBy::create(0.2f,ccp(0,-15.0f)),										
									CCMoveBy::create(0.1f,ccp(0,15.0f)),
									NULL) );
					    }				  					 				  				  
			  }	

			  			
			int chaienJumpBump = this->gg_detechBump( ccp(chaienPos.x+92, chaienPos.y) );			  			
		
		
			  if( (chaienJumpBump==0 || this->gg_p_isChaienCollisionTrafic)  && !this->gg_p_isChaienJumpping )
			   {					
				    this->gg_p_isChaienJumpping  = true;
					
					this->gg_p_chaien->stopAllActions();
					this->gg_p_chaien->initWithSpriteFrameName("chai_jump.png");	
					this->gg_p_chaien->setAnchorPoint(ccp(0.5f,0.0f));
					
					CCPoint chaienJumpToPoint = CCPointZero ;

					if(this->gg_p_isChaienCollisionTrafic)
					{
						
						chaienJumpToPoint = ccp(chaienPos.x + 220,chaienPos.y);
					}
					else
					{
						CCDictionary *chaienJumpToDict = (CCDictionary*)(this->gg_p_ChaienJumpPos->getObjects()->objectAtIndex( this->gg_p_chaienNextJumpPos++ ));
						//CCLOG( chaienJumpToDict->valueForKey("y")->getCString() );
						chaienJumpToPoint = ccp( chaienJumpToDict->valueForKey("x")->intValue()+20 , chaienJumpToDict->valueForKey("y")->intValue() - GG_ON_GROUND_DELTA  );
					}					
					
					//CCLOG( "%f,%f" ,chaienJumpToPoint.x,chaienJumpToPoint.y);					
					float dur = (chaienJumpToPoint.x - chaienPos.x)/this->gg_p_chaienJumpSpeed;
					//CCLOG("Chaien jump %f",dur);
					this->gg_p_chaien->runAction( CCSequence::create(
						CCJumpTo::create(dur,chaienJumpToPoint,162,1), 
						this->gg_createChaienAutoJumpCallback(),
						NULL ) );										 					 
					
			  }				  								 			
		}
		else
		{			
			this->gg_p_nobita->stopAllActions();
									
			int bump2 = 0;
			float fallToY = nobitaPos.y;
			do
			{
				bump2 = this->gg_detechBump(ccp(nobitaPos.x-80,fallToY));
				fallToY-=10;
			}
			while( bump2==0 );			
			CCPoint fallPos =  this->gg_p_layerBump->positionAt( this->gg_coordForPos(ccp( nobitaPos.x-80,fallToY)) );
			//CCLOG( "BUMp2 %i At {%f,%f}" , bump2 , fallPos.x,fallPos.y  );
			
			if( bump2>0 )
			{
				this->gg_p_nobita->runAction(  
				CCSpawn::create(
				this->gg_p_nobitaCollisionAnimate,
				CCEaseIn::create( CCMoveTo::create(0.1f,ccp( nobitaPos.x-80, fallPos.y + this->gg_p_tileHeight - GG_ON_GROUND_DELTA )) , 0.05f ),
				this->gg_createNobitaAfterCollisionWallCallback(),
				NULL) );
			}
			else
			{
				this->gg_p_nobita->runAction(  
				CCSpawn::create(
				this->gg_p_nobitaCollisionAnimate,
				CCEaseIn::create( CCMoveTo::create(1.0f,ccp(nobitaPos.x- 80, fallPos.y - GG_ON_GROUND_DELTA )) , 0.2f ),
				NULL) );
			}
			
		   //CCLOG("WALL");

			this->gg_p_isNobitaWalled = true;
			if( !this->gg_p_isChaienJumpping )
			{
				this->gg_p_chaien->stopAllActions();

			}

			this->gg_p_gameOver = 1;

			return;
			
		}		


		if( nobitaPos.x - chaienPos.x < 40 )
		{			
			this->gg_p_nobita->stopAllActions();
			this->gg_p_chaien->stopAllActions();
			this->gg_p_nobita->initWithSpriteFrameName("no_run_8.png");
			this->gg_p_chaien->initWithSpriteFrameName("chai_run_3.png");
			this->gg_p_nobita->setAnchorPoint(CCPointZero);
			this->gg_p_chaien->setAnchorPoint(CCPointZero);
			this->gg_p_chaien->setPosition(ccp( nobitaPos.x-50 , nobitaTargetY ));
			
			this->gg_p_gameOver = 1;
			
			//CCLOG("CHAIEN CATCHED NOBITA");
		}
   }//this->gg_p_gameOver == 0
  else
   {
	   /// GAME OVER	
	    this->unscheduleAllSelectors();
		CCCallFunc *gameOverDelayCall = CCCallFunc::create(this,callfunc_selector(GG_SceneGamePlay::GG_gameOver));
		this->runAction( CCSequence::create( CCDelayTime::create(1.0f),gameOverDelayCall,NULL) );
   }
}

void  GG_SceneGamePlay::GG__nobitaCollisionResponse(CCObject *object)
{
	//CCLOG ("Nobita collsion response : %i " , ((CCInteger*)object)->getValue()  );		
	this->gg_p_isNobitaCollisionTraffic = ((CCInteger*)object)->getValue();	
}

void  GG_SceneGamePlay::GG__chaienCollisionResponse(CCObject *object)
{	
	 if( ((CCInteger*)object)->getValue() == -1 )
	 {
	    //CCLOG("Chaien Eated APie");	
		 if( this->gg_p_isSoundOn )
		 {
			 static SimpleAudioEngine *media = SimpleAudioEngine::sharedEngine();
			 media->playEffect( "pie.wav" );
		 }
		 this->gg_p_chaienJumpSpeed+=2;
	 }
	this->gg_p_isChaienCollisionTrafic = true;
	//CCLOG("Chaien Collision Response");
}


void GG_SceneGamePlay::gg_chaienAutoJumpCallback()
{
	this->gg_p_isChaienCollisionTrafic = false;
	this->gg_p_isChaienJumpping = false;
	this->gg_p_chaien->runAction( this->gg_p_chaienRunAnimate );
	this->gg_p_chaien->runAction( CCRepeatForever::create( CCMoveBy::create(1.0f / 60.0f ,ccp(GG_RUN_SPEED,0)) ) );
	//CCLOG("Chaien Jumped");
		
	if(this->gg_p_isNobitaWalled)
	{
		this->gg_p_chaien->stopAllActions();
	}
	
}
CCCallFunc* GG_SceneGamePlay::gg_createChaienAutoJumpCallback()
{
	return CCCallFunc::create(this,callfunc_selector( GG_SceneGamePlay::gg_chaienAutoJumpCallback ));
}


void GG_SceneGamePlay::gg_nobitaAfterCollisionWallCallback()
{	
	CCSprite *stars =  CCSprite::create();		
	stars->setPosition(ccp(0,40));
	stars->setRotation(-30);
	this->gg_p_nobita->addChild(stars);
	stars->runAction( CCSequence::create(CCRepeat::create( GG_CREATE_ANIMATE_FROM_FRAMES("star",1,2,0.3f) , 10 ),CCHide::create(),NULL) );
}

CCCallFunc* GG_SceneGamePlay::gg_createNobitaAfterCollisionWallCallback()
{
	return CCCallFunc::create(this,callfunc_selector( GG_SceneGamePlay::gg_nobitaAfterCollisionWallCallback ));
}

int GG_SceneGamePlay::gg_detechBump(CCPoint pos)
{
	CCPoint coord =  this->gg_coordForPos( pos ) ;			
	//make sure coord is not out off the map
	if( coord.x > (this->gg_p_mapWidth-1) )
	{
		return -1;
	}
	else if( coord.x<0 ) 
	{
		return -2;
	}

	if( coord.y > (this->gg_p_mapHeight-1) )
	{
		return -3;
	}
	else if( coord.y<0 ) 
	{
		return -4;
	}
	
	  return this->gg_p_layerBump->tileGIDAt( coord );
}

CCPoint GG_SceneGamePlay::gg_coordForPos( CCPoint pos )
{	
	int xCoord = pos.x/this->gg_p_tileWidth;
	int yCoord = (this-> gg_p_mapHeight*this->gg_p_tileHeight - pos.y)/this->gg_p_tileHeight;		
	return ccp(xCoord,yCoord);
}

bool GG_SceneGamePlay::ccTouchBegan( cocos2d::CCTouch * touch, cocos2d::CCEvent * evn )
{			
	if( this->gg_p_gameOver !=0 )
	{		
		return false;
	}
	int bump = this->gg_detechBump( this->gg_p_nobita->getPosition() );
	if( bump!=0 && !this->gg_p_isNobitaJumpping)
	{
		this->gg_p_isNobitaJumpping = true;

		this->gg_p_nobita->stopAction( this->gg_p_nobitaRunAnimate );
		this->gg_p_nobita->initWithSpriteFrameName("no_run_1.png");	
		this->gg_p_nobita->setAnchorPoint(ccp(0.5f,0.0f));

		this->gg_p_nobitaJumpPosY = this->gg_p_nobita->getPositionY();	
		this->gg_p_NobitaJumpingTime = 0;	
		this->gg_p_isNobitaJumpping = true;
	}
	
	return true;
}


void GG_SceneGamePlay::gg_cleanUp()
{
	GG_REMOVE_TOUCH_DELEGATE(this);	
	this->unscheduleAllSelectors();
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);

	this->gg_p_nobitaRunAnimate->release();
	this->gg_p_nobitaRunAnimate=NULL;		

	this->gg_p_nobitaCollisionAnimate->release();
	this->gg_p_nobitaCollisionAnimate = NULL;

	this->gg_p_chaienRunAnimate->release();
	this->gg_p_chaienRunAnimate = NULL;
	
	this->removeAllChildren();	
	
}


void GG_SceneGamePlay::GG_gameOver()
{	
	CCUserDefault::sharedUserDefault()->setIntegerForKey ("GAMEOVER",this->gg_p_gameOver);
	
	if( this->gg_p_gameOver==2 ) //win
	{
		int distance = this->gg_p_nobita->getPositionX()-this->gg_p_chaien->getPositionX();
		//CCLOG( "DISTANCE %f",distance );
		CCUserDefault::sharedUserDefault()->setIntegerForKey("DISTANCE", distance );
	}
		
	this->gg_cleanUp();	
	CCDirector::sharedDirector()->replaceScene( GG_SceneGameOver::create() );
}



//////////////////// TRAFFIC //////////////////////////////////


bool GG_traffic::init()
{
	if(!GG_Sprite::init())
	{
		return false;
	}
	
	CCUserDefault *user = CCUserDefault::sharedUserDefault();
	this->gg_p_isSoundOn = user->getIntegerForKey("BG_SOUND",1)==1;

	this->setVisible(false);

	this->gg_p_dict = NULL;
	this->gg_p_contentWidth = 0;
	this->gg_p_contentHeight = 0;

	return true;
}
void GG_traffic::gg_initWithDict( CCDictionary *dict )
{
	this->gg_p_dict = dict;
	
	static CCNotificationCenter *notifier = CCNotificationCenter::sharedNotificationCenter();
	notifier->addObserver( this , callfuncO_selector( GG_traffic::gg_checkCollision ) , "CHECK_COLLISION" , NULL );	
}

void GG_traffic::gg_active( )
{			
	SimpleAudioEngine *media = SimpleAudioEngine::sharedEngine();

	int x =  this->gg_p_dict->valueForKey("x")->intValue();
	int y =  this->gg_p_dict->valueForKey("y")->intValue();
	
		
	switch( this->gg_p_dict->valueForKey("type")->intValue() )
	{							
		case -1:
		  {
			  this->initWithSpriteFrameName("pie.png");
			  this->gg_p_contentWidth = 30;
			  this->gg_p_contentHeight = 30;			  
			  this->setAnchorPoint(ccp(0.5f,0.0f));
			  this->setPosition(ccp(x,y));

			  this->setScale(0.5);
			  this->runAction( CCRepeatForever::create( CCSequence::create(CCScaleTo::create(0.5f,0.8f),CCScaleTo::create(1.0f,0.6f),NULL) ) );
			 
			  break;
		  }

		case 1: //fires
			{				
			  if(rand()%2==0)								
				{
					this->initWithSpriteFrameName("f_4.png");	
					this->setAnchorPoint(ccp(0.5f,0.0f));
					this->setPosition(ccp(x,y-20));

					CCSprite *fireAnimate = CCSprite::createWithSpriteFrameName("f_1.png");				
					fireAnimate->runAction( CCRepeatForever::create(  GG_CREATE_ANIMATE_FROM_FRAMES("f",1,3,0.1f) ));
					fireAnimate->setAnchorPoint(CCPointZero);
					fireAnimate->setPosition(ccp(0,16));
					this->addChild(fireAnimate);

					this->gg_p_contentWidth =  5;
					this->gg_p_contentHeight = 55; 
				}
				else
				{				
					this->initWithSpriteFrameName("big_grass.png");
					this->gg_p_contentWidth = 5;
					this->gg_p_contentHeight = 40;
					this->setAnchorPoint(ccp(0.5f,0.0f));
					this->setPosition(ccp(x,y-5));
				}

				break;
			}				
		case 2:
			{																								
				char type[20] = {0};
				unsigned int short animalType = (rand()%3+1);
				sprintf( type, "ani_%i" , animalType );
				char firstFrameName[20];
				sprintf( firstFrameName,"%s_%i.png", type,1  );
				
				this->initWithSpriteFrameName(firstFrameName);				
				this->runAction( CCRepeatForever::create( GG_CREATE_ANIMATE_FROM_FRAMES( type,1,6,0.08f) ) );				
				
				this->gg_p_contentWidth =  this->boundingBox().size.width-10;
				this->gg_p_contentHeight = this->boundingBox().size.height-15; 

				this->setAnchorPoint(ccp(0.5f,0.0f));
				this->setPosition(ccp(x,y-20));
				int move = this->gg_p_dict->valueForKey("move")->intValue();	
				if(move==0)
				{					
					move = 400;
				}
				CCCallFunc *stopAllAction = CCCallFunc::create(this,callfunc_selector(GG_Sprite::stopAllActions));
				this->runAction( CCSequence::create( 
					CCMoveBy::create( move/GG_TRAFFIC_SPEED ,ccp(-move,0)) ,
					stopAllAction ,
					NULL ) );

				//CCLOG("%i",animalType);
				if( animalType == 3)
				{
					if( this->gg_p_isSoundOn )
					{
						media->playEffect("dinosaur_2.wav");
					}
					this->gg_p_contentWidth-=20;
				}
				else
				{
					if( animalType == 2  )
					{
						if( this->gg_p_isSoundOn )
						{
							media->playEffect("dinosaur_1.wav");
						}
					}
					else
					{
						if( animalType == 1)
						{
							if( this->gg_p_isSoundOn )
							{
								media->playEffect("dinosaur_3.wav");
							}
						}
					}
				}

				break;
			}
		case 3:
			{					
				this->initWithSpriteFrameName("turtle_1.png");												
				this->setAnchorPoint(ccp(0.5f,0.0f));
				this->setPosition(ccp(x,y-20));
				
				this->gg_p_contentWidth =  8;
				this->gg_p_contentHeight = 8; 
				
				if( rand()%2==0 ) this->setScaleX(-1);
				
				this->runAction( 
						  CCSequence::create(
						  CCMoveBy::create(0.2f,ccp(0,2)),
						  CCMoveBy::create(0.1f,ccp(0,-2)),
						  CCMoveBy::create(0.2f,ccp(0,2)),
						  CCMoveBy::create(0.1f,ccp(0,-2)),
						  GG_CREATE_ANIMATE_FROM_FRAMES("turtle",2,6,0.1f),NULL));

				
																	
				break;
			}
		 
	}	
		
}


void GG_traffic::gg_checkCollision(CCObject *obj)
{	
	static CCNotificationCenter *notifier = CCNotificationCenter::sharedNotificationCenter();

	if( !this->isVisible() )
	{		
		if( this->getParent()->getPositionX() +  this->gg_p_dict->valueForKey("x")->intValue() < 900  )
		{
			CCLog("ACTIVE A TRAFFIC OBJECT");
			this->setVisible(true);
			this->gg_active();
		}
		return;
	}
	else
	{		
		if( this->getParent()->getPositionX() + this->getPositionX()  < -150  )
		{
			
			this->removeAllChildren();
			notifier->removeAllObservers(this);
			this->gg_p_dict = NULL;
			this->gg_p_contentWidth = 0;
			this->gg_p_contentHeight = 0;			
			this->stopAllActions();
			this->removeFromParent();
			return;
		}
	}
	
	CCArray *data = (CCArray*)obj;	
	
	CCPoint pos = this->getPosition();
	//Check nobita collision
	int _health = CCUserDefault::sharedUserDefault()->getIntegerForKey("Health");
    CCLog("truowcs---%d",CCUserDefault::sharedUserDefault()->getIntegerForKey("Health"));
	if( CCRect(pos.x,pos.y,this->gg_p_contentWidth,this->gg_p_contentHeight).intersectsRect( ((CCSprite*)data->objectAtIndex(0))->boundingBox() ) )
	{		
		CCLOG("Collision Detected");
       
        
		//pie
/*-----*/if(  this->gg_p_dict->valueForKey("type")->intValue() == -1 ) //pie eated
		 {
             notifier->postNotification("NOBITA_COLLISION" , CCInteger::create( this->gg_p_dict->valueForKey("type")->intValue() ) );
			 notifier->removeAllObservers(this);
			 this->gg_p_dict = NULL;
			 this->gg_p_contentWidth = 0;
			 this->gg_p_contentHeight = 0;			
			 this->stopAllActions();
			 this->removeFromParent();		
			 return;
		 }else
         {
             _health=_health-10;
              CCUserDefault::sharedUserDefault()->setIntegerForKey("Health",_health);
             if(_health==0)
             {
                 notifier->postNotification("NOBITA_COLLISION" , CCInteger::create( this->gg_p_dict->valueForKey("type")->intValue() ) );
             }
/*----*/ }//else pie
        CCLog("sau---%d",CCUserDefault::sharedUserDefault()->getIntegerForKey("Health"));
	}

	//chaien
	int dis =  pos.x - ((CCInteger*)data->objectAtIndex(1))->getValue() ;
	if( dis<180 && dis>0 )
	{
		//CCLOG("Chaien collision");		
		notifier->postNotification("CHAIEN_COLLISION",CCInteger::create( this->gg_p_dict->valueForKey("type")->intValue() ) );
		//pie
		 if(  this->gg_p_dict->valueForKey("type")->intValue() == -1 ) //pie eated
		 {
			 this->setPositionX(this->getPositionX()-50);
		 }
	}

}	

GG_traffic::~GG_traffic()
{
	this->stopAllActions();
	this->removeAllChildren();
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);	
}