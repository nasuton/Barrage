#include "MyShip.h"

USING_NS_CC;

MyShip::MyShip() {

}

MyShip::~MyShip() {

}

MyShip* MyShip::create(std::string _imageFileName) {
	auto ret = new MyShip();
	if (ret &&  ret->init(_imageFileName)) {
		ret->autorelease();

		return ret;
	}

	CC_SAFE_RELEASE(ret);

	return nullptr;
}

bool MyShip::init(std::string _imageFileName) {
	if (!Sprite::initWithFile(_imageFileName)) {
		return false;
	}

	auto baseSprite = Sprite::create(_imageFileName);
	auto textureSource = baseSprite->getTexture();
	textureSource->setAliasTexParameters();

	//一コマの大きさを取得する
	float frameWidth = baseSprite->getContentSize().width / 3.0f;
	float frameHeight = baseSprite->getContentSize().height;
	this->setContentSize(Size(frameWidth, frameWidth));

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.3f);
	animation->setRestoreOriginalFrame(true);

	for (int i = 0; i < 3; i++) {
		float _width = frameWidth * i;
		animation->addSpriteFrameWithTexture(textureSource, Rect(_width, 0.0f, frameWidth, frameHeight));
	}

	this->runAction(RepeatForever::create(Animate::create(animation)));

	this->setScale(2.0f);

	return true;
}

void MyShip::Move(Vec2 _vec) {
	Vec2 newPos;
	newPos = this->getPosition() + (_vec * -1);
}