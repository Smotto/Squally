#pragma once

#include "Scenes/Platformer/Components/Entities/Combat/EntityCombatBehaviorBase.h"

class KingGroggCombatBehavior : public EntityCombatBehaviorBase
{
public:
	static KingGroggCombatBehavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	KingGroggCombatBehavior(GameObject* owner);
	virtual ~KingGroggCombatBehavior();

	void initializePositions() override;
	void onLoad() override;

private:
	typedef EntityCombatBehaviorBase super;
};
