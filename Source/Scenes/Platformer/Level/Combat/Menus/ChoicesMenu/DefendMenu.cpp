#include "DefendMenu.h"

#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"

#include "Engine/Input/ClickableTextNode.h"
#include "Engine/Inventory/Inventory.h"
#include "Engine/Inventory/Item.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/Utils/MathUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Events/CombatEvents.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityAttackBehavior.h"
#include "Scenes/Platformer/Inventory/Items/Consumables/Consumable.h"
#include "Scenes/Platformer/Level/Combat/Attacks/PlatformerAttack.h"
#include "Scenes/Platformer/Level/Combat/Menus/ChoicesMenu/RadialEntry.h"
#include "Scenes/Platformer/Level/Combat/Timeline.h"
#include "Scenes/Platformer/Level/Combat/TimelineEntry.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const float DefendMenu::Radius = 384.0f;

DefendMenu* DefendMenu::create(Timeline* timelineRef)
{
	DefendMenu* instance = new DefendMenu(timelineRef);

	instance->autorelease();

	return instance;
}

DefendMenu::DefendMenu(Timeline* timelineRef) : super(DefendMenu::Radius, float(M_PI) / 9.0f)
{
	this->timelineRef = timelineRef;
}

DefendMenu::~DefendMenu()
{
}

void DefendMenu::buildAttackList(TimelineEntry* entry)
{
	this->clearItems();

	if (entry == nullptr)
	{
		return;
	}

	PlatformerEntity* entity = entry->getEntity();

	if (entity == nullptr)
	{
		return;
	}

	entity->getComponent<EntityAttackBehavior>([=](EntityAttackBehavior* attackBehavior)
	{
		int index = 0;
		int mana = entity->getRuntimeStateOrDefaultInt(StateKeys::Mana, 0);

		for (PlatformerAttack* attack : attackBehavior->getDefensives())
		{
			int cost = attack->getSpecialCost();
			LocalizedString* costString = (cost <= 0 ? nullptr : Strings::Platformer_Combat_Cost::create()->setStringReplacementVariables(ConstantString::create(std::to_string(cost))));
			LocalizedString* descriptionString = attack->getDescription();

			RadialEntry* newEntry = this->addEntry(attack->getString(), { costString, descriptionString }, attack->getIconResource(), UIResources::Combat_AttackCircle, [=]()
			{
				this->selectAttack(entry, attack, index);
			});

			if (cost > mana)
			{
				newEntry->disableInteraction(127);
				newEntry->toggleAllowInteractionEdits(false);
			}
			
			index++;
		}
	});
}

void DefendMenu::selectAttack(TimelineEntry* entry, PlatformerAttack* attack, int index)
{
	this->scrollTo(index);

	entry->stageCast(attack);

	switch (attack->getTargetingType())
	{
		case PlatformerAttack::TargetingType::Self:
		{
			CombatEvents::TriggerSelectCastTarget(CombatEvents::CastTargetsArgs({ entry->getEntity() }));
			break;
		}
		case PlatformerAttack::TargetingType::Multi:
		{
			switch (attack->getAttackType())
			{
				case PlatformerAttack::AttackType::Buff:
				case PlatformerAttack::AttackType::Healing:
				case PlatformerAttack::AttackType::Resurrection:
				{
					CombatEvents::TriggerSelectCastTarget(CombatEvents::CastTargetsArgs(this->timelineRef->getFriendlyEntities()));
					break;
				}
				default:
				case PlatformerAttack::AttackType::Damage:
				case PlatformerAttack::AttackType::Debuff:
				{
					CombatEvents::TriggerSelectCastTarget(CombatEvents::CastTargetsArgs(this->timelineRef->getEnemyEntities()));
					break;
				}
			}

			break;
		}
		case PlatformerAttack::TargetingType::Team:
		{
			CombatEvents::TriggerSelectCastTarget(CombatEvents::CastTargetsArgs(this->timelineRef->getFriendlyEntities()));
			break;
		}
		case PlatformerAttack::TargetingType::All:
		{
			CombatEvents::TriggerSelectCastTarget(CombatEvents::CastTargetsArgs(this->timelineRef->getEntities()));
			break;
		}
		default:
		case PlatformerAttack::TargetingType::Single:
		{
			switch (attack->getAttackType())
			{
				case PlatformerAttack::AttackType::Healing:
				case PlatformerAttack::AttackType::Buff:
				{
					auto meta = CombatEvents::MenuStateArgs::SelectionMeta(CombatEvents::MenuStateArgs::SelectionMeta::Choice::Attack, attack->getIconResource());
					CombatEvents::TriggerMenuStateChange(CombatEvents::MenuStateArgs(CombatEvents::MenuStateArgs::CurrentMenu::ChooseBuffTarget, entry, meta));
					break;
				}
				case PlatformerAttack::AttackType::Resurrection:
				{
					auto meta = CombatEvents::MenuStateArgs::SelectionMeta(CombatEvents::MenuStateArgs::SelectionMeta::Choice::Attack, attack->getIconResource());
					CombatEvents::TriggerMenuStateChange(CombatEvents::MenuStateArgs(CombatEvents::MenuStateArgs::CurrentMenu::ChooseResurrectionTarget, entry, meta));
					break;
				}
				case PlatformerAttack::AttackType::Damage:
				case PlatformerAttack::AttackType::Debuff:
				{
					auto meta = CombatEvents::MenuStateArgs::SelectionMeta(CombatEvents::MenuStateArgs::SelectionMeta::Choice::Attack, attack->getIconResource());
					CombatEvents::TriggerMenuStateChange(CombatEvents::MenuStateArgs(CombatEvents::MenuStateArgs::CurrentMenu::ChooseAttackTarget, entry, meta));
					break;
				}
				default:
				{
					auto meta = CombatEvents::MenuStateArgs::SelectionMeta(CombatEvents::MenuStateArgs::SelectionMeta::Choice::Attack, attack->getIconResource());
					CombatEvents::TriggerMenuStateChange(CombatEvents::MenuStateArgs(CombatEvents::MenuStateArgs::CurrentMenu::ChooseAnyTarget, entry, meta));
					break;
				}
			}

			break;
		}
	}

	// Hacky fix -- a pause is inserted to handle the case where a defensive was staged, but a UI event unpaused the timeline.
	if (entry != nullptr && entry->getStagedCast() != nullptr && entry->getStagedCast()->getAttackType() == PlatformerAttack::AttackType::Defensive)
	{
		CombatEvents::TriggerPauseTimeline();
	}
}
