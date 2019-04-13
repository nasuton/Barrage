#ifndef __COLLISIONUTILS_H__
#define __COLLISIONUTILS_H__

#include <stdio.h>
#include "cocos2d.h"

typedef struct __line {
	cocos2d::Point* start;
	cocos2d::Point* end;
} CollisionLine;

typedef struct __collision_corner {
	cocos2d::Point rightTop;
	cocos2d::Point leftTop;
	cocos2d::Point rightBottom;
    cocos2d::Point leftBottom;
    cocos2d::Point center;
    std::vector<cocos2d::Point*> points() {
        return {&rightTop, &leftTop, &rightBottom, &leftBottom, &center};
    }
    std::vector<CollisionLine> lines() {
        return {{&rightTop, &leftTop},
                {&rightTop, &rightBottom},
                {&rightBottom, &leftBottom},
                {&leftTop, &leftBottom}};
    }
} CollisionCorner;

class CollisionUtils {
public:
	//矩形と点の衝突判定
	static bool containsPoint(cocos2d::Node* _target,
		                      const cocos2d::Point& _point,
		                      const cocos2d::Size& _wide);
	
	//矩形同士の衝突判定
	static bool intersectRect(cocos2d::Node* _target, cocos2d::Node* _self);

	static CollisionCorner getCorner(cocos2d::Node* _node);

private:
	static bool cross(CollisionLine* _one, CollisionLine* _two);
	CC_DISALLOW_COPY_AND_ASSIGN(CollisionUtils);
	CollisionUtils() { };
};

#endif