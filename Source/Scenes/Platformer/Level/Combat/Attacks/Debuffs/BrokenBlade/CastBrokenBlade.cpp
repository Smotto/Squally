#include "CastBrokenBlade.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/CombatUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityBuffBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Debuffs/BrokenBlade/BrokenBlade.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

CastBrokenBlade* CastBrokenBlade::create(float attackDuration, float recoverDuration, Priority priority)
{
	CastBrokenBlade* instance = new CastBrokenBlade(attackDuration, recoverDuration, priority);

	instance->autorelease();

	return instance;
}

CastBrokenBlade::CastBrokenBlade(float attackDuration, float recoverDuration, Priority priority)
	: super(AttackType::Debuff, UIResources::Menus_Icons_SwordBrokenAlt, priority, AbilityType::Physical, 0, 0, 5, attackDuration, recoverDuration)
{
	this->castSound = WorldSound::create(SoundResources::Platformer_Spells_Heal5);
	
	this->addChild(this->castSound);
}

CastBrokenBlade::~CastBrokenBlade()
{
}

void CastBrokenBlade::initializePositions()
{
	super::initializePositions();
}

PlatformerAttack* CastBrokenBlade::cloneInternal()
{
	return CastBrokenBlade::create(this->getAttackDuration(), this->getRecoverDuration(), this->priority);
}

LocalizedString* CastBrokenBlade::getString()
{
	return Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_BrokenBlade::create();
}

std::string CastBrokenBlade::getAttackAnimation()
{
	return "Attack";
}

void CastBrokenBlade::performAttack(PlatformerEntity* owner, std::vector<PlatformerEntity*> targets)
{
	super::performAttack(owner, targets);

	this->castSound->play();
	owner->getAnimations()->clearAnimationPriority();
	owner->getAnimations()->playAnimation(this->getAttackAnimation());

	for (PlatformerEntity* next : targets)
	{
		next->getComponent<EntityBuffBehavior>([=](EntityBuffBehavior* entityBuffBehavior)
		{
			entityBuffBehavior->applyBuff(BrokenBlade::create(owner, next));
		});
	}
}

void CastBrokenBlade::onCleanup()
{
}

bool CastBrokenBlade::isWorthUsing(PlatformerEntity* caster, const std::vector<PlatformerEntity*>& sameTeam, const std::vector<PlatformerEntity*>& otherTeam)
{
	int uncastableCount = 0;

	for (PlatformerEntity* next : otherTeam)
	{
		if (!next->getRuntimeStateOrDefaultBool(StateKeys::IsAlive, true))
		{
			uncastableCount++;
			continue;
		}

		if (CombatUtils::HasDuplicateCastOnLivingTarget(caster, next, [](PlatformerAttack* next) { return dynamic_cast<CastBrokenBlade*>(next) != nullptr;  }))
		{
			uncastableCount++;
			continue;
		}

		next->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
		{
			entityBuffBehavior->getBuff<BrokenBlade>([&](BrokenBlade* debuff)
			{
				uncastableCount++;
			});
		});
	}

	return uncastableCount != int(otherTeam.size());
}

float CastBrokenBlade::getUseUtility(PlatformerEntity* caster, PlatformerEntity* target, const std::vector<PlatformerEntity*>& sameTeam, const std::vector<PlatformerEntity*>& otherTeam)
{
	float utility = CombatUtils::HasDuplicateCastOnLivingTarget(caster, target, [](PlatformerAttack* next) { return dynamic_cast<CastBrokenBlade*>(next) != nullptr;  }) ? 0.0f : 1.0f;

	target->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
	{
		entityBuffBehavior->getBuff<BrokenBlade>([&](BrokenBlade* debuff)
		{
			utility = 0.0f;
		});
	});
	
	return utility;
}
