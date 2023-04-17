#include "DemonRogue.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string DemonRogue::MapKey = "demon-rogue";

DemonRogue* DemonRogue::deserialize(ValueMap& properties)
{
	DemonRogue* instance = new DemonRogue(properties);

	instance->autorelease();

	return instance;
}

DemonRogue::DemonRogue(ValueMap& properties) : super(properties,
	DemonRogue::MapKey,
	EntityResources::Enemies_FirewallFissure_DemonRogue_Animations,
	EntityResources::Enemies_FirewallFissure_DemonRogue_Emblem,
	CSize(212.0f, 268.0f),
	0.65f,
	Vec2(0.0f, 0.0f))
{
}

DemonRogue::~DemonRogue()
{
}

Vec2 DemonRogue::getDialogueOffset()
{
	return Vec2(0.0f, 0.0f);
}

LocalizedString* DemonRogue::getEntityName()
{
	return Strings::Platformer_Entities_Names_Enemies_FirewallFissure_DemonRogue::create();
}
