#include "Dart.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCValue.h"

#include "Engine/Hackables/HackableCode.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Scenes/Platformer/Level/Combat/Physics/CombatCollisionType.h"
#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"

#include "Resources/ObjectResources.h"

using namespace cocos2d;

Dart* Dart::create(float dartRotation, float dartSpeed)
{
	Dart* instance = new Dart(dartRotation, dartSpeed);

	instance->autorelease();

	return instance;
}

Dart::Dart(float dartRotation, float dartSpeed) : super(nullptr, CollisionObject::createBox(CSize(64.0f, 16.0f)), (int)PlatformerCollisionType::Damage, false)
{
	this->dartSpeed = dartSpeed;
	this->sprite = Sprite::create(ObjectResources::Traps_DartTripodLauncher_DART);

	this->sprite->setRotation(180.0f);
	this->setProjectileRotation(dartRotation);
	this->setLaunchVelocity(Vec3(this->dartSpeed, 0.0f, 0.0f));

	this->setMovementMode(Projectile::MovementMode::RotationVelocity);

	this->contentNode->addChild(this->sprite);
}

Dart::~Dart()
{
}

void Dart::onEnter()
{
	super::onEnter();
	
	this->sprite->setVisible(false);
}

void Dart::runSpawnFX()
{
	super::runSpawnFX();

	this->sprite->setVisible(true);
}

void Dart::runImpactFX()
{
	super::runImpactFX();

	this->sprite->setVisible(false);
}
