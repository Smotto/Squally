#include "WandEnergyBolt.h"

#include "Events/CombatEvents.h"
#include "Engine/Camera/GameCamera.h"
#include "Engine/Sound/WorldSound.h"

#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const float WandEnergyBolt::DamageMultiplier = 1.75f;

WandEnergyBolt* WandEnergyBolt::create(int damageMin, int damageMax, float attackDuration, float recoverDuration, Priority priority)
{
	WandEnergyBolt* instance = new WandEnergyBolt(damageMin, damageMax, attackDuration, recoverDuration, priority);

	instance->autorelease();

	return instance;
}

WandEnergyBolt::WandEnergyBolt(int damageMin, int damageMax, float attackDuration, float recoverDuration, Priority priority)
	: super(
		AttackType::Damage,
		UIResources::Menus_Icons_ThunderBoltBlue,
		priority,
		AbilityType::Lightning,
		damageMin,
		damageMax,
		6,
		attackDuration,
		recoverDuration
	)
{
	this->slashSound = WorldSound::create(SoundResources::Platformer_Physical_Swings_SwingWeakHybrid2);
	this->hitSound = WorldSound::create(SoundResources::Platformer_Physical_Impact_HitSoft1);

	this->setDamageMultiplier(WandEnergyBolt::DamageMultiplier);

	this->addChild(this->slashSound);
	this->addChild(this->hitSound);
}

WandEnergyBolt::~WandEnergyBolt()
{
}

PlatformerAttack* WandEnergyBolt::cloneInternal()
{
	return WandEnergyBolt::create(this->getBaseDamageMin(), this->getBaseDamageMax(), this->getAttackDuration(), this->getRecoverDuration(), this->priority);
}

LocalizedString* WandEnergyBolt::getString()
{
	return Strings::Platformer_Combat_Attacks_EnergyBolt::create();
}

LocalizedString* WandEnergyBolt::getDescription()
{
	return Strings::Platformer_Combat_Attacks_EnergyBoltDescription::create();
}

std::string WandEnergyBolt::getAttackAnimation()
{
	return "AttackSlash"; // Keep this the same for now
}

void WandEnergyBolt::onAttackTelegraphBegin()
{
	super::onAttackTelegraphBegin();
	
	// Sound will be changed out later
	this->slashSound->play(false, this->attackDuration / 2.0f);
}

void WandEnergyBolt::performAttack(PlatformerEntity* owner, std::vector<PlatformerEntity*> targets)
{
	super::performAttack(owner, targets);
	
	for (PlatformerEntity* next : targets)
	{
		this->doDamageOrHealing(owner, next);
	}
}

void WandEnergyBolt::doDamageOrHealing(PlatformerEntity* owner, PlatformerEntity* target)
{
	this->hitSound->play();
	CombatEvents::TriggerDamage(CombatEvents::DamageOrHealingArgs(owner, target, this->getRandomDamage(), this->abilityType));

	GameCamera::getInstance()->shakeCamera(0.2f, 12.0f, 0.3f);
}
