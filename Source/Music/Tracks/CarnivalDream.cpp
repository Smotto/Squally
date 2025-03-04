#include "CarnivalDream.h"

#include "Resources/MusicResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string CarnivalDream::TrackKey = "carnival-dream";

CarnivalDream* CarnivalDream::create(cocos2d::ValueMap& properties)
{
	CarnivalDream* instance = new CarnivalDream(properties);

	instance->autorelease();

	return instance;
}

CarnivalDream::CarnivalDream(cocos2d::ValueMap& properties) : super(properties, MusicResources::CarnivalDream, Strings::Menus_Music_KrazyKartoons_CarnivalDream::create(), Strings::Menus_Music_KrazyKartoons_KrazyKartoons::create())
{
}

CarnivalDream::~CarnivalDream()
{
}
