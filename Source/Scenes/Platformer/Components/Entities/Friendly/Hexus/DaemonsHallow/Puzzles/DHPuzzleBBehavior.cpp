#include "DHPuzzleBBehavior.h"

#include "Objects/Platformer/ItemPools/HexusPools/DaemonsHallow/HexusPoolDHGeneric.h"
#include "Scenes/Hexus/CardData/CardKeys.h"
#include "Scenes/Hexus/CardData/CardList.h"
#include "Scenes/Hexus/Components/Components.h"
#include "Scenes/Hexus/Opponents/HexusOpponentData.h"
#include "Scenes/Hexus/StateOverride.h"

#include "Resources/HexusResources.h"
#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string DHPuzzleBBehavior::MapKey = "dh-puzzle-B";

DHPuzzleBBehavior* DHPuzzleBBehavior::create(GameObject* owner)
{
	DHPuzzleBBehavior* instance = new DHPuzzleBBehavior(owner);

	instance->autorelease();

	return instance;
}

DHPuzzleBBehavior::DHPuzzleBBehavior(GameObject* owner) : super(owner, SoundResources::Platformer_Entities_Generic_ChatterShort2, Strings::Platformer_Dialogue_Hexus_WeAcceptYourChallenge::create())
{
}

DHPuzzleBBehavior::~DHPuzzleBBehavior()
{
}

MinMaxPool* DHPuzzleBBehavior::generateReward()
{
	ValueMap properties = ValueMap();
	return HexusPoolDHGeneric::create(properties);
}

std::string DHPuzzleBBehavior::getWinLossSaveKey()
{
	return DHPuzzleBBehavior::MapKey;
}

std::string DHPuzzleBBehavior::getBackgroundResource()
{
	return HexusResources::Menus_HexusFrameDaemonsHallow;
}

std::vector<CardData*> DHPuzzleBBehavior::generateDeck()
{
	return HexusOpponentData::generateDeck(25, 1.0f,
	{
	});
}

StateOverride* DHPuzzleBBehavior::getStateOverride()
{
	return StateOverride::create(
		// Player losses
		1,
		// Enemy losses
		1,
		// Player's turn
		true,
		// Player passed
		true,
		// Enemy passed
		true,
		// Player deck
		std::vector<CardData*>
		{
			
		},
		// Enemy deck
		std::vector<CardData*>
		{
			
		},
		// Player graveyard
		std::vector<CardData*>
		{
			
		},
		// Enemy graveyard
		std::vector<CardData*>
		{
			
		},
		// Player hand
		std::vector<CardData*>
		{
			CardList::getInstance()->cardListByName.at(CardKeys::Steal),
		},
		// Enemy hand
		std::vector<CardData*>
		{
			
		},
		// Player binary cards
		std::vector<CardData*>
		{
		},
		// Player decimal cards
		std::vector<CardData*>
		{
		},
		// Player hex cards
		std::vector<CardData*>
		{
			
		},
		// Enemy binary cards
		std::vector<CardData*>
		{
		},
		// Enemy decimal cards
		std::vector<CardData*>
		{
		},
		// Enemy hex cards
		std::vector<CardData*>
		{
		}
	);
}

std::vector<TutorialBase*> DHPuzzleBBehavior::getTutorials()
{
	return { };
}
