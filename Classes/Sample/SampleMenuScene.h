#ifndef __SAMPLEMENUSCENE_H__
#define __SAMPLEMENUSCENE_H__

#include "System/SceneBase/SceneBase.h"

class SampleMenuScene : public SceneBase {
public:
	SampleMenuScene();
	virtual ~SampleMenuScene();

	static cocos2d::Scene* sceneCreate();
};

#endif