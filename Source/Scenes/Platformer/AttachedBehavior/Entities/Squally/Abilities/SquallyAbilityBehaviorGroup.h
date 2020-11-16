#pragma once

#include "Engine/AttachedBehavior/AttachedBehaviorGroup.h"

class Squally;

class SquallyAbilityBehaviorGroup : public AttachedBehaviorGroup
{
public:
	static SquallyAbilityBehaviorGroup* create(GameObject* owner);

	static const std::string MapKey;

protected:
	SquallyAbilityBehaviorGroup(GameObject* owner);
	virtual ~SquallyAbilityBehaviorGroup();

	void onLoad() override;

private:
	typedef AttachedBehaviorGroup super;
};
