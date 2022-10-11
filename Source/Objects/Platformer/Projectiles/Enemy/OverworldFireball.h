#pragma once

#include "Objects/Platformer/Projectiles/Projectile.h"

class SmartAnimationSequenceNode;
class WorldSound;

class OverworldFireball : public Projectile
{
public:
	static OverworldFireball* create();

	void runSpawnFX() override;
	void runImpactFX() override;

protected:
	OverworldFireball();
	virtual ~OverworldFireball();

	void onEnter() override;

private:
	typedef Projectile super;

	WorldSound* fireballSfx = nullptr;
	WorldSound* impactSfx = nullptr;

	SmartAnimationSequenceNode* fireball = nullptr;
	SmartAnimationSequenceNode* explosion = nullptr;
};
