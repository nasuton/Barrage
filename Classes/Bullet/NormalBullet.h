#ifndef __NORMALBULLET_H__
#define __NORMALBULLET_H__

#include "cocos2d.h"

class NormalBullet : public cocos2d::Sprite {
public:
	NormalBullet();
	~NormalBullet();

	static NormalBullet* create(std::string _imageFileName);

private:
	bool init(std::string _imageFileName);
};


#endif