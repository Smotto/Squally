#include "GoblinGruntBoar.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string GoblinGruntBoar::MapKey = "goblin-grunt-boar";

GoblinGruntBoar* GoblinGruntBoar::deserialize(ValueMap& properties)
{
	GoblinGruntBoar* instance = new GoblinGruntBoar(properties);

	instance->autorelease();

	return instance;
}

GoblinGruntBoar::GoblinGruntBoar(ValueMap& properties) : super(properties,
	GoblinGruntBoar::MapKey,
	EntityResources::Enemies_EndianForest_GoblinGruntBoar_Animations,
	EntityResources::Enemies_EndianForest_GoblinGruntBoar_Emblem,
	CSize(128.0f, 296.0f),
	0.85f)
{
}

GoblinGruntBoar::~GoblinGruntBoar()
{
}

Vec2 GoblinGruntBoar::getDialogueOffset()
{
	return Vec2(0.0f, -32.0f);
}

LocalizedString* GoblinGruntBoar::getEntityName()
{
	return Strings::Platformer_Entities_Names_Enemies_EndianForest_GoblinGruntBoar::create();
}
