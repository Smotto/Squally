#pragma once
#include <functional>
#include <vector>

#include "Engine/SmartNode.h"
#include "Scenes/Hexus/CardEffects.h"

class Card;
class ClickableNode;

class CardRow : public SmartNode
{
public:
	static CardRow* create(bool isPlayerRow);

	void insertCards(std::vector<Card*> cards, float cardInsertDelay, float indexDelay = 0.0f);
	void insertCard(Card* card, float cardInsertDelay);
	void insertCardsFront(std::vector<Card*> cards, float cardInsertDelay, float indexDelay = 0.0f);
	void insertCardFront(Card* card, float cardInsertDelay);
	void enableClearOperationsOnInsert();
	Card* removeCard(Card* card);
	void removeCardsWhere(std::function<bool(Card*)> predicate);
	int getCardCount();
	int getRowAttack();
	bool isPlayerRow();
	bool isEnemyRow();
	bool isEmpty();
	void setCardScale(float scale, float scaleSpeed);
	float getCardScale();
	void runEffect(CardEffects::CardEffect effect, std::vector<Card*> ignoreList = std::vector<Card*>());
	void clearEffects();
	void enableRowSelection(std::function<void(CardRow*)> callback);
	void enableRowCardSelection(std::function<void(Card*)> callback);
	void disableRowSelection();
	void disableRowCardSelection();
	void setMouseOverCallback(std::function<void(Card*)> callback);
	void setMouseOutCallback(std::function<void(Card*)> callback);
	void setMouseClickCallback(std::function<void(Card*)> callback);
	void enableRowCardInteraction();
	void disableRowCardInteraction();
	int simulateCardEffect(Card* card);
	void setRowWidth(float newRowWidth, float duration);
	void shuffle();
	void clear();
	void setCardPositions(float cardRepositionDelay, float indexDelay = 0.0f);

	std::vector<Card*> rowCards;

protected:
	CardRow(bool isPlayerRow);
	virtual ~CardRow();

	void onEnter() override;
	void initializeListeners() override;

private:
	typedef SmartNode super;

	void onRowSelectClick();

	float rowWidth = 0.0f;
	float cardScale = 0.0f;
	bool belongsToPlayer = false;
	ClickableNode* rowSelectSprite = nullptr;
	std::function<void(CardRow*)> rowSelectCallback = nullptr;
	bool clearOperationsOnInsert = false;
};
