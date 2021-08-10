#include "Weapon.h"

#include "Resources/SoundResources.h"

using namespace cocos2d;

Weapon::Weapon(CurrencyInventory* cost, int minAttack, int maxAttack, ItemStats itemStats) : super(cost, itemStats)
{
	this->minAttack = minAttack;
	this->maxAttack = maxAttack;
}

Weapon::~Weapon()
{
}

int Weapon::getMinAttack()
{
	return this->minAttack;
}

int Weapon::getMaxAttack()
{
	return this->maxAttack;
}

float Weapon::getAttackOnset()
{
	return 0.05f;
}

float Weapon::getAttackSustain()
{
	return 0.35f;
}

CSize Weapon::getWeaponCollisionSize()
{
	// As expected: Weapon width, height
	return CSize(32.0f, 160.0f);
}

Vec2 Weapon::getWeaponCollisionOffset()
{
	// Unintuitive: x influences y position, y influences x, likely due to initial weapon rotation
	return Vec2(0.0f, 192.0f);
}

std::string Weapon::getWeaponAttackSound()
{
	return SoundResources::Platformer_Physical_Swings_SwingWeakHybrid1;
}
