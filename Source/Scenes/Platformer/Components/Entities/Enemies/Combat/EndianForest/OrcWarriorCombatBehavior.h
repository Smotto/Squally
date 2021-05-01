#pragma once

#include "Scenes/Platformer/Components/Entities/Combat/EntityCombatBehaviorBase.h"

class OrcWarriorCombatBehavior : public EntityCombatBehaviorBase
{
public:
	static OrcWarriorCombatBehavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	OrcWarriorCombatBehavior(GameObject* owner);
	virtual ~OrcWarriorCombatBehavior();

	void initializePositions() override;
	void onLoad() override;

private:
	typedef EntityCombatBehaviorBase super;
};
