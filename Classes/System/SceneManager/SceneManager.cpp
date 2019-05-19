#include "System/SceneManager/SceneManager.h"
#include "GameMain/GameMainScene.h"

USING_NS_CC;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {

}

void SceneManager::CreateGameMain() {
	Scene* scene = sceneCreate<GameMainScene>();
}