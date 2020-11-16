#pragma once

#include "Scenes/Platformer/Level/Combat/Buffs/Buff.h"

namespace cocos2d
{
	class Sprite;
}

class PlatformerEntity;
class SmartParticles;

class Strength : public Buff
{
public:
	static Strength* create(PlatformerEntity* caster, PlatformerEntity* target);

	static const std::string StrengthIdentifier;

protected:
	Strength(PlatformerEntity* caster, PlatformerEntity* target);
	virtual ~Strength();

	void onEnter() override;
	void initializePositions() override;
	void registerHackables() override;
	void onBeforeDamageDealt(CombatEvents::ModifiableDamageOrHealingArgs* damageOrHealing) override;

private:
	typedef Buff super;

	void applyStrength();
	
	static volatile int currentDamageDealt;
	
	SmartParticles* spellEffect;
	cocos2d::Sprite* spellAura;
	
	static const int MinMultiplier;
	static const int MaxMultiplier;
	static const int DamageIncrease;
	static const float Duration;
};
