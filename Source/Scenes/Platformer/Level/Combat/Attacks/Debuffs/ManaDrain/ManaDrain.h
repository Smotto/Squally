#pragma once

#include "Scenes/Platformer/Level/Combat/Buffs/Buff.h"

class PlatformerEntity;
class SmartAnimationSequenceNode;
class WorldSound;

class ManaDrain : public Buff
{
public:
	static ManaDrain* create(PlatformerEntity* caster, PlatformerEntity* target);

	static const std::string ManaDrainIdentifier;
	static const int DrainAmount;
	static const float TimeBetweenTicks;
	static const float StartDelay;

protected:
	ManaDrain(PlatformerEntity* caster, PlatformerEntity* target);
	virtual ~ManaDrain();

	void onEnter() override;
	void initializePositions() override;
	void registerHackables() override;
	void runManaDrain();
	void runRestoreTick();

private:
	typedef Buff super;

	void incrementHeal();

	int drainAmount = 0;
	SmartAnimationSequenceNode* healEffect = nullptr;
	WorldSound* impactSound = nullptr;
	WorldSound* healSound = nullptr;
};
