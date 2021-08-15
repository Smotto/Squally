#pragma once

#include "Engine/Hackables/HackableObject.h"

class CollisionObject;
class SmartAnimationNode;

class Ram : public HackableObject
{
public:
	static Ram* create(cocos2d::ValueMap& properties);

	SmartAnimationNode* getAnimations();
	CollisionObject* getCollision();

	static const std::string MapKey;

protected:
	Ram(cocos2d::ValueMap& properties);
	virtual ~Ram();

	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;

private:
	typedef HackableObject super;

	SmartAnimationNode* ramAnimations = nullptr;
	CollisionObject* collision = nullptr;

	static const float AnimOffset;
};
