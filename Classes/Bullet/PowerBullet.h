#ifndef __POWERBULLET_H__
#define __POWERBULLET_H__

#include "cocos2d.h"

class PowerBullet : public cocos2d::Sprite {
public:
	PowerBullet();
	~PowerBullet();

	static PowerBullet* create(std::string _imageFileName);

private:
	bool init(std::string _imageFileName);
};


#endif