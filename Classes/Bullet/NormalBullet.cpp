#include "NormalBullet.h"

USING_NS_CC;

NormalBullet::NormalBullet(){

}

NormalBullet::~NormalBullet(){

}

NormalBullet* NormalBullet::create(std::string _imageFileName) {
	NormalBullet* ret = new NormalBullet();
	if (ret && ret->init(_imageFileName)) {
		ret->autorelease();

		return ret;
	}

	CC_SAFE_RELEASE(ret);

	return nullptr;
}

bool NormalBullet::init(std::string _imageFileName) {
	if (!Sprite::initWithFile(_imageFileName)) {
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();
	Vec2 endPos = Vec2(0, winSize.height);
	MoveBy* move = MoveBy::create(0.5f, endPos);
	RemoveSelf* remove = RemoveSelf::create();
	this->runAction(Sequence::create(move, remove, NULL));

	return true;
}