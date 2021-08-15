#pragma once

#include "Objects/Platformer/Interactables/InteractObject.h"

class SmartAnimationNode;
class WorldSound;

class Lever : public InteractObject
{
public:
	static Lever* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	Lever(cocos2d::ValueMap& properties);
	virtual ~Lever();

	void initializePositions() override;
	void initializeListeners() override;
	void onInteract(PlatformerEntity* interactingEntity) override;

private:
	typedef InteractObject super;

	bool canPull = true;
	SmartAnimationNode* lever = nullptr;
	WorldSound* leverSound = nullptr;
};
