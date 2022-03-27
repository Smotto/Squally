#include "EntityTextOverlayBehavior.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Entities/Platformer/PlatformerEnemy.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Entities/Platformer/StatsTables/StatsTables.h"
#include "Events/CombatEvents.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/ParticleResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string EntityTextOverlayBehavior::MapKey = "combat-text-overlays";

EntityTextOverlayBehavior* EntityTextOverlayBehavior::create(GameObject* owner)
{
	EntityTextOverlayBehavior* instance = new EntityTextOverlayBehavior(owner);

	instance->autorelease();

	return instance;
}

EntityTextOverlayBehavior::EntityTextOverlayBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);
	this->contentNode = Node::create();

	if (this->entity == nullptr)
	{
		this->invalidate();
	}

	this->addChild(this->contentNode);
}

EntityTextOverlayBehavior::~EntityTextOverlayBehavior()
{
}

void EntityTextOverlayBehavior::onLoad()
{
	ObjectEvents::TriggerBindObjectToUI(RelocateObjectArgs(this->contentNode));
	
	this->addEventListenerIgnorePause(EventListenerCustom::create(CombatEvents::EventCastInterrupt, [=](EventCustom* eventCustom)
	{
		CombatEvents::CastInterruptArgs* args = static_cast<CombatEvents::CastInterruptArgs*>(eventCustom->getData());

		if (args != nullptr && args->target == this->entity)
		{
			LocalizedLabel* interruptLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::H3, Strings::Platformer_Combat_Interrupted::create());

			interruptLabel->setTextColor(Color4B::ORANGE);
			interruptLabel->setPosition(Vec2(0.0f, 64.0f));

			this->runLabelOverEntity(interruptLabel);
		}
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(CombatEvents::EventCastBlocked, [=](EventCustom* eventCustom)
	{
		CombatEvents::CastBlockedArgs* args = static_cast<CombatEvents::CastBlockedArgs*>(eventCustom->getData());

		if (args != nullptr && args->target == this->entity)
		{
			LocalizedLabel* blockedLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::H3, Strings::Platformer_Combat_Blocked::create());

			blockedLabel->setTextColor(Color4B::ORANGE);
			blockedLabel->setPosition(Vec2(0.0f, 64.0f));

			this->runLabelOverEntity(blockedLabel);
		}
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(CombatEvents::EventDamageDealt, [=](EventCustom* eventCustom)
	{
		CombatEvents::DamageOrHealingArgs* args = static_cast<CombatEvents::DamageOrHealingArgs*>(eventCustom->getData());

		if (args != nullptr && args->target == this->entity)
		{
			// Negate damage (convert into a raw health delta)
			this->runHealthDelta(-args->damageOrHealing, false, args->overflowedMin, args->overflowedMax);
		}
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(CombatEvents::EventHealingDealt, [=](EventCustom* eventCustom)
	{
		CombatEvents::DamageOrHealingArgs* args = static_cast<CombatEvents::DamageOrHealingArgs*>(eventCustom->getData());

		if (args != nullptr && args->target == this->entity)
		{
			this->runHealthDelta(args->damageOrHealing, true, args->overflowedMin, args->overflowedMax);
		}
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(CombatEvents::EventManaRestoreDelt, [=](EventCustom* eventCustom)
	{
		CombatEvents::ManaRestoreOrDrainArgs* args = static_cast<CombatEvents::ManaRestoreOrDrainArgs*>(eventCustom->getData());

		if (args != nullptr && args->target == this->entity)
		{
			this->runManaDelta(args->manaRestoreOrDrain, true, args->overflowedMin, args->overflowedMax);
		}
	}));

	this->addEventListenerIgnorePause(EventListenerCustom::create(CombatEvents::EventManaDrainDelt, [=](EventCustom* eventCustom)
	{
		CombatEvents::ManaRestoreOrDrainArgs* args = static_cast<CombatEvents::ManaRestoreOrDrainArgs*>(eventCustom->getData());

		if (args != nullptr && args->target == this->entity)
		{
			// Negate restore (convert into a raw mana delta)
			this->runManaDelta(-args->manaRestoreOrDrain, false, args->overflowedMin, args->overflowedMax);
		}
	}));
}

void EntityTextOverlayBehavior::onDisable()
{
	super::onDisable();
}

void EntityTextOverlayBehavior::runHealthDelta(int delta, bool zeroAsGreen, bool overflowMin, bool overflowMax)
{
	bool isGreen = (delta > 0 || (delta == 0 && zeroAsGreen));

	ConstantString* amount = ConstantString::create(std::to_string(std::abs(delta)));
	LocalizedString* deltaString = isGreen ? Strings::Common_PlusConstant::create()->setStringReplacementVariables(amount) : Strings::Common_MinusConstant::create()->setStringReplacementVariables(amount);

	if (overflowMin)
	{
		deltaString = Strings::Common_ConcatSpaced::create()->setStringReplacementVariables({ deltaString, Strings::Platformer_Combat_Min::create() });
	}
	else if (overflowMax)
	{
		deltaString = Strings::Common_ConcatSpaced::create()->setStringReplacementVariables({ deltaString, Strings::Platformer_Combat_Max::create() });
	}

	LocalizedLabel* deltaLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::M3, deltaString);
	deltaLabel->setTextColor(isGreen ? Color4B::GREEN : Color4B::RED);

	this->runLabelOverEntity(deltaLabel);
}

void EntityTextOverlayBehavior::runManaDelta(int delta, bool zeroAsBlue, bool overflowMin, bool overflowMax)
{
	bool isBlue = (delta > 0 || (delta == 0 && zeroAsBlue));

	ConstantString* amount = ConstantString::create(std::to_string(std::abs(delta)));
	LocalizedString* deltaString = isBlue ? Strings::Common_PlusConstant::create()->setStringReplacementVariables(amount) : Strings::Common_MinusConstant::create()->setStringReplacementVariables(amount);

	if (overflowMin)
	{
		deltaString = Strings::Common_ConcatSpaced::create()->setStringReplacementVariables({ deltaString, Strings::Platformer_Combat_Min::create() });
	}
	else if (overflowMax)
	{
		deltaString = Strings::Common_ConcatSpaced::create()->setStringReplacementVariables({ deltaString, Strings::Platformer_Combat_Max::create() });
	}

	LocalizedLabel* deltaLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::M3, deltaString);
	deltaLabel->setTextColor(isBlue ? Color4B::BLUE : Color4B::PURPLE);

	this->runLabelOverEntity(deltaLabel);
}

void EntityTextOverlayBehavior::runLabelOverEntity(LocalizedLabel* label)
{
	this->contentNode->addChild(label);

	static const float LabelDuration = 2.0f;

	label->setPosition(label->getPosition() + this->entity->getEntityCenterPoint() + Vec2(0.0f, this->entity->getEntitySize().height / 2.0f + 64.0f));
	label->enableOutline(Color4B::BLACK, 2);

	label->runAction(Sequence::create(
		CallFunc::create([=]()
		{
			label->runAction(FadeTo::create(LabelDuration, 0));
			label->runAction(MoveTo::create(LabelDuration, label->getPosition() + Vec2(0.0f, 128.0f)));
		}),
		DelayTime::create(LabelDuration + 0.5f),
		CallFunc::create([=]()
		{
			this->contentNode->removeChild(label);
		}),
		nullptr
	));
}
