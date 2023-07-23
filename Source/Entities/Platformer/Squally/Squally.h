#pragma once

#include "Entities/Platformer/PlatformerFriendly.h"

namespace cocos2d
{
	typedef std::map<std::string, Value> ValueMap;
}

class Squally : public PlatformerFriendly
{
public:
	static Squally* create();
	static Squally* deserialize(cocos2d::ValueMap& properties);
	
	cocos2d::Vec2 getDialogueOffset() override;
	LocalizedString* getEntityName() override;
	std::string getSwimAnimation() override;
	std::string getJumpSound() override;
	std::vector<std::string> getWalkSounds() override;

	static const std::string MapKey;
	static const std::string TeamTag;
	static const float HoverHeight;

protected:
	Squally(cocos2d::ValueMap& properties);
	virtual ~Squally();

	void onEnter() override;
	void onEnterTransitionDidFinish() override;
	void initializePositions() override;
	void initializeListeners() override;
	void update(float dt) override;
	cocos2d::Vec2 getButtonOffset() override;

private:
	typedef PlatformerFriendly super;

	static const float SquallyScale;
};
