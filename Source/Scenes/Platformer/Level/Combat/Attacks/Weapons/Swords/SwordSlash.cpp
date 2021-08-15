#include "SwordSlash.h"

#include "Events/CombatEvents.h"
#include "Engine/Camera/GameCamera.h"
#include "Engine/Sound/WorldSound.h"

#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

SwordSlash* SwordSlash::create(int damageMin, int damageMax, float attackDuration, float recoverDuration, Priority priority)
{
	SwordSlash* instance = new SwordSlash(damageMin, damageMax, attackDuration, recoverDuration, priority);

	instance->autorelease();

	return instance;
}

SwordSlash::SwordSlash(int damageMin, int damageMax, float attackDuration, float recoverDuration, Priority priority)
	: super(AttackType::Damage, UIResources::Menus_Icons_SwordSlash, priority, AbilityType::Physical, damageMin, damageMax, 0, attackDuration, recoverDuration)
{
	this->swordSlashSound = WorldSound::create(SoundResources::Platformer_Physical_Swings_SwingBlade1);
	this->hitSound = WorldSound::create(SoundResources::Platformer_Physical_Impact_HitSoft1);

	this->addChild(this->swordSlashSound);
	this->addChild(this->hitSound);
}

SwordSlash::~SwordSlash()
{
}

PlatformerAttack* SwordSlash::cloneInternal()
{
	return SwordSlash::create(this->getBaseDamageMin(), this->getBaseDamageMax(), this->getAttackDuration(), this->getRecoverDuration(), this->priority);
}

LocalizedString* SwordSlash::getString()
{
	return Strings::Platformer_Combat_Attack::create();
}

std::string SwordSlash::getAttackAnimation()
{
	return "AttackSlash";
}

void SwordSlash::onAttackTelegraphBegin()
{
	super::onAttackTelegraphBegin();
	
	this->swordSlashSound->play(false, this->attackDuration / 2.0f);
}

void SwordSlash::performAttack(PlatformerEntity* owner, std::vector<PlatformerEntity*> targets)
{
	super::performAttack(owner, targets);
	
	for (auto next : targets)
	{
		this->doDamageOrHealing(owner, next);
	}
}

void SwordSlash::doDamageOrHealing(PlatformerEntity* owner, PlatformerEntity* target)
{
	this->hitSound->play();
	CombatEvents::TriggerDamage(CombatEvents::DamageOrHealingArgs(owner, target, this->getRandomDamage(), this->abilityType));

	GameCamera::getInstance()->shakeCamera(0.2f, 12.0f, 0.3f);
}
