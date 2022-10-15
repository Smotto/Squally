#include "BPPuzzleBBehavior.h"

#include "Objects/Platformer/ItemPools/HexusPools/BallmerPeaks/HexusPoolBPGeneric.h"
#include "Scenes/Hexus/CardData/CardKeys.h"
#include "Scenes/Hexus/CardData/CardList.h"
#include "Scenes/Hexus/Components/Components.h"
#include "Scenes/Hexus/Opponents/HexusOpponentData.h"
#include "Scenes/Hexus/StateOverride.h"

#include "Resources/HexusResources.h"
#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string BPPuzzleBBehavior::MapKey = "bp-puzzle-B";

BPPuzzleBBehavior* BPPuzzleBBehavior::create(GameObject* owner)
{
	BPPuzzleBBehavior* instance = new BPPuzzleBBehavior(owner);

	instance->autorelease();

	return instance;
}

BPPuzzleBBehavior::BPPuzzleBBehavior(GameObject* owner) : super(owner, SoundResources::Platformer_Entities_Generic_ChatterShort2, Strings::Platformer_Dialogue_Hexus_WeAcceptYourChallenge::create())
{
}

BPPuzzleBBehavior::~BPPuzzleBBehavior()
{
}

MinMaxPool* BPPuzzleBBehavior::generateReward()
{
	ValueMap properties = ValueMap();
	return HexusPoolBPGeneric::create(properties);
}

std::string BPPuzzleBBehavior::getWinLossSaveKey()
{
	return BPPuzzleBBehavior::MapKey;
}

std::string BPPuzzleBBehavior::getBackgroundResource()
{
	return HexusResources::Menus_HexusFrameBallmerPeaks;
}

std::vector<CardData*> BPPuzzleBBehavior::generateDeck()
{
	return HexusOpponentData::generateDeck(25, 1.0f,
	{
	});
}

StateOverride* BPPuzzleBBehavior::getStateOverride()
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
		// Player hand
		std::vector<CardData*>
		{
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

std::vector<TutorialBase*> BPPuzzleBBehavior::getTutorials()
{
	return { };
}
