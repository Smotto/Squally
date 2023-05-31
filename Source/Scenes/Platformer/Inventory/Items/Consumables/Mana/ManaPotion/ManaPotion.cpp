#include "ManaPotion.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Localization/LocalizedString.h"
#include "Engine/Sound/Sound.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Stats/EntityManaBehavior.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Consumables/Mana/ManaPotion/ThrowManaPotion.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/ItemResources.h"
#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string ManaPotion::SaveKey = "mana-potion";
const float ManaPotion::RestorePercentage = 0.4f;

ManaPotion* ManaPotion::create()
{
	ManaPotion* instance = new ManaPotion();

	instance->autorelease();

	return instance;
}

ManaPotion::ManaPotion() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 17 }}), ItemMeta(20, RubberBanding(3, 0.15f)), true)
{
	this->outOfCombatSound = Sound::create(SoundResources::Platformer_FX_Potions_PotionDrink2);

	this->addChild(this->outOfCombatSound);
}

ManaPotion::~ManaPotion()
{
}

void ManaPotion::useOutOfCombat(PlatformerEntity* target)
{
	target->getComponent<EntityManaBehavior>([=](EntityManaBehavior* manaBehavior)
	{
		manaBehavior->setMana(manaBehavior->getMana() + int(float(manaBehavior->getMaxMana()) * ManaPotion::RestorePercentage));
	});

	this->outOfCombatSound->play();
}

bool ManaPotion::canUseOnTarget(PlatformerEntity* target)
{
	if (!target->getRuntimeStateOrDefaultBool(StateKeys::IsAlive, true))
	{
		return false;
	}
	
	bool canUse = true;

	target->getComponent<EntityManaBehavior>([&](EntityManaBehavior* manaBehavior)
	{
		if (manaBehavior->getMana() == manaBehavior->getMaxMana())
		{
			canUse = false;
		}
	});
	
	return canUse;
}

LocalizedString* ManaPotion::getDescription()
{
	return Strings::Items_Consumables_Mana_ManaPotionDescription::create()
		->setStringReplacementVariables(ConstantString::create(std::to_string(int(ManaPotion::RestorePercentage * 100.0f))));
}

Item* ManaPotion::clone()
{
	return ManaPotion::create();
}

LocalizedString* ManaPotion::getString()
{
	return Strings::Items_Consumables_Mana_ManaPotion::create();
}

const std::string& ManaPotion::getIconResource()
{
	return ItemResources::Consumables_Potions_ManaPotion;
}

const std::string& ManaPotion::getIdentifier()
{
	return ManaPotion::SaveKey;
}

PlatformerAttack* ManaPotion::createAssociatedAttack()
{
	return ThrowManaPotion::create(PlatformerAttack::Priority::Common);
}
