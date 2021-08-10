#include "CameraControl.h"

#include "cocos/2d/CCDrawNode.h"
#include "cocos/base/CCDirector.h"
#include "cocos/base/CCValue.h"

#include "Engine/Camera/CameraTrackingData.h"
#include "Engine/Camera/GameCamera.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/GameUtils.h"
#include "Objects/Platformer/Camera/CameraTarget.h"
#include "Scenes/Platformer/Level/Physics/PlatformerCollisionType.h"

using namespace cocos2d;

const std::string CameraControl::MapKey = "camera-control";
const std::string CameraControl::PropertyTarget = "target";

CameraControl* CameraControl::create(ValueMap& properties)
{
	CameraControl* instance = new CameraControl(properties);

	instance->autorelease();

	return instance;
}

CameraControl::CameraControl(ValueMap& properties) : super(properties)
{
	CSize controlSize = CSize(this->properties.at(GameObject::MapKeyWidth).asFloat(), this->properties.at(GameObject::MapKeyHeight).asFloat());
	this->targetTag = GameUtils::getKeyOrDefault(this->properties, CameraControl::PropertyTarget, Value("")).asString();
	this->controlCollision = CollisionObject::create(CollisionObject::createBox(controlSize), (CollisionType)PlatformerCollisionType::Trigger, CollisionObject::Properties(false, false));
	this->cameraTarget = nullptr;

	this->addChild(this->controlCollision);
}

CameraControl::~CameraControl()
{
}

void CameraControl::initializeListeners()
{
	super::initializeListeners();

	ObjectEvents::WatchForObject<CameraTarget>(this, [=](CameraTarget* cameraTarget)
	{
		this->cameraTarget = cameraTarget;
	}, this->targetTag);

	this->controlCollision->whenCollidesWith({ (CollisionType)PlatformerCollisionType::Player }, [=](CollisionData collisionData)
	{
		this->beginTrack();

		return CollisionResult::DoNothing;
	});

	this->controlCollision->whenStopsCollidingWith({ (CollisionType)PlatformerCollisionType::Player }, [=](CollisionData collisionData)
	{
		this->endTrack();

		return CollisionResult::DoNothing;
	});
}

void CameraControl::beginTrack()
{
	if (this->cameraTarget == nullptr)
	{
		return;
	}

	CameraTrackingData trackingData = this->cameraTarget->getTrackingData();

	trackingData.id = this->getUniqueIdentifier();

	GameCamera::getInstance()->pushTarget(trackingData);
}

void CameraControl::endTrack()
{
	if (this->cameraTarget != nullptr)
	{
		GameCamera::getInstance()->removeTarget(this->getUniqueIdentifier());
	}
}
