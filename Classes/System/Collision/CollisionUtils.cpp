#include "System/Collision/CollisionUtils.h"

USING_NS_CC;

//矩形と点の衝突判定
bool CollisionUtils::containsPoint(Node* _target, const Point &_point, const Size &_wide) {
	if (_target == nullptr) {
		return false;
	}

	if (_target->getBoundingBox().containsPoint(_point) == false) {
		return false;
	}

	Point anchor(_target->getAnchorPoint());
	Point pos(_target->getPosition());
	Size size(_target->getContentSize());
	size.width *= _target->getScaleX();
	size.height *= _target->getScaleY();

	double x1 = pos.x - size.width * anchor.x;
	double y1 = pos.y + size.height * (1 - anchor.y);
	double cx1 = x1 + (size.width / 2);
	double cy1 = y1 - (size.height / 2);
	double l = sqrt(pow(_point.x - cx1, 2) + pow(_point.y - cy1, 2));
	double r = CC_DEGREES_TO_RADIANS(-_target->getRotation());
	double r2 = atan((_point.y - cy1) / (_point.x - cx1)) - r;
	double tx2 = l * cos(r2) + cx1;
	double ty2 = l * sin(r2) + cy1;

	if (x1 - _wide.width <= tx2 &&               
		tx2 <= x1 + size.width + _wide.width &&  
		y1 + _wide.height >= ty2 &&              
		ty2 >= y1 - size.height - _wide.height) {
		return true;
	}

	return false;
}

bool CollisionUtils::intersectRect(cocos2d::Node* _target, cocos2d::Node* _self)
{
	if (!_target->getBoundingBox().intersectsRect(_self->getBoundingBox())) {
		return false;
	}

	Size tSize(_target->getContentSize());
	Size sSize(_self->getContentSize());
	double tl = sqrt(pow(tSize.width, 2) + pow(tSize.height, 2));
	double sl = sqrt(pow(sSize.width, 2) + pow(sSize.height, 2));
	Node* s = tl >= sl ? _self : _target;
	Node* b = tl >= sl ? _target : _self;
	CollisionCorner sCorner = getCorner(s);
	CollisionCorner bCorner = getCorner(b);

	// 線分に入っているかどうかを見る
	for (CollisionLine line : sCorner.lines()) {
		for (CollisionLine line2 : bCorner.lines()) {
			if (cross(&line, &line2)) {
				return true;
			}
		}
	}
	return containsPoint(b, s->getPosition(), Size::ZERO) ||
		containsPoint(s, b->getPosition(), Size::ZERO);
}

CollisionCorner CollisionUtils::getCorner(cocos2d::Node * _node)
{
	Size size(_node->getContentSize());
	size.width *= _node->getScaleX();
	size.height *= _node->getScaleY();

	double _atan1 = atan2(size.height / 2, size.width / 2);
	double _atan2 = atan2(size.height / 2, -size.width / 2);
	double _atan3 = -_atan1;
	double _atan4 = -_atan2;
	double l = sqrt(pow(size.width / 2, 2) + pow(size.height / 2, 2));

	double radian = CC_DEGREES_TO_RADIANS(-_node->getRotation());
	Point pos(_node->getPosition());

	return { Point(pos.x + l * cosf(radian + _atan1),
				  pos.y + l * sinf(radian + _atan1)),
			Point(pos.x + l * cosf(radian + _atan2),
				  pos.y + l * sinf(radian + _atan2)),
			Point(pos.x + l * cosf(radian + _atan3),
				  pos.y + l * sinf(radian + _atan3)),
			Point(pos.x + l * cosf(radian + _atan4),
				  pos.y + l * sinf(radian + _atan4)),
			pos };
}

bool CollisionUtils::cross(CollisionLine* _one, CollisionLine* _two) {
	float ax = _one->start->x;
	float ay = _one->start->y;
	float bx = _one->end->x;
	float by = _one->end->y;
	float cx = _two->start->x;
	float cy = _two->start->y;
	float dx = _two->end->x;
	float dy = _two->end->y;

	float ta = (cx - dx) * (ay - cy) + (cy - dy) * (cx - ax);
	float tb = (cx - dx) * (by - cy) + (cy - dy) * (cx - bx);
	float tc = (ax - bx) * (cy - ay) + (ay - by) * (ax - cx);
	float td = (ax - bx) * (dy - ay) + (ay - by) * (ax - dx);

	return tc * td < 0 && ta * tb < 0;
}
