#include "CastStrength.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Sound/WorldSound.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityBuffBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Buffs/Strength/Strength.h"

#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

CastStrength* CastStrength::create(float attackDuration, float recoverDuration, Priority priority)
{
	CastStrength* instance = new CastStrength(attackDuration, recoverDuration, priority);

	instance->autorelease();

	return instance;
}

CastStrength::CastStrength(float attackDuration, float recoverDuration, Priority priority)
	: super(AttackType::Buff, UIResources::Menus_Icons_Gauntlet, priority, AbilityType::Physical, 0, 0, 4, attackDuration, recoverDuration)
{
	this->castSound = WorldSound::create(SoundResources::Platformer_Spells_Heal5);

	this->addChild(this->castSound);
}

CastStrength::~CastStrength()
{
}

void CastStrength::initializePositions()
{
	super::initializePositions();
}

PlatformerAttack* CastStrength::cloneInternal()
{
	return CastStrength::create(this->getAttackDuration(), this->getRecoverDuration(), this->priority);
}

LocalizedString* CastStrength::getString()
{
	return Strings::Menus_Hacking_Abilities_Buffs_Strength_Strength::create();
}

std::string CastStrength::getAttackAnimation()
{
	return "AttackCast";
}

void CastStrength::performAttack(PlatformerEntity* owner, std::vector<PlatformerEntity*> targets)
{
	super::performAttack(owner, targets);

	this->castSound->play();
	owner->getAnimations()->clearAnimationPriority();
	owner->getAnimations()->playAnimation(this->getAttackAnimation());

	for (auto next : targets)
	{
		next->getComponent<EntityBuffBehavior>([=](EntityBuffBehavior* entityBuffBehavior)
		{
			entityBuffBehavior->applyBuff(Strength::create(owner, next));
		});
	}
}

void CastStrength::onCleanup()
{
}

bool CastStrength::isWorthUsing(PlatformerEntity* caster, const std::vector<PlatformerEntity*>& sameTeam, const std::vector<PlatformerEntity*>& otherTeam)
{
	bool hasBuff = false;

	caster->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
	{
		entityBuffBehavior->getBuff<Strength>([&](Strength* buff)
		{
			hasBuff = true;
		});
	});

	return !hasBuff;
}

float CastStrength::getUseUtility(PlatformerEntity* caster, PlatformerEntity* target, const std::vector<PlatformerEntity*>& sameTeam, const std::vector<PlatformerEntity*>& otherTeam)
{
	// Prioritize self-cast
	return target == this->owner ? 1.0f : 0.0f;
}
