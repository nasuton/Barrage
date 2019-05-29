#include "MyShip.h"
#include "Bullet/NormalBullet.h"
#include "Bullet/PowerBullet.h"

USING_NS_CC;

#define NORMALBULLETINTERVAL 0.08f
#define POWERBULLETINTERVAL  0.2f

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

	limitSize = Size::ZERO;
	intervalTime = 0.0f;

	Sprite* baseSprite = Sprite::create(_imageFileName);
	Texture2D* textureSource = baseSprite->getTexture();
	textureSource->setAliasTexParameters();

	//一コマの大きさを取得する
	float frameWidth = baseSprite->getContentSize().width / 3.0f;
	float frameHeight = baseSprite->getContentSize().height;
	this->setContentSize(Size(frameWidth, frameWidth));

	Animation* animation = Animation::create();
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

void MyShip::Limit(Vec2 _pos) {
	_pos.clamp(minLimitPos, maxLimitPos);
	this->setPosition(_pos);
}

void MyShip::Move(Vec2 _vec) {
	Vec2 newPos;
	newPos = this->getPosition() + (_vec * -1);
	Limit(newPos);
}

void MyShip::SetLimit(Size _limitSize) {
	limitSize = _limitSize;
	//移動できる最小値及び最大値を設定
	minLimitPos = Vec2(0.0f + this->getContentSize().width * 0.8f, 0.0f + this->getContentSize().height);
	maxLimitPos = Vec2(limitSize.width - (this->getContentSize().width * 0.8f), limitSize.height - this->getContentSize().height);
}

void MyShip::Shot(float _dt, Layer* _target) {
	intervalTime += _dt;

	//ノーマル
	if (NORMALBULLETINTERVAL <= intervalTime) {
		intervalTime = 0.0f;

		NormalBullet* bullet = NormalBullet::create("bullet/normal.png");
		bullet->setPosition(this->getPosition().x, this->getPosition().y + 50.0f);
		_target->addChild(bullet);
	}

	//パワー
	/*
	if (POWERBULLETINTERVAL <= intervalTime) {
		intervalTime = 0.0f;

		PowerBullet* bullet = PowerBullet::create("bullet/missile.png");
		bullet->setPosition(this->getPosition().x, this->getPosition().y + 50.0f);
		_target->addChild(bullet);
	}
	*/
}