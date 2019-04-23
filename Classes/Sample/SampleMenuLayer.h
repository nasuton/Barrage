#ifndef __SAMPLEMENULAYER_H__
#define __SAMPLEMENULAYER_H__

#include "System/LayerBase/LayerBase.h"

class SampleMenuLayer : public LayerBase {
public:
	SampleMenuLayer();
	virtual ~SampleMenuLayer();

	CREATE_FUNC(SampleMenuLayer);

	bool usefortree;
	int num;
	int winSize;
	cocos2d::Vector<cocos2d::MenuItemSprite*> nums;
	cocos2d::Vector<cocos2d::MenuItemSprite*> uses;
	cocos2d::Vector<cocos2d::MenuItemSprite*> winSizes;

protected:
	cocos2d::MenuItemSprite* generateButton(const std::string& _caption, const cocos2d::ccMenuCallback& _collback);

private:
	bool init();
};

#endif