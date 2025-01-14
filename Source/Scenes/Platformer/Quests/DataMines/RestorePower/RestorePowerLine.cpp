#include "RestorePowerLine.h"

#include "Engine/Quests/QuestTask.h"
#include "Scenes/Platformer/Quests/DataMines/RestorePower/BancroftTrainAccess.h"
#include "Scenes/Platformer/Quests/DataMines/RestorePower/DefeatKrampus.h"
#include "Scenes/Platformer/Quests/DataMines/RestorePower/RestorePower.h"
#include "Scenes/Platformer/Quests/DataMines/RestorePower/ReturnToPrincessDawn.h"
#include "Scenes/Platformer/Quests/DataMines/RestorePower/TalkToOlive.h"
#include "Scenes/Platformer/Quests/DataMines/RestorePower/TalkToPrincessDawn.h"

using namespace cocos2d;

const std::string RestorePowerLine::MapKeyQuestLine = "restore-power";

RestorePowerLine* RestorePowerLine::create()
{
	RestorePowerLine* instance = new RestorePowerLine();

	instance->autorelease();

	return instance;
}

RestorePowerLine::RestorePowerLine() : super(RestorePowerLine::MapKeyQuestLine, {
	QuestData(TalkToOlive::MapKeyQuest, true, [](GameObject* owner, QuestLine* questLine) { return TalkToOlive::create(owner, questLine); }),
	QuestData(TalkToPrincessDawn::MapKeyQuest, true, [](GameObject* owner, QuestLine* questLine) { return TalkToPrincessDawn::create(owner, questLine); }),
	QuestData(DefeatKrampus::MapKeyQuest, true, [](GameObject* owner, QuestLine* questLine) { return DefeatKrampus::create(owner, questLine); }),
	QuestData(RestorePower::MapKeyQuest, false, [](GameObject* owner, QuestLine* questLine) { return RestorePower::create(owner, questLine); }),
	QuestData(ReturnToPrincessDawn::MapKeyQuest, false, [](GameObject* owner, QuestLine* questLine) { return ReturnToPrincessDawn::create(owner, questLine); }),
	QuestData(BancroftTrainAccess::MapKeyQuest, false, [](GameObject* owner, QuestLine* questLine) { return BancroftTrainAccess::create(owner, questLine); }),
})
{
}

RestorePowerLine::~RestorePowerLine()
{
}

QuestLine* RestorePowerLine::clone()
{
	return RestorePowerLine::create();
}
