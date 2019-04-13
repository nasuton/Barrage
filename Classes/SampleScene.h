#ifndef __SAMPLESCENE_H__
#define __SAMPLESCENE_H__

#include "System/SceneBase/SceneBase.h"

class SampleScene : public SceneBase {
public:
	SampleScene();
	virtual ~SampleScene();

	static cocos2d::Scene* sceneCreate();
};

#endif