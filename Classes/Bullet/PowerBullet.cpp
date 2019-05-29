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

	Size winSize = Director::getInstance()->getWinSize();
	Vec2 endPos = Vec2(0, winSize.height * 0.3f);
	MoveBy* move = MoveBy::create(0.5f, endPos);
	RemoveSelf* remove = RemoveSelf::create();
	this->runAction(Sequence::create(move, remove, NULL));

	return true;
}