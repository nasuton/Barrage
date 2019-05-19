#ifndef __GAMEMAINLAYER_H__
#define __GAMEMAINLAYER_H__

#include "System/LayerBase/LayerBase.h"

class GameMainLayer : public LayerBase {
public:
	GameMainLayer();
	~GameMainLayer();

	CREATE_FUNC(GameMainLayer);
private:
	bool init();
};

#endif