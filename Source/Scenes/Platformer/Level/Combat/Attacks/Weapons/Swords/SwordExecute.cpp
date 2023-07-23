#include "SwordExecute.h"

#include "Events/CombatEvents.h"
#include "Engine/Camera/GameCamera.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Sound/WorldSound.h"

#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const float SwordExecute::DamageMultiplier = 1.65f;

SwordExecute* SwordExecute::create(int damageMin, int damageMax, float attackDuration, float recoverDuration, Priority priority)
{
	SwordExecute* instance = new SwordExecute(damageMin, damageMax, attackDuration, recoverDuration, priority);

	instance->autorelease();

	return instance;
}

SwordExecute::SwordExecute(int damageMin, int damageMax, float attackDuration, float recoverDuration, Priority priority)
	: super(
		AttackType::Damage,
		UIResources::Menus_Icons_BleedingLimb,
		priority,
		AbilityType::Physical,
		damageMin,
		damageMax,
		6,
		attackDuration,
		recoverDuration
	)
{
	this->slashSound = WorldSound::create(SoundResources::Platformer_Physical_Swings_SwingBlade2);
	this->hitSound = WorldSound::create(SoundResources::Platformer_Physical_Impact_HitHeavy1);

	this->setDamageMultiplier(SwordExecute::DamageMultiplier);

	this->addChild(this->slashSound);
	this->addChild(this->hitSound);
}

SwordExecute::~SwordExecute()
{
}

PlatformerAttack* SwordExecute::cloneInternal()
{
	return SwordExecute::create(this->getBaseDamageMin(), this->getBaseDamageMax(), this->getAttackDuration(), this->getRecoverDuration(), this->priority);
}

LocalizedString* SwordExecute::getString()
{
	return Strings::Platformer_Combat_Attacks_Execute::create();
}

LocalizedString* SwordExecute::getDescription()
{
	return Strings::Platformer_Combat_Attacks_ExecuteDescription::create()
		->setStringReplacementVariables(ConstantString::create(std::to_string(int(SwordExecute::DamageMultiplier * 100.0f))));
}

std::string SwordExecute::getAttackAnimation()
{
	return "AttackSlash";
}

void SwordExecute::onAttackTelegraphBegin()
{
	super::onAttackTelegraphBegin();
	
	this->slashSound->play(false, this->attackDuration / 2.0f);
}

void SwordExecute::performAttack(PlatformerEntity* owner, std::vector<PlatformerEntity*> targets)
{
	super::performAttack(owner, targets);
	
	for (PlatformerEntity* next : targets)
	{
		this->doDamageOrHealing(owner, next);
	}
}

void SwordExecute::doDamageOrHealing(PlatformerEntity* owner, PlatformerEntity* target)
{
	this->hitSound->play();
	CombatEvents::TriggerDamage(CombatEvents::DamageOrHealingArgs(owner, target, this->getRandomDamage(), this->abilityType));

	GameCamera::getInstance()->shakeCamera(0.2f, 12.0f, 0.3f);
}
