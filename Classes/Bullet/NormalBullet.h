#ifndef __NORMALBULLET_H__
#define __NORMALBULLET_H__

#include "cocos2d.h"

class NormalBullet : public cocos2d::Sprite {
public:
	NormalBullet();
	~NormalBullet();

	CREATE_FUNC(NormalBullet);

private:
	bool init();

private:
	cocos2d::Size winSize;
};


#endif