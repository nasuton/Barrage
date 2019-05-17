#ifndef __SAMPLEMAINLAYER_H__
#define __SAMPLEMAINLAYER_H__

#include <stdio.h>
#include "cocos2d.h"

class SampleMainLayer : public cocos2d::LayerColor {
   public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(SampleMainLayer);
    bool m_usefortree;
    int m_num;
    int m_winSize;
    cocos2d::Vector<cocos2d::MenuItemSprite*> m_nums;
    cocos2d::Vector<cocos2d::MenuItemSprite*> m_uses;
    cocos2d::Vector<cocos2d::MenuItemSprite*> m_winsizes;

   protected:
    bool init() override;
    cocos2d::MenuItemSprite* generateButton(
        const std::string& caption, const cocos2d::ccMenuCallback& callback);
};

#endif