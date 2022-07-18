#include "Vase.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Particles/SmartParticles.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/GameUtils.h"

#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"

#include "Resources/FXResources.h"
#include "Resources/ObjectResources.h"
#include "Resources/ParticleResources.h"
#include "Resources/SoundResources.h"

using namespace cocos2d;

const std::string Vase::MapKey = "vase";

Vase* Vase::create(ValueMap& properties)
{
	Vase* instance = new Vase(properties);

	instance->autorelease();

	return instance;
}

Vase::Vase(ValueMap& properties, int requiredHits) : super(properties, CSize(113.0f, 160.0f), requiredHits)
{
	this->explosion = SmartAnimationSequenceNode::create();
	this->breakSound = WorldSound::create(SoundResources::Platformer_Objects_PotBreak_PotSmash1);
	this->color = GameUtils::getKeyOrDefault(this->properties, GameObject::PropertyColor, Value("yellow")).asString();
	this->shardParticles = SmartParticles::create(ParticleResources::Objects_VaseBreak, SmartParticles::CullInfo(CSize(113.0f, 160.0f)));
	this->vaseBroken = CollisionObject::create(CollisionObject::createBox(CSize(113.0f, 92.0f)), (CollisionType)PlatformerCollisionType::Physics, CollisionObject::Properties(true, true, 0.1f));

	if (this->color == "blue")
	{
		this->vase = Sprite::create(ObjectResources::Breakables_VaseBlue);
		this->vaseBroken->addChild(Sprite::create(ObjectResources::Breakables_VaseBlueBroken));
	}
	else if (this->color == "brown")
	{
		this->vase = Sprite::create(ObjectResources::Breakables_VaseBrown);
		this->vaseBroken->addChild(Sprite::create(ObjectResources::Breakables_VaseBrownBroken));
	}
	else if (this->color == "clay")
	{
		this->vase = Sprite::create(ObjectResources::Breakables_VaseClay);
		this->vaseBroken->addChild(Sprite::create(ObjectResources::Breakables_VaseClayBroken));
	}
	else if (this->color == "gray" || this->color == "grey")
	{
		this->vase = Sprite::create(ObjectResources::Breakables_VaseGray);
		this->vaseBroken->addChild(Sprite::create(ObjectResources::Breakables_VaseGrayBroken));
	}
	else if (this->color == "green")
	{
		this->vase = Sprite::create(ObjectResources::Breakables_VaseGreen);
		this->vaseBroken->addChild(Sprite::create(ObjectResources::Breakables_VaseGreenBroken));
	}
	else if (this->color == "tan")
	{
		this->vase = Sprite::create(ObjectResources::Breakables_VaseTan);
		this->vaseBroken->addChild(Sprite::create(ObjectResources::Breakables_VaseTanBroken));
	}
	else // if (this->color == "yellow")
	{
		this->vase = Sprite::create(ObjectResources::Breakables_VaseYellow);
		this->vaseBroken->addChild(Sprite::create(ObjectResources::Breakables_VaseYellowBroken));
	}

	this->vaseBroken->setPhysicsFlagEnabled(false);
	this->vaseBroken->setVisible(false);

	this->contentNode->addChild(this->vase);
	this->contentNode->addChild(this->vaseBroken);
	this->contentNode->addChild(this->shardParticles);
	this->contentNode->addChild(this->explosion);
	this->contentNode->addChild(this->breakSound);
}

Vase::~Vase()
{
}

void Vase::initializePositions()
{
	super::initializePositions();

	this->vase->setPosition(Vec2(0.0f, 0.0f));
}

void Vase::initializeListeners()
{
	super::initializeListeners();

	this->vaseBroken->whileCollidesWith({ (CollisionType)PlatformerCollisionType::Solid, (CollisionType)PlatformerCollisionType::PassThrough, (CollisionType)PlatformerCollisionType::Physics, (CollisionType)PlatformerCollisionType::Player, (CollisionType)PlatformerCollisionType::PlayerWeapon }, [=](CollisionData collisionData)
	{
		return CollisionResult::CollideWithPhysics;
	});
}

Vec2 Vase::getButtonOffset()
{
	return Vec2(0.0f, 96.0f);
}

void Vase::onBreak()
{
	super::onBreak();
	
	this->explosion->playAnimation(FXResources::ExplosionNormal_Explosion_0000, 0.035f, true);
	this->vase->setVisible(false);
	this->vaseBroken->setPhysicsFlagEnabled(true);
	this->vaseBroken->setVisible(true);
	this->vaseBroken->setVelocity(Vec2(3200.0f, 9600.0f));
	this->shardParticles->start();
	this->breakSound->play();
}
