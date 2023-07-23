#include "StateOverride.h"

#include "Scenes/Hexus/CardData/CardData.h"
#include "Scenes/Hexus/CardData/CardList.h"
#include "Scenes/Hexus/CardData/CardKeys.h"
#include "Scenes/Hexus/Deck.h"

using namespace cocos2d;

StateOverride* StateOverride::create(
	int playerLosses,
	int enemyLosses,
	bool playersTurn,
	bool playerPassed,
	bool enemyPassed,
	std::vector<CardData*> playerDeck,
	std::vector<CardData*> enemyDeck,
	std::vector<CardData*> playerGraveyard,
	std::vector<CardData*> enemyGraveyard,
	std::vector<CardData*> playerHand,
	std::vector<CardData*> enemyHand,
	std::vector<CardData*> playerBinaryCards,
	std::vector<CardData*> playerDecimalCards,
	std::vector<CardData*> playerHexCards,
	std::vector<CardData*> enemyBinaryCards,
	std::vector<CardData*> enemyDecimalCards,
	std::vector<CardData*> enemyHexCards,
	bool showChallengeBanner
)
{
	StateOverride* instance = new StateOverride(
		playerLosses,
		enemyLosses,
		playersTurn,
		playerPassed,
		enemyPassed,
		playerDeck,
		enemyDeck,
		playerGraveyard,
		enemyGraveyard,
		playerHand,
		enemyHand,
		playerBinaryCards,
		playerDecimalCards,
		playerHexCards,
		enemyBinaryCards,
		enemyDecimalCards,
		enemyHexCards,
		showChallengeBanner);

	instance->autorelease();

	return instance;
}

StateOverride::StateOverride()
{
}

StateOverride::StateOverride(
	int playerLosses,
	int enemyLosses,
	bool playersTurn,
	bool playerPassed,
	bool enemyPassed,
	std::vector<CardData*> playerDeck,
	std::vector<CardData*> enemyDeck,
	std::vector<CardData*> playerGraveyard,
	std::vector<CardData*> enemyGraveyard,
	std::vector<CardData*> playerHand,
	std::vector<CardData*> enemyHand,
	std::vector<CardData*> playerBinaryCards,
	std::vector<CardData*> playerDecimalCards,
	std::vector<CardData*> playerHexCards,
	std::vector<CardData*> enemyBinaryCards,
	std::vector<CardData*> enemyDecimalCards,
	std::vector<CardData*> enemyHexCards,
	bool showChallengeBanner)
{
	this->playerLosses = playerLosses;
	this->enemyLosses = enemyLosses;
	this->playersTurn = playersTurn;
	this->playerPassed = playerPassed;
	this->enemyPassed = enemyPassed;
	this->playerDeck = playerDeck;
	this->enemyDeck = enemyDeck;
	this->playerGraveyard = playerGraveyard;
	this->enemyGraveyard = enemyGraveyard;
	this->playerHand = playerHand;
	this->enemyHand = enemyHand;
	this->playerBinaryCards = playerBinaryCards;
	this->playerDecimalCards = playerDecimalCards;
	this->playerHexCards = playerHexCards;
	this->enemyBinaryCards = enemyBinaryCards;
	this->enemyDecimalCards = enemyDecimalCards;
	this->enemyHexCards = enemyHexCards;
	this->showChallengeBanner = showChallengeBanner;
}

StateOverride::~StateOverride()
{
}
