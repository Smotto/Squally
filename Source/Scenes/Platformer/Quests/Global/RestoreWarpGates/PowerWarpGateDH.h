#pragma once

#include "Engine/Quests/QuestTask.h"

class PlatformerEntity;
class Portal;
class QuestLine;
class Trigger;

class PowerWarpGateDH : public QuestTask
{
public:
	static PowerWarpGateDH* create(GameObject* owner, QuestLine* questLine);

	static const std::string MapKeyQuest;

protected:
	PowerWarpGateDH(GameObject* owner, QuestLine* questLine);
	virtual ~PowerWarpGateDH();

	void onLoad(QuestState questState) override;
	void onActivate(bool isActiveThroughSkippable) override;
	void onComplete() override;
	void onSkipped() override;

private:
	typedef QuestTask super;

	Portal* portal = nullptr;
	Trigger* trigger = nullptr;
	PlatformerEntity* mage = nullptr;
};
