#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "cocos2d.h"

class SceneManager {
public:
	SceneManager();
	virtual ~SceneManager();

	//ここにシーンを追加していく
	static cocos2d::Scene* CreateSampleScene();

private:
	template <class Scene>
	static cocos2d::Scene* sceneCreate();
};

template <class Scene>
inline cocos2d::Scene* SceneManager::sceneCreate() {
	cocos2d::Scene* scene = Scene::sceneCreate();

	return scene;
}

#endif