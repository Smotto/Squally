#pragma once

#include "Engine/Hackables/HackableObject.h"

namespace cocos2d
{
	class Sprite;
}

class CollisionObject;
class HackableData;

class PendulumBlade : public HackableObject
{
public:
	static PendulumBlade* create(cocos2d::ValueMap& properties);

	static const std::string HackIdentifierTargetAngle;
	static const std::string MapKey;

protected:
	PendulumBlade(cocos2d::ValueMap& properties);
	virtual ~PendulumBlade();

	void onEnter() override;
	void initializePositions() override;
	void update(float) override;
	cocos2d::Vec2 getButtonOffset() override;
	void registerHackables() override;
	HackablePreview* createDefaultPreview() override;

private:
	typedef HackableObject super;
	friend class BinderPendulumBlade;

	void startSwing();
	void setSwingAngle(float angle);
	void doSwing();
	void buildChain();
	std::vector<cocos2d::Vec2> createBladeCollision();

	volatile float previousAngle = 0.0f;
	volatile float targetAngle = 0.0f;
	cocos2d::Sprite* neck = nullptr;
	cocos2d::Node* bladeChain = nullptr;
	float chainHeight = 0.0f;
	HackableData* hackableDataTargetAngle = nullptr;
	CollisionObject* bladeCollision = nullptr;
	
	static const float DefaultAngle;
	static const float SwingsPerSecondAt480Length;
	static const float MinAngle;
	static const float MaxAngle;
};
