#ifndef __COLLISIONDETACTION_H__
#define __COLLISIONDETACTION_H__

#include <stdio.h>
#include <unordered_set>
#include "cocos2d.h"
#define TREEMANAGER_MAXLEVEL 9

class CollisionNode : public cocos2d::Ref {
private:
	CC_SYNTHESIZE_READONLY(cocos2d::Node*, node, Node);
	CC_SYNTHESIZE_READONLY(unsigned int, group, Group);

public:
	virtual ~CollisionNode() {
		CC_SAFE_RELEASE_NULL(node);
	}

	CollisionNode()
		: node(nullptr)
		, group(0)
	{

	}

	//同一グループかどうか
	bool isGroup(unsigned int _group) {
		return group == _group;
	}

	//BitMaskを返す
	unsigned int bitMask(unsigned int bit) {
		return group & bit;
	}

	//同一グループならtrueを返す
	bool isSameGroup(CollisionNode* _node) {
		return group == _node->group;
	}

	//nodeをフィールドから削除する
	void removeFromParent() {
		if (node) {
			node->removeFromParent();
		}
	}

	//インスタンスを生成
	static CollisionNode* create(cocos2d::Node* _node, unsigned int _group) {
		auto ref = new CollisionNode();
		_node->retain();
		ref->autorelease();
		ref->node = _node;
		ref->group = _group;
		return ref;
	}
};

class CollisionNodeList : public cocos2d::Ref {
private:
	cocos2d::Vector<CollisionNode*> nodeList;

public:
	CREATE_FUNC(CollisionNodeList);

	size_t count() const {
		return nodeList.size();
	}

	void clear() {
		nodeList.clear();
	}

	cocos2d::Vector<CollisionNode*>::iterator begin() {
		return nodeList.begin();
	}

	cocos2d::Vector<CollisionNode*>::iterator end() {
		return nodeList.end();
	}

	cocos2d::Vector<CollisionNode*>::const_iterator begin() const {
		return nodeList.begin();
	}

	cocos2d::Vector<CollisionNode*>::const_iterator end() const {
		return nodeList.end();
	}

	bool containsObject(CollisionNode* _node) {
		return nodeList.contains(_node);
	}

	void addObject(CollisionNode* _node) {
		nodeList.pushBack(_node);
	}

	CollisionNode* objectAtIndex(size_t _size) {
		return nodeList.at(_size);
	}

	void erase(cocos2d::Node* _node) {
		if (auto __node = find(_node)) {
			nodeList.eraseObject(__node);
		}
	}

	CollisionNode* find(cocos2d::Node* _node) {
		auto it = std::find_if(
			nodeList.begin(), nodeList.end(),
			[_node](CollisionNode* _one) { 
			return _one->getNode() == _node; 
		});

		if (it != end()) {
			return *it;
		}
		return nullptr;
	}

	void erase(CollisionNode* _node) {
		nodeList.eraseObject(_node);
	}

	void removeLastObject() {
		nodeList.popBack();
	}

private:
	bool init() {
		return true;
	}
};

class CollisionNodeMap : public cocos2d::Ref {
private:
	cocos2d::Map<size_t, CollisionNodeList*> nodeListMap;
	size_t size;

public:
	CollisionNodeMap()
		: nodeListMap()
		, size(0)
	{

	}

	CREATE_FUNC(CollisionNodeMap);

	void refresh() {
		for (auto space : nodeListMap) {
			space.second->clear();
		}
	}

	void removeAllObjects() {
		nodeListMap.clear();
		size = 0;
	}

	void addOject(CollisionNodeList* _list) {
		nodeListMap.insert(size, _list);
		size++;
	}

	size_t count() {
		return size;
	}

	CollisionNodeList* get(size_t _index) {
		return nodeListMap.at(_index);
	}

private:
	bool init() {
		return true;
	}
};

using DetectCollisionFunc = std::function<void(CollisionNode*, CollisionNode*)>;

class CollisionDetaction : public cocos2d::Ref {
public:
	CollisionDetaction();
	~CollisionDetaction();

private:
	DetectCollisionFunc func;

	std::function<void()> onExit;

	cocos2d::Node* field;

	CollisionNodeMap* spaceArray;

	CollisionNodeList* gameObjectArray;

	cocos2d::Size unitSize;

	int dwCellNum;

	int uiLevel;

	unsigned int iPow[TREEMANAGER_MAXLEVEL + 1];

public:
	static CollisionDetaction* getDefaultDetaction();

	static CollisionDetaction* create();

	virtual bool init(cocos2d::Node* _field, int _level, bool _autoClean,
		const DetectCollisionFunc& _func);

	virtual void reset();

	virtual void update();

	virtual void add(CollisionNode* _node);

	virtual void remove(CollisionNode* _node);

	virtual void remove(cocos2d::Node* _node);

protected:
	virtual void updateSpaceStatus(CollisionNode* _collisionObject);

	virtual void scanCollisionDetection(int _spaceIndex, CollisionNodeList* stackArray);

	virtual void checkHit(CollisionNode* _collisionObject1, CollisionNode* _collisionObject2);

	virtual void checkHitSpaceCell(CollisionNodeList* _array1, CollisionNodeList* _array2);
};

#endif