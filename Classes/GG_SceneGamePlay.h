#ifndef ___GG_SCENE_GAMEPLAY___
#define ___GG_SCENE_GAMEPLAY___

#include "cocos2d.h"
#include "GG_DEFINE.h"
#include "GG_HudLayer.h"
using namespace cocos2d;


class GG_SceneGamePlay : public cocos2d::CCScene, public cocos2d::CCTouchDelegate
{
public:
	
	void gg_cleanUp();

	bool init();	
	
	bool ccTouchBegan( cocos2d::CCTouch * touch, cocos2d::CCEvent * evn ); 	
	
	void gg_run();
	void gg_runInterVal(float delay);
	
	CCPoint gg_coordForPos(CCPoint pos);
	int gg_detechBump(CCPoint pos);
	GG_HudLayer *hud;

	void gg_chaienAutoJumpCallback();
	CCCallFunc *gg_createChaienAutoJumpCallback();

	void gg_nobitaAfterCollisionWallCallback();
	CCCallFunc *gg_createNobitaAfterCollisionWallCallback();

	void GG__nobitaCollisionResponse(CCObject *object);
	void GG__chaienCollisionResponse(CCObject *object);


	void GG_gameOver();

	//menu
	void gg_buttomMenuTapped( CCObject *obj );


	CREATE_FUNC( GG_SceneGamePlay );

protected:
	
	GG_Sprite *gg_p_nobita;
	GG_Sprite *gg_p_chaien;
	
	CCTMXTiledMap *gg_p_map;
	CCTMXLayer    *gg_p_layerBump;
	CCTMXLayer    *gg_p_layerWorld;
	int   gg_p_mapWidth;
	int   gg_p_mapHeight;
	int   gg_p_tileWidth;
	int   gg_p_tileHeight;
	int   gg_p_mapPixelWidth;
	
	bool gg_p_isNobitaJumpping;		
	int gg_p_nobitaJumpPosY;
	unsigned int short gg_p_NobitaJumpingTime;

	bool gg_p_isNobitaFalling;
	int short gg_p_isNobitaCollisionTraffic;
	

	//chaien
	bool gg_p_isChaienJumpping;
	CCTMXObjectGroup *gg_p_ChaienJumpPos;
	unsigned int gg_p_chaienNextJumpPos;
	bool gg_p_isChaienCollisionTrafic;

	/////Animate
	CCRepeatForever* gg_p_nobitaRunAnimate;	
	CCAnimate *gg_p_nobitaCollisionAnimate;

	CCRepeatForever* gg_p_chaienRunAnimate;	
		
	//Layers
	CCLayer *gg_p_layerPlay;
	CCSpriteBatchNode *gg_p_background;	

	bool gg_p_isNobitaWalled;	
	CCSpriteBatchNode *gg_p_preview;
	unsigned int gg_p_chaienJumpSpeed;

	unsigned int short gg_p_gameOver;


	bool gg_p_isSoundOn;
};




class GG_traffic : public GG_Sprite
{

public:
	bool init();
	void gg_initWithDict( CCDictionary *dict );	
	void gg_checkCollision(CCObject *obj);	
	void gg_active();	
	CREATE_FUNC( GG_traffic );
	~GG_traffic();

protected:			
	CCDictionary* gg_p_dict;
	int gg_p_contentWidth;
	int gg_p_contentHeight;

	bool gg_p_isSoundOn;

};

#endif 