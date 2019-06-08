#ifndef __JSONHELPER_H__
#define __JSONHELPER_H__

#include "cocos2d.h"

//json関連
#include "json/rapidjson.h"
#include "json/document.h"

class JsonHelper {
public:
	//jsonで使う際の手順をまとめた
	template<unsigned Flag>
	static rapidjson::Document DocumentParse(std::string _jsonName);

};

template<unsigned Flag>
rapidjson::Document JsonHelper::DocumentParse(std::string _jsonName) {
	cocos2d::FileUtils* fileUtils = cocos2d::FileUtils::getInstance();
	std::string path = fileUtils->getStringFromFile(_jsonName);

	rapidjson::Document doc;
	doc.Parse<Flag>(path.c_str());

	return doc;
}

#endif