#include "System/Collision/CollisionComponent.h"

USING_NS_CC;

CollisionComponent::CollisionComponent()
	:detaction(nullptr)
	, node(nullptr)
	, group(UINT_MAX) {
}

CollisionComponent::~CollisionComponent() {
	CC_SAFE_RELEASE_NULL(node);
	CC_SAFE_RELEASE_NULL(detaction);
}

CollisionComponent* CollisionComponent::create(unsigned int _group) {
	return create(CollisionDetaction::getDefaultDetaction(), _group);
}

CollisionComponent* CollisionComponent::create(CollisionDetaction* _detaction) {
	return create(_detaction, UINT_MAX);
}

CollisionComponent* CollisionComponent::create() {
	return create(CollisionDetaction::getDefaultDetaction(), UINT_MAX);
}

CollisionComponent* CollisionComponent::create(CollisionDetaction* _detaction, unsigned int _group) {
	auto ref = new CollisionComponent();
	if (ref && ref->initWithDetaction(_detaction, _group)) {
		ref->autorelease();
		return ref;
	}

	CC_SAFE_RELEASE(ref);
	return nullptr;
}

void CollisionComponent::onEnter() {
	Component::onEnter();
	node = CollisionNode::create(getOwner(), group);
	node->retain();
	detaction->add(node);
}

void CollisionComponent::onExit() {
	Component::onExit();
	if (node->getReferenceCount() > 1) {
		detaction->remove(node);
	}
	CC_SAFE_RELEASE_NULL(detaction);
	CC_SAFE_RELEASE_NULL(node);
}

bool CollisionComponent::initWithDetaction(CollisionDetaction * _detaction, unsigned int _group) {
	if(!Component::init()) {
		return false;
	}
	CC_SAFE_RETAIN(_detaction);
	detaction = _detaction;
	group = _group;

	return true;
}
