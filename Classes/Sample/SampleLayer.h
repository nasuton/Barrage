#ifndef __SAMPLELAYER_H__
#define __SAMPLELATER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "System/LayerBase/LayerBase.h"
#include "System/Collision/CollisionDetaction.h"

class SampleLayer : public LayerBase {
private:
	CC_SYNTHESIZE_RETAIN(cocos2d::Layer*, other, Layer);
	cocos2d::Label* label;
	cocos2d::SpriteBatchNode* node;
	CollisionDetaction* detection;
	double m_max;
	int num;
	bool use;
	int scale;
	cocos2d::extension::ScrollView* scroll;
	cocos2d::EventListenerTouchOneByOne* touchListener;

public:
	SampleLayer();
	virtual ~SampleLayer();

	static SampleLayer* create(int _num, bool _use, int _scale);

	void DetectCollision(CollisionNode* collisionObject1,
		                 CollisionNode* collisionObject2);

private:
	bool init(int _num, bool _use, int _scale);
	void update(float dt);

	bool onTouchBegan(cocos2d::Touch* _touch, cocos2d::Event* _event);
	void onTouchMoved(cocos2d::Touch* _touch, cocos2d::Event* _event);
	void onTouchEnded(cocos2d::Touch* _touch, cocos2d::Event* _event);
	void onTouchCancelled(cocos2d::Touch* _touch, cocos2d::Event* _event);

	cocos2d::MenuItemSprite* generateButton(const std::string& _caption,
		                                    const cocos2d::ccMenuCallback& _callback);
};

#endif