#ifndef __GAMEMAINSCENE_H__
#define __GAMEMAINSCENE_H__

#include "System/SceneBase/SceneBase.h"

class GameMainScene : public SceneBase {
public:
	GameMainScene();
	~GameMainScene();

	static cocos2d::Scene* sceneCreate();
};

#endif