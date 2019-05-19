#include "GameMain/GameMainScene.h"
#include "GameMain/GameMainLayer.h"

USING_NS_CC;

GameMainScene::GameMainScene() {

}

GameMainScene::~GameMainScene() {

}


Scene* GameMainScene::sceneCreate() {
	Scene* scene = Scene::create();
	scene->addChild(createLayer<GameMainLayer>());
	return scene;
}