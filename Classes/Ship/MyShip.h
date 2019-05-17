#ifndef __MYSHIP_H__
#define __MYSHIP_H__

#include "cocos2d.h"

class MyShip : public cocos2d::Sprite {
public:
	MyShip();
	~MyShip();

	static MyShip* create(std::string _imageFileName);

	void Move(cocos2d::Vec2 _vec);

private:
	bool init(std::string _iamgeFileName);
};

#endif