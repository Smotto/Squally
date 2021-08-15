#pragma once

#include "Scenes/Platformer/Level/Combat/Buffs/Buff.h"

namespace cocos2d
{
	class Sprite;
}

class HackablePreview;
class PlatformerEntity;
class SmartParticles;
class WorldSound;

class Haste : public Buff
{
public:
	static Haste* create(PlatformerEntity* caster, PlatformerEntity* target);
	
	static const std::string HasteIdentifier;

protected:
	Haste(PlatformerEntity* caster, PlatformerEntity* target);
	virtual ~Haste();

	void onEnter() override;
	void initializePositions() override;
	void registerHackables() override;
	void onModifyTimelineSpeed(CombatEvents::ModifiableTimelineSpeedArgs* speed) override;

private:
	typedef Buff super;

	void applyHaste();
	
	SmartParticles* spellEffect = nullptr;
	cocos2d::Sprite* spellAura = nullptr;
	
	static volatile float currentSpeed;

	static const float MinSpeed;
	static const float DefaultSpeed;
	static const float MaxSpeed;
	static const float Duration;
};
