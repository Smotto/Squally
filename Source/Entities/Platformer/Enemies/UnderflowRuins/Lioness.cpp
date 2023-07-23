#include "Lioness.h"

#include "cocos/math/CCGeometry.h"

#include "Resources/EntityResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Lioness::MapKey = "lioness";

Lioness* Lioness::deserialize(ValueMap& properties)
{
	Lioness* instance = new Lioness(properties);

	instance->autorelease();

	return instance;
}

Lioness::Lioness(ValueMap& properties) : super(properties,
	Lioness::MapKey,
	EntityResources::Enemies_UnderflowRuins_Lioness_Animations,
	EntityResources::Enemies_UnderflowRuins_Lioness_Emblem,
	CSize(128.0f, 278.0f),
	1.0f)
{
}

Lioness::~Lioness()
{
}

Vec2 Lioness::getDialogueOffset()
{
	return Vec2(-16.0f, 0.0f);
}

LocalizedString* Lioness::getEntityName()
{
	return Strings::Platformer_Entities_Names_Enemies_UnderflowRuins_Lioness::create();
}
