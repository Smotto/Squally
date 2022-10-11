#pragma once

#include "Scenes/Platformer/Components/Entities/Friendly/Hexus/HexusBehaviorBase.h"

class CardData;
class MinMaxPool;

class AphroditeHexusBehavior : public HexusBehaviorBase
{
public:
	static AphroditeHexusBehavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	AphroditeHexusBehavior(GameObject* owner);
	virtual ~AphroditeHexusBehavior();

	MinMaxPool* generateReward() override;
	std::string getWinLossSaveKey() override;
	std::string getBackgroundResource() override;
	std::vector<CardData*> generateDeck() override;
	StateOverride* getStateOverride() override;
	std::vector<TutorialBase*> getTutorials() override;

private:
	typedef HexusBehaviorBase super;

};
