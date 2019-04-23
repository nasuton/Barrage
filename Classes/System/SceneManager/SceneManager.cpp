#include "System/SceneManager/SceneManager.h"
#include "System/LayerBase/LayerBase.h"
#include "Sample/SampleScene.h"
#include "Sample/SampleMenuScene.h"

USING_NS_CC;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {

}

Scene* SceneManager::CreateSampleScene(int _num, bool _use, int _scale) {
	Scene* nextScene = sceneCreate<SampleScene>(_num, _use, _scale);
	return nextScene;
}

Scene* SceneManager::CreateSampleMenuScene() {
	Scene* nextScene = sceneCreate<SampleMenuScene>();
	return nextScene;
}