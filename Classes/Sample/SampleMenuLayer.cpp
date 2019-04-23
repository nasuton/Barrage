#include "Sample/SampleMenuLayer.h"
#include "System/SceneManager/SceneManager.h"

#pragma execution_character_set("utf-8")

USING_NS_CC;

SampleMenuLayer::SampleMenuLayer() {

}

SampleMenuLayer::~SampleMenuLayer() {

}

bool SampleMenuLayer::init() {
	if (!Layer::init()) {
		return false;
	}

	auto win = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	Vector<MenuItem*> list;

	auto spriteNumLabel = Label::createWithSystemFont("Sprite", "Arrial", 20.0f);
	spriteNumLabel->setPosition(75.0f, win.height - 50.0f);
	spriteNumLabel->setColor(Color3B::WHITE);
	this->addChild(spriteNumLabel);

	auto isTreeUseLabel = Label::createWithSystemFont("4分木利用", "Arrial", 20.0f);
	isTreeUseLabel->setPosition(200.0f, win.height - 50.0f);
	isTreeUseLabel->setColor(Color3B::WHITE);
	this->addChild(isTreeUseLabel);

	auto fieldSizeLabel = Label::createWithSystemFont("フィールド", "Arrial", 20.0f);
	fieldSizeLabel->setPosition(340.0f, win.height - 50.0f);
	fieldSizeLabel->setColor(Color3B::WHITE);
	this->addChild(fieldSizeLabel);

	for (int i = 100; i <= 500; i += 100) {
		auto callback = [this, i](Ref *ref) {
			num = i;
			CCLOG("num: %d", i);
			for (auto m : nums) {
				if (m != ref) {
					m->setOpacity(140);
				}
				else {
					m->setOpacity(255);
				}
			}
		};
		auto str = StringUtils::format("%d個", i);
		auto menu = generateButton(str, callback);
		if (i == 100) {
			num = i;
			menu->setOpacity(255);
		}
		else {
			menu->setOpacity(140);
		}
		menu->setPosition(menu->getContentSize().width / 2,
			win.height - 50 - i);
		list.pushBack(menu);
		nums.pushBack(menu);
	}

	// 4分木使うかどうか
	for (bool i : {true, false}) {
		auto callback = [this, i](Ref *ref) {
			usefortree = i;
			CCLOG("useForTree: %s", i ? "true" : "false");
			for (auto m : uses) {
				if (m != ref) {
					m->setOpacity(140);
				}
				else {
					m->setOpacity(255);
				}
			}
		};
		auto str = StringUtils::format("%s", i ? "する" : "しない");
		auto menu = generateButton(str, callback);
		if (i) {
			usefortree = i;
			menu->setOpacity(255);
		}
		else {
			menu->setOpacity(140);
		}
		menu->setPosition(menu->getContentSize().width * 1.5,
			win.height - 50 - (i ? 100 : 200));
		list.pushBack(menu);
		uses.pushBack(menu);
	}

	// フィールド
	for (int i : {1, 2, 3}) {
		auto callback = [this, i](Ref *ref) {
			winSize = i;
			for (auto m : winSizes) {
				if (m != ref) {
					m->setOpacity(140);
				}
				else {
					m->setOpacity(255);
				}
			}
		};
		auto str = StringUtils::format("%d倍", i);
		auto menu = generateButton(str, callback);
		if (i == 1) {
			winSize = i;
			menu->setOpacity(255);
		}
		else {
			menu->setOpacity(140);
		}
		menu->setPosition(menu->getContentSize().width * 2.5,
			win.height - 50 - (i * 100));
		list.pushBack(menu);
		winSizes.pushBack(menu);
	}

	auto callback = [this](Ref *ref) {
		//auto scene = SampleMenuLayer::createScene(num, usefortree, winSize);
		//Director::getInstance()->replaceScene(scene);
		auto scene = SceneManager::CreateSampleScene(num, usefortree, winSize);
		Director::getInstance()->replaceScene(scene);
	};
	auto go = generateButton("Go", callback);
	go->setPosition(win.width - 130, win.height - 50);
	list.pushBack(go);

	auto _menu = Menu::createWithArray(list);
	_menu->setContentSize(win);
	_menu->setAnchorPoint(Point::ZERO);
	_menu->setPosition(Point::ZERO);
	this->addChild(_menu);

	return true;
}

MenuItemSprite* SampleMenuLayer::generateButton(const std::string& _caption, const cocos2d::ccMenuCallback& _collback) {
	auto generate = [this](const std::string &cap) {
		auto spr = Sprite::create("sample/button.png");
		auto size = spr->getContentSize();
		auto text = Label::createWithSystemFont(cap, "Arial", 30);
		text->setPosition(Point(size.width / 2, size.height / 2));
		text->setColor(Color3B::BLACK);
		spr->addChild(text);
		return spr;
	};
	auto on = generate(_caption);
	auto select = generate(_caption);
	auto disable = generate(_caption);
	select->setColor(Color3B(200, 230, 230));
	disable->setColor(Color3B::GRAY);
	return MenuItemSprite::create(on, select, disable, _collback);
}