#include "System/SceneManager/SceneManager.h"
#include "System/LayerBase/LayerBase.h"
#include "SampleScene.h"

USING_NS_CC;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {

}

Scene* SceneManager::CreateSampleScene() {
	Scene* nextScene = sceneCreate<SampleScene>();
	return nextScene;
}