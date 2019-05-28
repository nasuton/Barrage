#ifndef __MYSHIP_H__
#define __MYSHIP_H__

#include "cocos2d.h"

class MyShip : public cocos2d::Sprite {
public:
	MyShip();
	~MyShip();

	static MyShip* create(std::string _imageFileName);

	void Move(cocos2d::Vec2 _vec);

	void SetLimit(cocos2d::Size _limitSize);

	void Shot(float _dt, cocos2d::Layer* _target);

private:
	bool init(std::string _iamgeFileName);

	void Limit(cocos2d::Vec2 _pos);

private:
	//移動制限用
	cocos2d::Size limitSize;
	cocos2d::Vec2 minLimitPos;
	cocos2d::Vec2 maxLimitPos;

	//打つ間隔用
	float intervalTime;
};

#endif