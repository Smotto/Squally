#pragma once
#include <set>

#include "Engine/Particles/SmartParticles.h"
#include "Engine/Hackables/HackableObject.h"

class SmartParticles;

class MagicWall : public HackableObject
{
public:
	static MagicWall* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	MagicWall(cocos2d::ValueMap& properties);
	virtual ~MagicWall();

	void onEnter() override;

private:
	typedef HackableObject super;

	SmartParticles* wallParticles;
	cocos2d::CSize wallSize;
};
