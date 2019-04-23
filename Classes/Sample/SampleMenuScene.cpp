#include "Sample/SampleMenuScene.h"
#include "Sample/SampleMenuLayer.h"

USING_NS_CC;

SampleMenuScene::SampleMenuScene() {

}

SampleMenuScene::~SampleMenuScene() {

}

Scene* SampleMenuScene::sceneCreate() {
	Scene* scene = Scene::create();
	scene->addChild(createLayer<SampleMenuLayer>());
	return scene;
}