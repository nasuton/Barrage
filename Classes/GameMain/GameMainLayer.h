#ifndef __GAMEMAINLAYER_H__
#define __GAMEMAINLAYER_H__

#include "System/LayerBase/LayerBase.h"
#include "Ship/MyShip.h"

class GameMainLayer : public LayerBase {
private:
	//タッチイベント登録用
	cocos2d::EventListenerTouchOneByOne* touchListener;

	cocos2d::Vec2 previousPos;
	cocos2d::Vec2 playerPos;

	MyShip* player;

public:
	GameMainLayer();
	~GameMainLayer();

	CREATE_FUNC(GameMainLayer);
private:
	bool init();
	void update(float _dt);

	//タッチイベント関数
	bool onTouchBegan(cocos2d::Touch* _touch, cocos2d::Event* _event);
	void onTouchMoved(cocos2d::Touch* _touch, cocos2d::Event* _event);
	void onTouchEnded(cocos2d::Touch* _touch, cocos2d::Event* _event);
	void onTouchCancelled(cocos2d::Touch* _touch, cocos2d::Event* _event);
};

#endif