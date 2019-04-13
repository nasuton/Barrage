#include "SampleLayer.h"
#include "SampleScene.h"

USING_NS_CC;

SampleScene::SampleScene() {

}

SampleScene::~SampleScene() {

}

Scene* SampleScene::sceneCreate() {
	Scene* scene = Scene::create();

	scene->addChild(createLayer<SampleLayer>());

	return scene;
}