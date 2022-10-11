#pragma once
#include "Engine/SmartNode.h"

namespace SpriterEngine
{
	class EntityInstance;
	class UniversalObjectInterface;
}

namespace cocos2d
{
	class Sprite;
}

// This class represents a part of an animation that needs to be controlled by code. Note: This class does NOT actually manage the animation part.
// This class wraps over Spriter and passes information to and from this class to the Spriter object.
// The API for this class is carefully designed, overriding cocos methods, to maintain the illusion that this is the actual animation part.
class AnimationPart : public SmartNode
{
public:
	static AnimationPart* create(SpriterEngine::EntityInstance* entity, std::string partName);

	void removeTrackingObject(cocos2d::Node* trackedObject);
	void addTrackingObject(cocos2d::Node* trackedObject);
	void replaceWithObject(cocos2d::Node* replacement, float disappearDuration = 2.0f, float fadeInDuration = 0.5f);
	void reattachToTimeline();
	void detachFromTimeline();
	std::string getSpriteResource();
	void replaceSprite(std::string spriteResource);
	void restoreSprite();
	void setRotation(float rotation) override;
	float getRotationSpriter();
	void setRotationSpriter(float rotation);
	void setOffset(cocos2d::Vec2 offset);
	void restoreOffset();
	void setOpacity(GLubyte opacity) override;
	void setVisible(bool visible) override;
	void updateTrackedAttributes();

	GLubyte getOpacity() const override;

private:
	typedef SmartNode super;
	AnimationPart(SpriterEngine::EntityInstance* entity, std::string partName);
	virtual ~AnimationPart();

	void onEnter() override;
	void update(float dt) override;
	void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
	void onDeveloperModeEnable(int debugLevel) override;
	void onDeveloperModeDisable() override;

	std::string originalPath;
	std::vector<cocos2d::Node*> trackedObjects;
	cocos2d::Sprite* ghostSprite = nullptr;
	SpriterEngine::UniversalObjectInterface* spriterAnimationPart = nullptr;
	SpriterEngine::EntityInstance* entity = nullptr;
	float rotation = 0.0f;
	std::string lastKnownAnim;
};
