#include "Axe.h"

#include "Resources/SoundResources.h"

using namespace cocos2d;

Axe::Axe(CurrencyInventory* cost, int minAttack, int maxAttack, ItemStats itemStats) : super(cost, minAttack, maxAttack, itemStats)
{
}

Axe::~Axe()
{
}

std::string Axe::getWeaponAttackSound()
{
	return SoundResources::Platformer_Physical_Swings_SwingWeakHybrid3;
}

std::string Axe::getWeaponImpactSound()
{
	return SoundResources::Platformer_Physical_Impact_HitSoft1;
}
