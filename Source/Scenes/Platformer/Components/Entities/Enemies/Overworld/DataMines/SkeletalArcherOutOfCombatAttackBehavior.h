#pragma once

#include "Scenes/Platformer/Components/Entities/Enemies/Overworld/EnemyOutOfCombatAttackBehavior.h"

namespace cocos2d
{
	class Sprite;
};

class AgroBehavior;
class PlatformerEnemy;
class Projectile;
class SmartAnimationSequenceNode;
class Weapon;

class SkeletalArcherOutOfCombatAttackBehavior : public EnemyOutOfCombatAttackBehavior
{
public:
	static SkeletalArcherOutOfCombatAttackBehavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	SkeletalArcherOutOfCombatAttackBehavior(GameObject* owner);
	virtual ~SkeletalArcherOutOfCombatAttackBehavior();

	void onLoad() override;
	void onDisable() override;
	void update(float dt) override;
	Projectile* createProjectile() override;

private:
	typedef EnemyOutOfCombatAttackBehavior super;
	
	std::string getOutOfCombatAttackAnimation() override;
	std::string getOutOfCombatAttackSound() override;
	float getOutOfCombatAttackOnset() override;
	float getOutOfCombatAttackSustain() override;
	float getOutOfCombatAttackCooldown() override;
	float getProjectileLifetime() override;
	
	PlatformerEnemy* skeletalArcher = nullptr;
	AgroBehavior* agroBehavior = nullptr;
};
