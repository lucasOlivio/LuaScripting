#pragma once

#include <string>
#include <rapidjson/document.h>

class SceneParserJSON
{
public:
	SceneParserJSON();
	~SceneParserJSON();

	bool ParseFromJsonObj(rapidjson::Value& jsonObject);
	bool ParseToJsonObj(rapidjson::Value& jsonObjectOut,
						rapidjson::Document::AllocatorType& allocator);
};