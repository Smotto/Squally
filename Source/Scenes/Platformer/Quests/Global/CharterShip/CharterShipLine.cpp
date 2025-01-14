#include "CharterShipLine.h"

#include "Engine/Quests/QuestTask.h"
#include "Scenes/Platformer/Quests/DataMines/RestorePower/DefeatKrampus.h"
#include "Scenes/Platformer/Quests/DataMines/RestorePower/RestorePowerLine.h"
#include "Scenes/Platformer/Quests/Global/CharterShip/BoardCharterShip.h"
#include "Scenes/Platformer/Quests/Global/CharterShip/CharterShip.h"
#include "Scenes/Platformer/Quests/Global/CharterShip/DiscoverCharterShip.h"

using namespace cocos2d;

const std::string CharterShipLine::MapKeyQuestLine = "charter-ship";

CharterShipLine* CharterShipLine::create()
{
	CharterShipLine* instance = new CharterShipLine();

	instance->autorelease();

	return instance;
}

CharterShipLine::CharterShipLine() : super(CharterShipLine::MapKeyQuestLine,
	{
		QuestData(DiscoverCharterShip::MapKeyQuest, false, [](GameObject* owner, QuestLine* questLine) { return DiscoverCharterShip::create(owner, questLine); }),
		QuestData(BoardCharterShip::MapKeyQuest, true, [](GameObject* owner, QuestLine* questLine) { return BoardCharterShip::create(owner, questLine); }),
		QuestData(CharterShip::MapKeyQuest, false, [](GameObject* owner, QuestLine* questLine) { return CharterShip::create(owner, questLine); }),
	})
{
}

CharterShipLine::~CharterShipLine()
{
}

QuestLine* CharterShipLine::clone()
{
	return CharterShipLine::create();
}
