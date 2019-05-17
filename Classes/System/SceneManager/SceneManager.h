#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "cocos2d.h"

class SceneManager {
public:
	SceneManager();
	virtual ~SceneManager();

	//ここにシーンを追加していく
	

private:
	template <class Scene>
	static cocos2d::Scene* sceneCreate();

	template <class Scene>
	static cocos2d::Scene* sceneCreate(int _num, bool _use, int _scale);
};

template <class Scene>
inline cocos2d::Scene* SceneManager::sceneCreate() {
	cocos2d::Scene* scene = Scene::sceneCreate();

	return scene;
}

template <class Scene>
inline cocos2d::Scene* SceneManager::sceneCreate(int _num, bool _use, int _scale) {
	cocos2d::Scene* scene = Scene::sceneCreate(_num, _use, _scale);

	return scene;
}

#endif