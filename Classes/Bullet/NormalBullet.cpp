#include "NormalBullet.h"

USING_NS_CC;

NormalBullet::NormalBullet(){

}

NormalBullet::~NormalBullet(){

}

bool NormalBullet::init() {
	if (!Sprite::init()) {
		return false;
	}

	winSize = Director::getInstance()->getWinSize();

	return true;
}