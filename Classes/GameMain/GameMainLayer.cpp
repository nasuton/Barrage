#include "GameMain/GameMainLayer.h"
#include "bullet/PowerBullet.h"

USING_NS_CC;

GameMainLayer::GameMainLayer() {
	touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameMainLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameMainLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameMainLayer::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(GameMainLayer::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	previousPos = Vec2::ZERO;
	playerPos = Vec2::ZERO;
}

GameMainLayer::~GameMainLayer() {
	Director::getInstance()->getEventDispatcher()->removeEventListener(touchListener);
}

bool GameMainLayer::init() {
	if (!Layer::init()) {
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();

	player = MyShip::create("ship/airship01.png");
	player->setPosition(winSize.width * 0.5f, winSize.height * 0.5f);
	player->SetLimit(winSize);
	this->addChild(player);

	Sprite* enemy = Sprite::create("enemy/enemy01.png");
	enemy->setPosition(winSize.width * 0.5, winSize.height * 0.8f);
	this->addChild(enemy);

	this->scheduleUpdate();

	return true;
}

void GameMainLayer::update(float _dt) {
	player->Shot(_dt, this);
}

bool GameMainLayer::onTouchBegan(Touch* _touch, Event* _event) {
	previousPos = _touch->getLocation();

	return true;
}

void GameMainLayer::onTouchMoved(Touch* _touch, Event* _event) {
	Vec2 nowVec;
	Vec2 nowPos = _touch->getLocation();
	nowVec = previousPos - nowPos;
	player->Move(nowVec);
	previousPos = _touch->getLocation();
}

void GameMainLayer::onTouchEnded(Touch* _touch, Event* _event) {

}

void GameMainLayer::onTouchCancelled(Touch* _touch, Event* _event) {

}