#pragma once

#include "Scenes/Platformer/AttachedBehavior/Entities/Friendly/Hexus/HexusPuzzleBehaviorBase.h"

class CardData;
class MinMaxPool;

class URPuzzleCBehavior : public HexusPuzzleBehaviorBase
{
public:
	static URPuzzleCBehavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	URPuzzleCBehavior(GameObject* owner);
	virtual ~URPuzzleCBehavior();

	MinMaxPool* generateReward() override;
	std::string getWinLossSaveKey() override;
	std::string getBackgroundResource() override;
	std::vector<CardData*> generateDeck() override;
	StateOverride* getStateOverride() override;
	std::vector<TutorialBase*> getTutorials() override;

private:
	typedef HexusPuzzleBehaviorBase super;
};
