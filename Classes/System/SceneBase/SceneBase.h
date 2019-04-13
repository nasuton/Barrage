#ifndef __SCENEBASE_H__
#define __SCENEBASE_H__

#include "cocos2d.h"

class SceneBase : public cocos2d::Scene {
public:
	SceneBase();
	virtual ~SceneBase();

protected:
	template <class LayerClass>
	static cocos2d::Layer* createLayer();
};

template <class LayerClass>
inline cocos2d::Layer* SceneBase::createLayer() {
	LayerClass* layer = LayerClass::create();
	return layer;
}

#endif