#pragma once

#include "Scenes/Platformer/Level/Combat/Buffs/Buff.h"

namespace cocos2d
{
	class Sprite;
}

class PlatformerEntity;
class SmartParticles;

class Fortitude : public Buff
{
public:
	static Fortitude* create(PlatformerEntity* caster, PlatformerEntity* target);
	
	static const std::string FortitudeIdentifier;
	static const std::string HackIdentifierFortitude;

protected:
	Fortitude(PlatformerEntity* caster, PlatformerEntity* target);
	virtual ~Fortitude();

	void onEnter() override;
	void initializePositions() override;
	void registerHackables() override;
	void onBeforeDamageTaken(CombatEvents::ModifiableDamageOrHealingArgs* damageOrHealing) override;

private:
	typedef Buff super;

	void applyFortitude();
	
	cocos2d::Sprite* bubble = nullptr;
	SmartParticles* spellEffect = nullptr;
	cocos2d::Sprite* spellAura = nullptr;
	
	static const int MaxMultiplier;
	static const int DamageReduction;
	static const float Duration;
};
