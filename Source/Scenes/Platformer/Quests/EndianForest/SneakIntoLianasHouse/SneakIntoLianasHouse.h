#pragma once

#include "Engine/Quests/QuestTask.h"

class Portal;
class QuestLine;

class SneakIntoLianasHouse : public QuestTask
{
public:
	static SneakIntoLianasHouse* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	SneakIntoLianasHouse(GameObject* owner, QuestLine* questLine);
	virtual ~SneakIntoLianasHouse();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable, bool isInitialActivation) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	Portal* portal = nullptr;
};
