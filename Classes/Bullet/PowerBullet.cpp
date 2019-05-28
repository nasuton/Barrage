#include "PowerBullet.h"

USING_NS_CC;

PowerBullet::PowerBullet(){

}

PowerBullet::~PowerBullet() {

}

PowerBullet* PowerBullet::create(std::string _imageFileName) {
	auto ret = new PowerBullet();
	if (ret &&  ret->init(_imageFileName)) {
		ret->autorelease();

		return ret;
	}

	CC_SAFE_RELEASE(ret);

	return nullptr;
}

bool PowerBullet::init(std::string _imageFileName) {
	if (!Sprite::initWithFile(_imageFileName)) {
		return false;
	}

	return true;
}