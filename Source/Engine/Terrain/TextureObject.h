#pragma once
#include <string>

#include "cocos/base/ccTypes.h"

#include "Engine/Hackables/HackableObject.h"
#include "Engine/Utils/AlgoUtils.h"

class TextureObject : public HackableObject
{
public:
	struct TextureData
	{
		std::string textureMapKeyValue;
		std::string textureResource;
		cocos2d::Color4B infillColor;

		TextureData(
			std::string textureMapKeyValue,
			std::string textureResource,
			cocos2d::Color4B infillColor) :
			textureMapKeyValue(textureMapKeyValue),
			textureResource(textureResource),
			infillColor(infillColor)
		{
		}

		TextureData()
		{
		}
	};
	
	static std::string MapKeyTypeTexture;
	static std::string PropertyKeyClearAnchor;

protected:
	TextureObject(cocos2d::ValueMap& properties, TextureData terrainData);
	virtual ~TextureObject();

	TextureData terrainData;

	cocos2d::CRect boundsRect;
	std::vector<cocos2d::Vec2> points;
	std::vector<std::tuple<cocos2d::Vec2, cocos2d::Vec2>> segments;
	std::vector<AlgoUtils::Triangle> textureTriangles;

private:
	typedef HackableObject super;

	void setPoints(std::vector<cocos2d::Vec2> points);
	void buildTextures();

	bool useClipping;

	cocos2d::Node* infillTexturesNode;
};
