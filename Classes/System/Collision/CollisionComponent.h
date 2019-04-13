#ifndef __COLLISIONCOMPONENT_H__
#define __COLLISIONCOMPONENT_H__

#include <stdio.h>
#include "System/Collision/CollisionDetaction.h"
#include "cocos2d.h"

class CollisionComponent : public cocos2d::Component {
private:
	CollisionDetaction* detaction;
	CollisionNode* node;
	unsigned int group;

public:
	CollisionComponent();

	virtual ~CollisionComponent();

	static CollisionComponent* create();

	static CollisionComponent* create(unsigned int _group);

	static CollisionComponent* create(CollisionDetaction* _detaction);

	static CollisionComponent* create(CollisionDetaction* _detaction, unsigned int _group);

	void onEnter() override;

	void onExit() override;

protected:
	virtual bool initWithDetaction(CollisionDetaction* _detaction, unsigned int _group);
};

#endif