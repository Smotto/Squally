#pragma once
#include <functional>
#include <vector>

#include "Engine/SmartNode.h"
#include "Scenes/Hexus/Card.h"

namespace cocos2d
{
	class Sprite;
}

class ClickableNode;

class Deck : public SmartNode
{
public:
	static Deck* create(bool isPlayerOwnedDeck = true);
	static Deck* create(Card::CardStyle cardStyle, std::vector<CardData*> cards, bool isPlayerOwnedDeck = true);

	void copyTo(Deck* otherDeck);
	int getCardCount();

	void enableTopCardInteraction(std::function<void(Card*)> mouseOverCallback);
	void disableInteraction();
	void enableDeckSelection(std::function<void(Deck*)> callback);
	void disableDeckSelection();
	void enableClearOperationsOnInsert();
	void removeCardsWhere(std::function<bool(Card*)> predicate);
	Card* removeCard(Card* card);
	Card* drawCard();
	void setCardScale(float scale, float scaleSpeed);
	bool hasCards();
	void shuffle();
	void insertCardTop(Card* card, bool faceUp, float insertDelay);
	void insertCardBottom(Card* card, bool faceUp, float insertDelay);
	void insertCardRandom(Card* card, bool faceUp, float insertDelay);
	void clear();
	void setCardPositions(float cardRepositionDelay, float indexDelay = 0.0f);
	
	std::vector<Card*> deckCards;
	Card::CardStyle style = Card::CardStyle::Shadow;

protected:
	Deck(bool isPlayerOwnedDeck);
	Deck(Card::CardStyle cardStyle, std::vector<CardData*> cardData, bool isPlayerOwnedDeck);
	virtual ~Deck();

private:
	typedef SmartNode super;

	void setCardOrder();
	void doInsertAnimation(Card* card, bool faceUp, float insertDelay);

	bool clearOperationsOnInsert = false;
	bool isPlayerOwnedDeck = false;
	
	ClickableNode* pad = nullptr;
	cocos2d::Node* cardsNode = nullptr;
	cocos2d::Sprite* deckFocus = nullptr;
};
