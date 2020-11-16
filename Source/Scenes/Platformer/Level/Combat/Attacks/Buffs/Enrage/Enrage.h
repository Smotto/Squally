#pragma once

#include "Scenes/Platformer/Level/Combat/Buffs/Buff.h"

namespace cocos2d
{
	class Sprite;
}

class PlatformerEntity;
class SmartParticles;

class Enrage : public Buff
{
public:
	static Enrage* create(PlatformerEntity* caster, PlatformerEntity* target);
	
	static const std::string EnrageIdentifier;

protected:
	Enrage(PlatformerEntity* caster, PlatformerEntity* target);
	virtual ~Enrage();

	void onEnter() override;
	void initializePositions() override;
	void registerHackables() override;
	void onModifyTimelineSpeed(CombatEvents::ModifiableTimelineSpeedArgs* speed) override;
	void onBeforeDamageTaken(CombatEvents::ModifiableDamageOrHealingArgs* damageOrHealing) override;
	void onBeforeDamageDealt(CombatEvents::ModifiableDamageOrHealingArgs* damageOrHealing) override;

private:
	typedef Buff super;

	void applyEnrageSpeed();
	void applyEnrageIncreaseDamageDealt();
	void applyEnrageIncreaseDamageTaken();
	
	static volatile float currentSpeed;
	
	SmartParticles* spellEffect;
	cocos2d::Sprite* spellAura;

	static const float MinSpeed;
	static const float DefaultSpeed;
	static const float MaxSpeed;
	static const float Duration;
};
