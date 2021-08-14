#pragma once

#include "Engine/SmartNode.h"

class Card;
class ClickableNode;

class AutoCard : public SmartNode
{
public:
	static AutoCard* create(int defaultAttack = 5);

	enum class DisplayType
	{
		Binary,
		Decimal,
		Hex
	};

	void incrementAttack();
	void decrementAttack();
	void setAttack(int attack);
	int getAttack();
	void setDisplayType(DisplayType displayType);
	DisplayType getDisplayType();
	void setCardScale(float cardScale);

	Card* activeCard;

protected:
	AutoCard(int defaultAttack);
	virtual ~AutoCard();

	void onEnter() override;

private:
	typedef SmartNode super;

	void updateToggle();

	cocos2d::Node* cardNode = nullptr;
	ClickableNode* upToggle = nullptr;
	ClickableNode* downToggle = nullptr;

	DisplayType displayType = DisplayType::Binary;
	float cardScale = 1.0f;
	int currentAttack = 0;

	std::map<int, Card*> binaryCardMap;
	std::map<int, Card*> decimalCardMap;
	std::map<int, Card*> hexCardMap;

	std::map<int, std::function<Card*()>> binaryCardFactory;
	std::map<int, std::function<Card*()>> decimalCardFactory;
	std::map<int, std::function<Card*()>> hexCardFactory;
};
