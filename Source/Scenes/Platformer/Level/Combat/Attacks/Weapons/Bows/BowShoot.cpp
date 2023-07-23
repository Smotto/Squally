#include "BowShoot.h"

#include "Events/CombatEvents.h"
#include "Engine/Camera/GameCamera.h"
#include "Engine/Sound/WorldSound.h"

#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

BowShoot* BowShoot::create(int damageMin, int damageMax, float attackDuration, float recoverDuration, Priority priority)
{
	BowShoot* instance = new BowShoot(damageMin, damageMax, attackDuration, recoverDuration, priority);

	instance->autorelease();

	return instance;
}

BowShoot::BowShoot(int damageMin, int damageMax, float attackDuration, float recoverDuration, Priority priority)
	: super(
		AttackType::Damage,
		UIResources::Menus_Icons_Arrow,
		priority,
		AbilityType::Physical,
		damageMin,
		damageMax,
		0,
		attackDuration,
		recoverDuration
	)
{
	this->slashSound = WorldSound::create(SoundResources::Platformer_Physical_Projectiles_Woosh1);
	this->hitSound = WorldSound::create(SoundResources::Platformer_Physical_Impact_HitSoft5);

	this->addChild(this->slashSound);
	this->addChild(this->hitSound);
}

BowShoot::~BowShoot()
{
}

PlatformerAttack* BowShoot::cloneInternal()
{
	return BowShoot::create(this->getBaseDamageMin(), this->getBaseDamageMax(), this->getAttackDuration(), this->getRecoverDuration(), this->priority);
}

LocalizedString* BowShoot::getString()
{
	return Strings::Platformer_Combat_Attacks_Shoot::create();
}

std::string BowShoot::getAttackAnimation()
{
	return "AttackShoot";
}

void BowShoot::onAttackTelegraphBegin()
{
	super::onAttackTelegraphBegin();
	
	// Sound will be changed out later
	this->slashSound->play(false, this->attackDuration / 2.0f);
}

void BowShoot::performAttack(PlatformerEntity* owner, std::vector<PlatformerEntity*> targets)
{
	super::performAttack(owner, targets);
	
	for (PlatformerEntity* next : targets)
	{
		this->doDamageOrHealing(owner, next);
	}
}

void BowShoot::doDamageOrHealing(PlatformerEntity* owner, PlatformerEntity* target)
{
	this->hitSound->play();
	CombatEvents::TriggerDamage(CombatEvents::DamageOrHealingArgs(owner, target, this->getRandomDamage(), this->abilityType));

	GameCamera::getInstance()->shakeCamera(0.2f, 12.0f, 0.3f);
}
