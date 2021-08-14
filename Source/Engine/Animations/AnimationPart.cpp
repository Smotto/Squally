#include "AnimationPart.h"

#include <spriter2dx/AnimationNode.h>

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Utils/GameUtils.h"

#include "Resources/UIResources.h"

using namespace cocos2d;

AnimationPart* AnimationPart::create(SpriterEngine::EntityInstance* entity, std::string partName)
{
	AnimationPart* instance = new AnimationPart(entity, partName);

	instance->autorelease();

	return instance;
}

AnimationPart::AnimationPart(SpriterEngine::EntityInstance* entity, std::string partName)
{
	this->entity = entity;
	this->spriterAnimationPart = this->entity == nullptr ? nullptr : this->entity->getObjectInstance(partName);
	this->ghostSprite = this->spriterAnimationPart == nullptr ? nullptr : Sprite::create(this->spriterAnimationPart->getImage() == nullptr ? UIResources::EmptyImage : this->spriterAnimationPart->getImage()->path());
	this->originalPath = "";
	this->lastKnownAnim = "";

	if (this->ghostSprite != nullptr)
	{
		this->ghostSprite->setColor(Color3B::BLUE);
		this->ghostSprite->setVisible(false);
	}

	if (this->spriterAnimationPart != nullptr)
	{
		this->rotation = float(this->spriterAnimationPart->getAngle());

		if (this->spriterAnimationPart->getImage() != nullptr)
		{
			this->originalPath = this->spriterAnimationPart->getImage()->path();
		}
	}

	if (this->ghostSprite != nullptr)
	{
		this->addChild(this->ghostSprite);
	}
}

AnimationPart::~AnimationPart()
{
}

void AnimationPart::onEnter()
{
	super::onEnter();

	this->scheduleUpdate();
}

void AnimationPart::update(float dt)
{
	super::update(dt);

	this->updateTrackedAttributes();
}

void AnimationPart::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
	this->updateTrackedAttributes();

	super::visit(renderer, parentTransform, parentFlags);
}

void AnimationPart::reattachToTimeline()
{
	if (this->spriterAnimationPart == nullptr)
	{
		return;
	}

	this->spriterAnimationPart->toggleTimelineCanUpdate(true);
}

void AnimationPart::detachFromTimeline()
{
	if (this->spriterAnimationPart == nullptr)
	{
		return;
	}

	// Detach the spriter animation part from the timeline such that it is entirely in the user's control
	this->spriterAnimationPart->toggleTimelineCanUpdate(false);
}

void AnimationPart::removeTrackingObject(Node* trackedObject)
{
	if (trackedObject == nullptr)
	{
		return;
	}

	if (std::find(this->trackedObjects.begin(), this->trackedObjects.end(), trackedObject) != this->trackedObjects.end())
	{
		this->removeChild(trackedObject);

		this->trackedObjects.erase(std::remove(this->trackedObjects.begin(), this->trackedObjects.end(), trackedObject), this->trackedObjects.end());
	}
}

void AnimationPart::addTrackingObject(Node* trackedObject)
{
	if (trackedObject == nullptr)
	{
		return;
	}

	this->addChild(trackedObject);

	this->trackedObjects.push_back(trackedObject);
}

void AnimationPart::replaceWithObject(Node* replacement, float disappearDuration, float fadeInDuration)
{
	if (replacement == nullptr || this->spriterAnimationPart == nullptr)
	{
		return;
	}

	this->setOpacity(0);
	replacement->setRotation(this->getRotation());
	replacement->setPosition(GameUtils::getWorldCoords(this));

	this->runAction(Sequence::create(
		DelayTime::create(disappearDuration),
		FadeTo::create(fadeInDuration, 255),
		CallFunc::create([=]()
		{
			// -1 means to use whatever value is on the timeline (no longer overriding)
			this->spriterAnimationPart->setAlphaOverride(-1.0f);
		}),
		nullptr
	));
}

std::string AnimationPart::getSpriteResource()
{
	return this->originalPath;
}

void AnimationPart::replaceSprite(std::string spriteResource)
{
	if (this->spriterAnimationPart == nullptr || this->spriterAnimationPart->getImage() == nullptr)
	{
		return;
	}

	this->spriterAnimationPart->getImage()->setPath(spriteResource);
}

void AnimationPart::restoreSprite()
{
	if (this->spriterAnimationPart == nullptr || this->spriterAnimationPart->getImage() == nullptr)
	{
		return;
	}
	
	this->spriterAnimationPart->getImage()->setPath(this->originalPath);
}

float AnimationPart::getRotationSpriter()
{
	if (this->spriterAnimationPart != nullptr)
	{
		return float(this->spriterAnimationPart->getAngle());
	}

	return 0.0f;
}

void AnimationPart::setRotationSpriter(float rotation)
{
	this->detachFromTimeline();

	if (this->spriterAnimationPart != nullptr)
	{
		this->spriterAnimationPart->setAngle(rotation);
	}
}

void AnimationPart::setRotation(float rotation)
{
	this->detachFromTimeline();

	if (this->spriterAnimationPart != nullptr)
	{
		this->spriterAnimationPart->setAngle(-rotation / 180.0f * M_PI);
	}
}

void AnimationPart::setOffset(Vec2 offset)
{
	if (this->spriterAnimationPart != nullptr)
	{	
		// Flipped x/y for some reason
		this->spriterAnimationPart->setOffset(SpriterEngine::point(offset.y, offset.x));
	}
}

void AnimationPart::restoreOffset()
{
	if (this->spriterAnimationPart != nullptr)
	{	
		this->spriterAnimationPart->setOffset(SpriterEngine::point(0.0f, 0.0f));
	}
}

void AnimationPart::setOpacity(GLubyte opacity)
{
	if (this->spriterAnimationPart != nullptr)
	{
		this->spriterAnimationPart->setAlphaOverride((float)opacity / 255.0f);
	}
}

GLubyte AnimationPart::getOpacity() const
{
	if (this->spriterAnimationPart != nullptr)
	{
		return (GLubyte)(this->spriterAnimationPart->getAlphaOverride() * 255.0f);
	}
	
	return 0;
}

void AnimationPart::setVisible(bool visible)
{
	static const float ClearOverride = -1.0f;

	if (this->spriterAnimationPart != nullptr)
	{
		this->spriterAnimationPart->setAlphaOverride(visible ? 1.0f : ClearOverride);
	}
}

void AnimationPart::updateTrackedAttributes()
{
	if (this->ghostSprite == nullptr)
	{
		return;
	}

	SmartAnimationNode* parent = dynamic_cast<SmartAnimationNode*>(this->getParent());
	this->ghostSprite->setPosition(Vec2(this->ghostSprite->getContentSize().width / 2.0f, this->ghostSprite->getContentSize().height / 2.0f));

	if (parent != nullptr)
	{
		this->ghostSprite->setFlippedX(parent->getFlippedX());

		if (!parent->getFlippedX())
		{
			this->ghostSprite->setPositionX(this->ghostSprite->getPositionX() + this->ghostSprite->getContentSize().width);
		}
	}

	const Vec3 spriteCoords = GameUtils::getWorldCoords3D(this->ghostSprite);
	const Vec3 thisCords = GameUtils::getWorldCoords3D(this);
	const Vec3 delta = thisCords - spriteCoords;

	const float angle = float(this->spriterAnimationPart->getAngle());
	const Vec2 position = Vec2(float(this->spriterAnimationPart->getPosition().x), -float(this->spriterAnimationPart->getPosition().y));
	const Vec2 anchor = Vec2(float(this->spriterAnimationPart->getPivot().x), float(this->spriterAnimationPart->getPivot().y));

	// In order to make the game think that this AnimationPart class is the Spriter object, we need to keep certain things in sync
	super::setRotation(angle * 180.0f / float(M_PI));
	super::setPosition(position - Vec2(delta.x, delta.y));
	super::setAnchorPoint(anchor);

	this->ghostSprite->setPosition(Vec2::ZERO);
}

void AnimationPart::onDeveloperModeEnable(int debugLevel)
{
	super::onDeveloperModeEnable(debugLevel);

	if (this->ghostSprite != nullptr)
	{
		this->ghostSprite->setVisible(true);
	}
}

void AnimationPart::onDeveloperModeDisable()
{
	super::onDeveloperModeDisable();

	if (this->ghostSprite != nullptr)
	{
		this->ghostSprite->setVisible(false);
	}
}
