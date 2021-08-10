#include "Tent.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCValue.h"

#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Localization/LocalizedString.h"
#include "Engine/Hackables/HackableCode.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Entities/Platformer/PlatformerFriendly.h"
#include "Events/SwitchEvents.h"
#include "Scenes/Platformer/Components/Entities/Stats/EntityHealthBehavior.h"
#include "Scenes/Platformer/Level/Physics/PlatformerCollisionType.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/FXResources.h"
#include "Resources/ObjectResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

using namespace cocos2d;

const std::string Tent::MapKey = "tent";

Tent* Tent::create(ValueMap& properties)
{
	Tent* instance = new Tent(properties);

	instance->autorelease();

	return instance;
}

Tent::Tent(ValueMap& properties) : super(properties)
{
	this->tentBack = Sprite::create(ObjectResources::Interactive_TentBack);
	this->healAnimation = SmartAnimationSequenceNode::create();
	this->tentFront = Sprite::create(ObjectResources::Interactive_TentFront);
	this->topCollision = CollisionObject::create(this->createTentTopCollision(), (CollisionType)PlatformerCollisionType::Solid, CollisionObject::Properties(false, false));
	this->healCollision = CollisionObject::create(CollisionObject::createBox(CSize(192.0f, 356.0f)), (CollisionType)PlatformerCollisionType::Trigger, CollisionObject::Properties(false, false));
	this->healSound = WorldSound::create(SoundResources::Platformer_Spells_Heal4);
	this->isAnimating = false;
	
	this->addChild(this->healCollision);
	this->addChild(this->topCollision);
	this->addChild(this->tentBack);
	this->addChild(this->healAnimation);
	this->addChild(this->tentFront);
	this->addChild(this->healSound);
}

Tent::~Tent()
{
}

void Tent::onEnter()
{
	super::onEnter();

	this->scheduleUpdate();
}

void Tent::onEnterTransitionDidFinish()
{
	super::onEnterTransitionDidFinish();

	ObjectEvents::TriggerElevateObject(RelocateObjectArgs(this->healAnimation));
	ObjectEvents::TriggerElevateObject(RelocateObjectArgs(this->tentFront));
}

void Tent::initializePositions()
{
	super::initializePositions();

	this->tentBack->setPosition(Vec2(-132.0f, -140.0f - 32.0f));
	this->healCollision->setPosition(Vec2(0.0f, -160.0f - 32.0f));
	this->healAnimation->setPosition(Vec2(0.0f, -160.0f - 32.0f));
	this->topCollision->setPosition(Vec2(-8.0f, 320.0f - 32.0f));
	this->tentFront->setPosition(Vec2(0.0f, -16.0f));
}

void Tent::initializeListeners()
{
	super::initializeListeners();

	this->topCollision->whenCollidesWith({(int)PlatformerCollisionType::Force, (int)PlatformerCollisionType::Player, (int)PlatformerCollisionType::Physics}, [=](CollisionData data)
	{
		return CollisionResult::CollideWithPhysics;
	});

	this->healCollision->whenCollidesWith({ (int)PlatformerCollisionType::Player }, [=](CollisionData data)
	{
		this->runHealAnimation();

		ObjectEvents::QueryObjects<PlatformerFriendly>([=](PlatformerFriendly* entity)
		{
			entity->getComponent<EntityHealthBehavior>([=](EntityHealthBehavior* healthBehavior)
			{
				healthBehavior->setHealth(healthBehavior->getMaxHealth());
			});
		}, PlatformerFriendly::PlatformerFriendlyTag);

		return CollisionResult::DoNothing;
	});

	this->healCollision->whenStopsCollidingWith({ (int)PlatformerCollisionType::Player }, [=](CollisionData data)
	{
		this->isAnimating = false;

		return CollisionResult::DoNothing;
	});
}

void Tent::runHealAnimation(bool reRun)
{
	if (this->isAnimating && !reRun)
	{
		return;
	}

	if (!reRun)
	{
		this->healSound->play();
	}

	this->isAnimating = true;
	
	this->healAnimation->playAnimation(FXResources::Heal_Heal_0000, 0.05f, false, [=]()
	{
		if (this->isAnimating)
		{
			this->runHealAnimation(true);
		}
	});
}

std::vector<Vec2> Tent::createTentTopCollision()
{
	std::vector<Vec2> points = std::vector<Vec2>();

	points.push_back(Vec2(0.0f, 0.0f));
	points.push_back(Vec2(-336.0f, -192.0f));
	points.push_back(Vec2(0.0f, -256.0f));
	points.push_back(Vec2(336.0f, -192.0f));

	return points;
}
