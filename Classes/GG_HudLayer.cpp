
#include "GG_HudLayer.h"
USING_NS_CC;
using namespace cocos2d;

bool GG_HudLayer::init()
{	
	if(!CCLayer::init()) { return false; }
    
	return true;	
}	
void GG_HudLayer::updateHealth(int heal)
{
    health=heal;
    healBar->setPercentage(health);
}
GG_HudLayer::GG_HudLayer()
{
    this->health =100;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    healBar= CCProgressTimer::create(CCSprite::create("loading.png"));
    healBar->setScaleX(0.3);
    healBar->setType(kCCProgressTimerTypeBar);
    healBar->setBarChangeRate(ccp(1,0));
    healBar->setMidpoint(ccp(0,0));
    healBar->setPercentage(health);
    healBar->setPosition(115,winSize.height-20);
    this->addChild(healBar);
    
    CCSprite *nobita = CCSprite::create("nobita.png");
    nobita->setPosition(ccp(30,winSize.height-20));
    nobita->setScale(0.7);
    //nobita->setFlipY(360);
    this->addChild(nobita);

}