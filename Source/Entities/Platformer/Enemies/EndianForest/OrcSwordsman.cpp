#include "OrcSwordsman.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string OrcSwordsman::MapKey = "orc-swordsman";

OrcSwordsman* OrcSwordsman::deserialize(ValueMap& properties)
{
	OrcSwordsman* instance = new OrcSwordsman(properties);

	instance->autorelease();

	return instance;
}

OrcSwordsman::OrcSwordsman(ValueMap& properties) : super(properties,
	OrcSwordsman::MapKey,
	EntityResources::Enemies_EndianForest_OrcSwordsman_Animations,
	EntityResources::Enemies_EndianForest_OrcSwordsman_Emblem,
	CSize(256.0f, 228.0f),
	1.0f)
{
}

OrcSwordsman::~OrcSwordsman()
{
}

Vec2 OrcSwordsman::getDialogueOffset()
{
	return Vec2(0.0f, 0.0f);
}

LocalizedString* OrcSwordsman::getEntityName()
{
	return Strings::Platformer_Entities_Names_Enemies_EndianForest_OrcSwordsman::create();
}
