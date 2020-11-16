#pragma once

#include "Scenes/Platformer/Level/Combat/Buffs/Buff.h"

namespace cocos2d
{
	class Sprite;
}

class PlatformerEntity;
class SmartParticles;

class Reflect : public Buff
{
public:
	static Reflect* create(PlatformerEntity* caster, PlatformerEntity* target);
	
	static const std::string ReflectIdentifier;

protected:
	Reflect(PlatformerEntity* caster, PlatformerEntity* target);
	virtual ~Reflect();

	void onEnter() override;
	void initializePositions() override;
	void registerHackables() override;
	void onBeforeDamageTaken(CombatEvents::ModifiableDamageOrHealingArgs* damageOrHealing) override;

private:
	typedef Buff super;

	void applyReflect();
	
	SmartParticles* spellEffect;
	cocos2d::Sprite* bubble;
	cocos2d::Sprite* spellAura;
	
	static const std::string StateKeyDamageReflected;

	static const int MinMultiplier;
	static const int MaxMultiplier;
	static const float Duration;
};
