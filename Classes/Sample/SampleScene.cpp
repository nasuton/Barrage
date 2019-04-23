#include "Sample/SampleScene.h"
#include "Sample/SampleLayer.h"

USING_NS_CC;

SampleScene::SampleScene() {

}

SampleScene::~SampleScene() {

}

Scene* SampleScene::sceneCreate(int _num, bool _use, int _scale) {
	Scene* scene = Scene::create();

	scene->addChild(createLayer<SampleLayer>(_num, _use, _scale));

	return scene;
}