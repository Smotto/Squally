#include "LiftBase.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCValue.h"

#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/GameUtils.h"
#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"

using namespace cocos2d;

const std::string LiftBase::PropertySpeed = "speed";

LiftBase::LiftBase(ValueMap& properties, CSize collisionSize, std::string railArt) : super(properties)
{
    this->railingNode = Node::create();
    this->railing = Sprite::create(railArt);
    this->liftNode = Node::create();
    this->liftCollision = CollisionObject::create(
        CollisionObject::createBox(collisionSize),
        (int)PlatformerCollisionType::PassThrough,
        CollisionObject::Properties(false, false)
    );

    this->width = GameUtils::getKeyOrDefault(this->properties, GameObject::MapKeyWidth, Value(0.0f)).asFloat();
    this->height = GameUtils::getKeyOrDefault(this->properties, GameObject::MapKeyHeight, Value(0.0f)).asFloat();
    this->speedPer256px = GameUtils::getKeyOrDefault(this->properties, LiftBase::PropertySpeed, Value(1.0f)).asFloat();
    
    if (this->width >= this->height)
    {
        this->movementDirection = MovementDirection::LeftRight;
    }
    else
    {
        this->movementDirection = MovementDirection::UpDown;
    }

	// Create parameters to repeat the texture
	Texture2D::TexParams params = Texture2D::TexParams();
	params.minFilter = GL_LINEAR;
	params.magFilter = GL_LINEAR;

	switch(this->movementDirection)
	{
		default:
		case MovementDirection::LeftRight:
		{
			params.wrapS = GL_REPEAT;
		    this->railing->setTextureRect(CRect(0.0f, 0.0f, this->width, this->railing->getContentSize().height));
            this->liftCollision->setPosition(Vec2(-this->width / 2.0f, 0.0f));
			break;
		}
		case MovementDirection::UpDown:
		{
			params.wrapT = GL_REPEAT;
		    this->railing->setTextureRect(CRect(0.0f, 0.0f, this->railing->getContentSize().width, this->height));
            this->liftCollision->setPosition(Vec2(-this->height / 2.0f, 0.0f));
			break;
		}
	}
	
	if (this->railing->getTexture() != nullptr)
	{
		this->railing->getTexture()->setTexParameters(params);
	}

    this->liftCollision->addChild(this->liftNode);
    this->railingNode->addChild(this->railing);
    this->addChild(this->railingNode);
    this->addChild(this->liftCollision);
}

LiftBase::~LiftBase()
{
}

void LiftBase::initializePositions()
{
	super::initializePositions();

    const float Padding = this->getPadding();
    const float AdjustedSpeed = (this->movementDirection == MovementDirection::LeftRight ? this->width : this->height) / this->speedPer256px / 256.0f;
    const Vec2 StartPosition = this->movementDirection == MovementDirection::LeftRight ? Vec2(this->width / 2.0f - Padding, 0.0f) : Vec2(0.0f, this->height / 2.0f - Padding);

    this->railing->setPosition(this->getRailsOffset());
	this->liftCollision->setPosition(-StartPosition);
}

void LiftBase::onEnter()
{
    super::onEnter();

    this->setMoving(true);
    this->scheduleUpdate();
}

void LiftBase::update(float dt)
{
    super::update(dt);

    if (!this->isMoving)
    {
        return;
    }

    float adjustedSpeed = (this->isMovingUp ? -1.0f : 1.0f) * this->getAdjustedSpeed();
    Vec2 startPosition = (this->isMovingUp ? -1.0f : 1.0f) * this->getStartPosition();

    this->liftCollision->setPositionY(this->liftCollision->getPositionY() + adjustedSpeed * dt);

    if (this->isMovingUp)
    {
        if (this->liftCollision->getPositionY() < startPosition.y)
        {
            this->isMovingUp = !this->isMovingUp;
        }
    }
    else
    {
        if (this->liftCollision->getPositionY() > startPosition.y)
        {
            this->isMovingUp = !this->isMovingUp;
        }
    }
}

float LiftBase::getAdjustedSpeed()
{
    return this->speedPer256px * 256.0f;
}

Vec2 LiftBase::getStartPosition()
{
    const float Padding = this->getPadding();
    return this->movementDirection == MovementDirection::LeftRight ? Vec2(this->width / 2.0f - Padding, 0.0f) : Vec2(0.0f, this->height / 2.0f - Padding);
}

void LiftBase::setMoving(bool isMoving)
{
	this->isMoving = isMoving;
}

float LiftBase::getPadding()
{
    return 0.0f;
}

Vec2 LiftBase::getRailsOffset()
{
    return Vec2::ZERO;
}
