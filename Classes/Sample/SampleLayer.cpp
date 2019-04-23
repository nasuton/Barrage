#include "Sample/SampleLayer.h"
#include "System/Collision/CollisionComponent.h"
#include "System/Collision/CollisionUtils.h"
#include "System/SceneManager/SceneManager.h"

USING_NS_CC;

SampleLayer::SampleLayer() 
:m_max(0)
,num(100)
,use(false)
,scale(1)
,scroll(nullptr)
,node(nullptr)
,detection(nullptr)
,label(nullptr)
,other(nullptr)
,touchListener(nullptr){
	touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(SampleLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(SampleLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SampleLayer::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(SampleLayer::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

SampleLayer::~SampleLayer() {
	Director::getInstance()->getEventDispatcher()->removeEventListener(touchListener);
	CC_SAFE_RELEASE_NULL(scroll);
	CC_SAFE_RELEASE_NULL(node);
	CC_SAFE_RELEASE_NULL(other);
	CC_SAFE_RELEASE_NULL(label);
	CC_SAFE_RELEASE_NULL(touchListener);
}

SampleLayer* SampleLayer::create(int _num, bool _use, int _scale) {
	auto ret = new SampleLayer();

	if (ret && ret->init(_num, _use, _scale)) {
		ret->autorelease();

		return ret;
	}
	else {
		CC_SAFE_RELEASE_NULL(ret);
	}

	return nullptr;
}

void SampleLayer::DetectCollision(CollisionNode * collisionObject1,
	                              CollisionNode * collisionObject2) {
	auto node1 = collisionObject1->getNode();
	auto node2 = collisionObject2->getNode();

	if (CollisionUtils::intersectRect(node1, node2) == false) {
		return;
	}

	node1->setColor(Color3B::RED);
	node2->setColor(Color3B::RED);
}

bool SampleLayer::init(int _num, bool _use, int _scale) {
	if (!Layer::init()) {
		return false;
	}

	num = _num;
	use = _use;
	scale = _scale;
	srand((unsigned int)time(nullptr));
	auto win = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto m_scroll = cocos2d::extension::ScrollView::create(win);
	m_scroll->setMaxScale(2.0f);
	m_scroll->setMinScale(0.5f);

	other = Layer::create();
	other->retain();
	label = Label::createWithSystemFont("test", "Arial", 40.0f);
	label->setColor(Color3B::BLUE);
	label->setPosition(win.width * 0.5f, win.height - 50.0f);
	label->retain();
	other->addChild(label);

	auto menuItem = generateButton("Back", [](Ref* _ref) {
		auto scene = SceneManager::CreateSampleMenuScene();
		Director::getInstance()->replaceScene(scene);
	});

	auto menu = Menu::createWithItem(menuItem);
	menu->setContentSize(menuItem->getContentSize());
	menu->setAnchorPoint(Point::ANCHOR_MIDDLE);
	menu->setPosition(win.width - menu->getContentSize().width * 0.5f,
		              win.height - 50.0f);
	other->addChild(menu);

	win = win * scale;
	m_scroll->setContentSize(win);
	m_scroll->setTouchEnabled(true);
	m_scroll->setPosition(Point::ZERO);
	m_scroll->setAnchorPoint(Point::ZERO);
	this->addChild(m_scroll);

	node = SpriteBatchNode::create("sample/sprite.png");
	node->retain();
	node->setContentSize(win);
	m_scroll->setContainer(node);
	scroll = m_scroll;
	scroll->retain();

	auto call1 = CC_CALLBACK_2(SampleLayer::DetectCollision, this);
	detection = CollisionDetaction::getDefaultDetaction();
	detection->init(node, 4, true, call1);

	auto sprite = Sprite::create("sample/sprite.png");
	sprite->setPosition(win.width * 0.5f + origin.x,
		                win.height * 0.5f + origin.y);
	sprite->addComponent(CollisionComponent::create(2));
	node->addChild(sprite);

	for (int i = 0; i < num; i++) {
		auto sprite = Sprite::create("sample/sprite.png");
		auto pos = Point(
			win.width * 0.5f + CCRANDOM_MINUS1_1() * win.width * 0.5f + origin.x,
			win.height * 0.5f + CCRANDOM_MINUS1_1() * win.height * 0.5f + origin.y);
		sprite->setScaleX(CCRANDOM_0_1() * 0.3f + 0.1f);
		sprite->setScaleY(CCRANDOM_0_1() + 0.1f);
		sprite->setPosition(pos);
		sprite->setRotation(CCRANDOM_MINUS1_1() * 360.0f);
		auto in = rand() % 2 + 1;
		sprite->addComponent(CollisionComponent::create(in));
		node->addChild(sprite);
	}

	auto _sprite = Sprite::create("sample/sprite.png");
	_sprite->setPosition(win.width / 4.0f + origin.x,
		                 win.height / 4.0f + origin.y);
	_sprite->setRotation(90.0f);
	auto rotate = RotateBy::create(1.0f, 45.0f);
	auto repeat = RepeatForever::create(rotate);
	_sprite->addComponent(CollisionComponent::create(2));
	node->addChild(_sprite);
	_sprite->runAction(repeat);

	this->schedule(
		[this](float f) {
		auto time = Director::getInstance()->getSecondsPerFrame();
		auto str = StringUtils::format("衝突判定:%fs\n全体:%fs", m_max, time);
		label->setString(str);
	},
	0.1f, "display");

	return true;
}

void SampleLayer::update(float dt) {
	Layer::update(dt);
	for (auto n : node->getChildren()) {
		n->setColor(Color3B::WHITE);
	}

	double time = utils::gettime();
	if (use) {
		detection->update();
	}
	else {
		auto nodes = node->getChildren();
		auto size = nodes.size();
		for (int i = 0; i < size; i++) {
			auto node1 = nodes.at(i);
			for (int k = i + 1; k < size; k++) {
				auto node2 = nodes.at(k);
				if (CollisionUtils::intersectRect(node1, node2)) {
					node1->setColor(Color3B::RED);
					node2->setColor(Color3B::RED);
				}
			}
		}
	}
	time = utils::gettime() - time;
	m_max = time;
}

bool SampleLayer::onTouchBegan(cocos2d::Touch* _touch, cocos2d::Event* _event) {
	auto pos = node->convertToNodeSpace(_touch->getLocation());
	for (auto _node : node->getChildren()) {
		if (CollisionUtils::containsPoint(_node, pos, Size::ZERO)) {
			_node->retain();
			setUserData(_node);
			scroll->setTouchEnabled(false);
			return true;
		}
	}
	return true;
}

void SampleLayer::onTouchMoved(cocos2d::Touch* _touch, cocos2d::Event* _event) {
	auto pos = node->convertToNodeSpace(_touch->getLocation());
	if (auto _node = (Node*)(getUserData())) {
		if (_node->getParent() == nullptr) {
			_node->release();
			setUserData(nullptr);
			return;
		}
		_node->setPosition(pos);
		return;
	}
}

void SampleLayer::onTouchEnded(cocos2d::Touch* _touch, cocos2d::Event* _event) {
	if (auto _node = (Node*)(getUserData())) {
		_node->release();
		setUserData(nullptr);
	}
	scroll->setTouchEnabled(true);
}

void SampleLayer::onTouchCancelled(cocos2d::Touch* _touch, cocos2d::Event* _event) {
	return;
}

cocos2d::MenuItemSprite * SampleLayer::generateButton(const std::string& _caption, 
	                                                  const cocos2d::ccMenuCallback& _callback) {
	auto generate = [this](const std::string& _cap) {
		auto spr = Sprite::create("sample/button.png");
		auto size = spr->getContentSize();
		auto text = Label::createWithSystemFont(_cap, "Arial", 30.0f);
		text->setPosition(Point(size.width * 0.5f, size.height * 0.5f));
		text->setColor(Color3B::BLACK);
		spr->addChild(text);
		return spr;
	};
	auto on = generate(_caption);
	auto select = generate(_caption);
	auto disable = generate(_caption);
	select->setColor(Color3B(200, 230, 230));
	disable->setColor(Color3B::GRAY);

	return MenuItemSprite::create(on, select, disable, _callback);
}