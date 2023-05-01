#include "Olive.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Olive::MapKey = "olive";

Olive* Olive::deserialize(ValueMap& properties)
{
	Olive* instance = new Olive(properties);

	instance->autorelease();

	return instance;
}

Olive::Olive(ValueMap& properties) : super(properties,
	Olive::MapKey,
	EntityResources::Npcs_DataMines_Olive_Animations,
	EntityResources::Npcs_DataMines_Olive_Emblem,
	CSize(112.0f, 160.0f),
	1.05f)
{
}

Olive::~Olive()
{
}

Vec2 Olive::getDialogueOffset()
{
	return Vec2(0.0f, 0.0f);
}

LocalizedString* Olive::getEntityName()
{
	return Strings::Platformer_Entities_Names_Npcs_DataMines_Olive::create();
}
