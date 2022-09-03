#pragma once

#include "Objects/Platformer/Interactables/Mounts/MountBase.h"

class CollisionObject;
class SmartAnimationNode;

enum class LocomotiveSize
{
	Large,
	Medium,
	Small,
};

enum class TrainColor
{
	Red,
	Blue,
	Black,
};

class Train : public MountBase
{
public:
	static Train* create(cocos2d::ValueMap& properties);

	void mount(PlatformerEntity* interactingEntity) override;
	void dismount() override;

	static const std::string MapKey;

protected:
	Train(cocos2d::ValueMap& properties);
	virtual ~Train();

	void onEnter() override;
	void update(float dt) override;
	void initializePositions() override;
	void initializeListeners() override;
	
	cocos2d::Vec2 getReparentPosition() override;

private:
	typedef MountBase super;

	void positionTrainSegments();
	
	enum class CartColor
	{
		Brown,
		Blue
	};

	CollisionObject* bottomCollision = nullptr;
	SmartAnimationNode* locomotive = nullptr;
	std::vector<std::tuple<SmartAnimationNode*, float>> segments;

	bool isIdling = false;
	TrainColor color;
	LocomotiveSize locomotiveSize;

	MountDirection cachedMountDirection = MountDirection::Left;
	
	static const std::string PropertyLocomotiveSize;
	static const std::string PropertyColor;
	static const std::string PropertySegments;
	static const std::string PropertyIsIdling;
};