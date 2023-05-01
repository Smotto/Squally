#include "Cleopatra.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Cleopatra::MapKey = "cleopatra";

Cleopatra* Cleopatra::deserialize(ValueMap& properties)
{
	Cleopatra* instance = new Cleopatra(properties);

	instance->autorelease();

	return instance;
}

Cleopatra::Cleopatra(ValueMap& properties) : super(properties,
	Cleopatra::MapKey,
	EntityResources::Npcs_UnderflowRuins_Cleopatra_Animations,
	EntityResources::Npcs_UnderflowRuins_Cleopatra_Emblem,
	CSize(112.0f, 256.0f),
	1.0f)
{
}

Cleopatra::~Cleopatra()
{
}

Vec2 Cleopatra::getDialogueOffset()
{
	return Vec2(0.0f, 0.0f);
}

LocalizedString* Cleopatra::getEntityName()
{
	return Strings::Platformer_Entities_Names_Npcs_UnderflowRuins_Cleopatra::create();
}
