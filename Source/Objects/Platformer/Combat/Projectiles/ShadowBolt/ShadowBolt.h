#pragma once

#include "Objects/Platformer/Combat/Projectiles/ThrownObject/ThrownObject.h"

class ThrownObject;
class PlatformerAttack;
class PlatformerEntity;
class SmartAnimationSequenceNode;
class WorldSound;

class ShadowBolt : public ThrownObject
{
public:
	static ShadowBolt* create(PlatformerEntity* owner, PlatformerEntity* target);
	
	void runSpawnFX() override;
	void runImpactFX() override;

protected:
	ShadowBolt(PlatformerEntity* owner, PlatformerEntity* target);
	virtual ~ShadowBolt();

	void update(float dt) override;
	cocos2d::Vec2 getButtonOffset() override;
	void registerHackables() override;
	HackablePreview* createDefaultPreview() override;

private:
	typedef ThrownObject super;

	void setShadowBoltSpeed();

	SmartAnimationSequenceNode* shadowBoltAnim = nullptr;
	SmartAnimationSequenceNode* explosionAnim = nullptr;
	WorldSound* launchSound = nullptr;
	WorldSound* impactSound = nullptr;
};
