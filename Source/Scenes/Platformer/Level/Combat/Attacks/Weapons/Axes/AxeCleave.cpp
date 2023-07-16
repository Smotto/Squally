#include "AxeCleave.h"

#include "Events/CombatEvents.h"
#include "Engine/Camera/GameCamera.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Sound/WorldSound.h"

#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const float AxeCleave::DamageMultiplier = 0.75f;

AxeCleave* AxeCleave::create(int damageMin, int damageMax, float attackDuration, float recoverDuration, Priority priority)
{
	AxeCleave* instance = new AxeCleave(damageMin, damageMax, attackDuration, recoverDuration, priority);

	instance->autorelease();

	return instance;
}

AxeCleave::AxeCleave(int damageMin, int damageMax, float attackDuration, float recoverDuration, Priority priority)
	: super(
		AttackType::Damage,
		UIResources::Menus_Icons_AxeSlash,
		priority,
		AbilityType::Physical,
		damageMin,
		damageMax,
		8,
		attackDuration,
		recoverDuration,
		TargetingType::Multi
	)
{
	this->slashSound = WorldSound::create(SoundResources::Platformer_Physical_Swings_SwingWeakHybrid3);
	this->hitSound = WorldSound::create(SoundResources::Platformer_Physical_Impact_HitHeavy1);

	this->setDamageMultiplier(AxeCleave::DamageMultiplier);

	this->addChild(this->slashSound);
	this->addChild(this->hitSound);
}

AxeCleave::~AxeCleave()
{
}

PlatformerAttack* AxeCleave::cloneInternal()
{
	return AxeCleave::create(this->getBaseDamageMin(), this->getBaseDamageMax(), this->getAttackDuration(), this->getRecoverDuration(), this->priority);
}

LocalizedString* AxeCleave::getString()
{
	return Strings::Platformer_Combat_Attacks_Cleave::create();
}

LocalizedString* AxeCleave::getDescription()
{
	return Strings::Platformer_Combat_Attacks_CleaveDescription::create()
		->setStringReplacementVariables(ConstantString::create(std::to_string(int(AxeCleave::DamageMultiplier * 100.0f))));
}

std::string AxeCleave::getAttackAnimation()
{
	return "AttackSlash";
}

void AxeCleave::onAttackTelegraphBegin()
{
	super::onAttackTelegraphBegin();
	
	// Sound will be changed out later
	this->slashSound->play(false, this->attackDuration / 2.0f);
}

void AxeCleave::performAttack(PlatformerEntity* owner, std::vector<PlatformerEntity*> targets)
{
	super::performAttack(owner, targets);
	
	for (PlatformerEntity* next : targets)
	{
		this->doDamageOrHealing(owner, next);
	}
}

void AxeCleave::doDamageOrHealing(PlatformerEntity* owner, PlatformerEntity* target)
{
	this->hitSound->play();
	CombatEvents::TriggerDamage(CombatEvents::DamageOrHealingArgs(owner, target, this->getRandomDamage(), this->abilityType));

	GameCamera::getInstance()->shakeCamera(0.2f, 12.0f, 0.3f);
}
