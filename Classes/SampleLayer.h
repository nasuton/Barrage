#ifndef __SAMPLELAYER_H__
#define __SAMPLELATER_H__

#include "System/LayerBase/LayerBase.h"

class SampleLayer : public LayerBase {
public:
	SampleLayer();
	virtual ~SampleLayer();

	CREATE_FUNC(SampleLayer);

private:
	bool init();
};

#endif