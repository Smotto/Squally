#include "RupertHexusBehavior.h"

#include "Objects/Platformer/ItemPools/HexusPools/UnderflowRuins/HexusPoolURGeneric.h"
#include "Scenes/Hexus/CardData/CardKeys.h"
#include "Scenes/Hexus/CardData/CardList.h"
#include "Scenes/Hexus/Opponents/HexusOpponentData.h"
#include "Scenes/Hexus/StateOverride.h"
#include "Scenes/Platformer/Components/Entities/Friendly/Hexus/UnderflowRuins/URHexusConfig.h"

#include "Resources/HexusResources.h"
#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string RupertHexusBehavior::MapKey = "rupert-hexus";

RupertHexusBehavior* RupertHexusBehavior::create(GameObject* owner)
{
	RupertHexusBehavior* instance = new RupertHexusBehavior(owner);

	instance->autorelease();

	return instance;
}

RupertHexusBehavior::RupertHexusBehavior(GameObject* owner) : super(owner, SoundResources::Platformer_Entities_Generic_ChatterShort1)
{
}

RupertHexusBehavior::~RupertHexusBehavior()
{
}

MinMaxPool* RupertHexusBehavior::generateReward()
{
	return HexusPoolURGeneric::create();
}

std::string RupertHexusBehavior::getWinLossSaveKey()
{
	return RupertHexusBehavior::MapKey;
}

std::string RupertHexusBehavior::getBackgroundResource()
{
	return HexusResources::Menus_HexusFrameUnderflowRuins;
}

std::vector<CardData*> RupertHexusBehavior::generateDeck()
{
	const float LocalOrder = 3.0f / URHexusConfig::MaxEntities;

	return HexusOpponentData::generateDeck(28, calculateStrength(LocalOrder, URHexusConfig::ZoneOrder),
	{
		CardList::getInstance()->cardListByName[CardKeys::Binary0],
		CardList::getInstance()->cardListByName[CardKeys::Decimal0],
		CardList::getInstance()->cardListByName[CardKeys::Hex0],
		CardList::getInstance()->cardListByName[CardKeys::Binary0],
		CardList::getInstance()->cardListByName[CardKeys::Decimal0],
		CardList::getInstance()->cardListByName[CardKeys::Hex0],

		// CardList::getInstance()->cardListByName[CardKeys::Mov],
		CardList::getInstance()->cardListByName[CardKeys::Mov],
		// CardList::getInstance()->cardListByName[CardKeys::Flip1],
		CardList::getInstance()->cardListByName[CardKeys::Flip1],
		// CardList::getInstance()->cardListByName[CardKeys::Flip2],
		CardList::getInstance()->cardListByName[CardKeys::Flip2],
		// CardList::getInstance()->cardListByName[CardKeys::Addition],
		CardList::getInstance()->cardListByName[CardKeys::Addition],
		// CardList::getInstance()->cardListByName[CardKeys::ShiftLeft],
		CardList::getInstance()->cardListByName[CardKeys::ShiftLeft],
		// CardList::getInstance()->cardListByName[CardKeys::ShiftRight],
		CardList::getInstance()->cardListByName[CardKeys::ShiftRight],
		// CardList::getInstance()->cardListByName[CardKeys::LogicalOr],
		CardList::getInstance()->cardListByName[CardKeys::LogicalOr],
	});
}

StateOverride* RupertHexusBehavior::getStateOverride()
{
	return nullptr;
}

std::vector<TutorialBase*> RupertHexusBehavior::getTutorials()
{
	return { };
}
