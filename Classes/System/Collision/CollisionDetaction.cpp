#include "CollisionDetaction.h"
#include "CollisionUtils.h"
#include "collision_point.h"

USING_NS_CC;

static CollisionDetaction* instance = nullptr;

CollisionDetaction::CollisionDetaction()
	:field(nullptr)
	, unitSize()
	, dwCellNum(0)
	, uiLevel(0)
	, spaceArray(nullptr)
	, gameObjectArray(nullptr)
{
	int i = 1;
	iPow[0] = 1;

	for (i = 1; i < TREEMANAGER_MAXLEVEL + 1; i++) {
		iPow[i] = iPow[i - 1] * 4;
	}
}

CollisionDetaction::~CollisionDetaction() {
	reset();
}

CollisionDetaction* CollisionDetaction::create() {
	CollisionDetaction* ref = new CollisionDetaction();
	ref->autorelease();
	return ref;
}

void CollisionDetaction::add(CollisionNode* _node) {
	if (gameObjectArray == nullptr) {
		return;
	}
	gameObjectArray->addObject(_node);
}

void CollisionDetaction::remove(Node* _node) {
	if (gameObjectArray == nullptr) {
		return;
	}

	gameObjectArray->erase(_node);
}

void CollisionDetaction::remove(CollisionNode* _node) {
	if (gameObjectArray == nullptr) {
		return;
	}

	gameObjectArray->erase(_node);
}

void CollisionDetaction::reset() {
	if (field) {
		field->setonExitTransitionDidStartCallback(onExit);
	}
	CC_SAFE_RELEASE_NULL(gameObjectArray);
	CC_SAFE_RELEASE_NULL(field);
	CC_SAFE_RELEASE_NULL(spaceArray);
	onExit = nullptr;
	func = nullptr;
	log("reset");
}

bool CollisionDetaction::init(Node* _field, int _level, bool _autoClean,
	                          const DetectCollisionFunc& _func) {
	CCASSERT(_level < TREEMANAGER_MAXLEVEL, "レベルがMAXLEVELを超えています");

	reset();

	func = _func;

	gameObjectArray = CollisionNodeList::create();
	gameObjectArray->retain();

	spaceArray = CollisionNodeMap::create();
	spaceArray->retain();

	field = _field;
	field->retain();

	if (_autoClean) {
		onExit = field->getonExitTransitionDidStartCallback();
		auto callback = [this]() {
			if (onExit) {
				onExit();
			}
			this->reset();
		};
		field->setOnExitCallback(callback);
	}

	Size winSize = field->getContentSize();
	dwCellNum = (iPow[_level + 1] - 1) / 3;
	uiLevel = _level;
	unitSize = Size((float)(winSize.width / (1 << _level)),
		            (float)(winSize.height / (1 << _level)));
	for (int i = 0; i < dwCellNum; i++) {
		CollisionNodeList* spaceCellArray = CollisionNodeList::create();
		spaceArray->addOject(spaceCellArray);
	}

	return true;
}

void CollisionDetaction::update() {
	if (spaceArray == nullptr) {
		return;
	}

	spaceArray->refresh();

	if (!gameObjectArray || gameObjectArray->count() < 2) {
		return;
	}

	for (CollisionNode* _node : *gameObjectArray) {
		updateSpaceStatus(_node);
	}

	CollisionNodeList* ary = CollisionNodeList::create();
	scanCollisionDetection(0, ary);
}

//4分木空間計算
void CollisionDetaction::updateSpaceStatus(CollisionNode* _collisionObject) {
	int level = -1;
	Point pos(_collisionObject->getNode()->getPosition());
	Size size(_collisionObject->getNode()->getContentSize());
	size.width = size.width / 2 * _collisionObject->getNode()->getScaleX();
	size.height = size.height / 2 * _collisionObject->getNode()->getScaleY();

	float left = pos.x - size.width;
	float top = pos.y + size.height;
	float right = pos.x + size.width;
	float bottom = pos.y - size.height;
	float width = unitSize.width;
	float height = unitSize.height;

	int LT = collision_get_point_elem(left, top, width, height);
	int RB = collision_get_point_elem(right, bottom, width, height);
	int Def = RB ^ LT;
	unsigned int HiLevel = 0;

	for (unsigned int i = 0; i < uiLevel; i++) {
		int check = (Def >> (i * 2)) & 0x3;
		if (check != 0) {
			HiLevel = i + 1;
		}
	}

	level = RB >> (HiLevel * 2);
	int AddNum = (iPow[uiLevel - HiLevel] - 1) / 3;
	level += AddNum;

	if (level < 0 || level >= dwCellNum) {
		return;
	}

	auto spaceCellArray = spaceArray->get(level);
	if (!spaceCellArray->containsObject(_collisionObject)) {
		spaceCellArray->addObject(_collisionObject);
	}
}

void CollisionDetaction::scanCollisionDetection(int _spaceIndex, CollisionNodeList* _stackArray) {
	if (!spaceArray->count()) {
		return;
	}

	CollisionNodeList* spaceCellArray = spaceArray->get(_spaceIndex);

	checkHitSpaceCell(spaceCellArray, spaceCellArray);
	checkHitSpaceCell(spaceCellArray, _stackArray);

	bool childFlag = false;
	int ObjNum = 0;
	int nextSpaceIndex;
	for (int i = 0; i < 4; i++) {
		nextSpaceIndex = _spaceIndex * 4 + 1 + i;
		if (nextSpaceIndex >= 0 && nextSpaceIndex < dwCellNum) {
			if (!childFlag) {
				for (CollisionNode* obj : *spaceCellArray) {
					_stackArray->addObject(obj);
				}
				ObjNum += spaceCellArray->count();
			}
			childFlag = true;
			scanCollisionDetection(nextSpaceIndex, _stackArray);
		}
	}

	if (childFlag) {
		for (int i = 0; i < ObjNum; i++) {
			_stackArray->removeLastObject();
		}
	}

	return;
}

void CollisionDetaction::checkHit(CollisionNode* _collisionObject1, CollisionNode* _collisionObject2) {
	if (func) {
		func(_collisionObject1, _collisionObject2);
	}
}

void CollisionDetaction::checkHitSpaceCell(CollisionNodeList* _array1, CollisionNodeList* _array2) {
	if (_array1 == _array2) {
		size_t ary1Count = _array1->count();
		for (int i = 0; i < ary1Count; i++) {
			size_t ary2Count = _array2->count();
			for (int k = 0; k < ary2Count; k++) {
				checkHit(_array1->objectAtIndex(i), _array2->objectAtIndex(k));
			}
		}
	}
	else {
		for (CollisionNode* obj : *_array1) {
			for (CollisionNode* obj2 : *_array2) {
				if (obj != obj2) {
					checkHit(obj, obj2);
				}
			}
		}
	}
}

CollisionDetaction* CollisionDetaction::getDefaultDetaction() {
	if (instance == nullptr) {
		instance = new CollisionDetaction();
	}

	return instance;
}