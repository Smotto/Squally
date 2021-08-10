#pragma once

#include "Engine/Quests/QuestTask.h"

class PlatformerEntity;
class Portal;
class QuestLine;
class Squally;
class Trigger;

class PowerWarpGateUR : public QuestTask
{
public:
	static PowerWarpGateUR* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	PowerWarpGateUR(GameObject* owner, QuestLine* questLine);
	virtual ~PowerWarpGateUR();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	void runCinematicSequence();

	Portal* portal;
	Trigger* trigger;
	PlatformerEntity* mage;
	Squally* squally = nullptr;
};
