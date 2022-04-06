#include "Thatcher.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Thatcher::MapKey = "thatcher";

Thatcher* Thatcher::deserialize(ValueMap& properties)
{
	Thatcher* instance = new Thatcher(properties);

	instance->autorelease();

	return instance;
}

Thatcher::Thatcher(ValueMap& properties) : super(properties,
	Thatcher::MapKey,
	EntityResources::Npcs_DaemonsHallow_Thatcher_Animations,
	EntityResources::Npcs_DaemonsHallow_Thatcher_Emblem,
	CSize(112.0f, 160.0f),
	0.9f,
	Vec2(0.0f, 0.0f))
{
}

Thatcher::~Thatcher()
{
}

Vec2 Thatcher::getDialogueOffset()
{
	return Vec2(0.0f, 0.0f);
}

LocalizedString* Thatcher::getEntityName()
{
	return Strings::Platformer_Entities_Names_Npcs_VoidStar_Thatcher::create();
}
