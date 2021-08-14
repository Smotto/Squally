#pragma once

#include "Scenes/Platformer/Components/Entities/Enemies/Overworld/EnemyOutOfCombatAttackBehavior.h"

class AgroBehavior;
class Projectile;
class OrcBomber;
class SmartAnimationSequenceNode;
class Weapon;

class OrcBomberOutOfCombatAttackBehavior : public EnemyOutOfCombatAttackBehavior
{
public:
	static OrcBomberOutOfCombatAttackBehavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	OrcBomberOutOfCombatAttackBehavior(GameObject* owner);
	virtual ~OrcBomberOutOfCombatAttackBehavior();

	void onLoad() override;
	void onDisable() override;
	void update(float dt) override;
	std::string getOutOfCombatAttackAnimation() override;
	std::string getOutOfCombatAttackSound() override;
	float getOutOfCombatAttackOnset() override;
	float getOutOfCombatAttackSustain() override;
	float getOutOfCombatAttackCooldown() override;
	float getProjectileLifetime() override;
	Projectile* createProjectile() override;

private:
	typedef EnemyOutOfCombatAttackBehavior super;
	
	AgroBehavior* agroBehavior = nullptr;
	SmartAnimationSequenceNode* fireBreath = nullptr;
	OrcBomber* orcBomber = nullptr;
};
