#pragma once

#include "Scenes/Platformer/Level/Combat/Attacks/PlatformerAttack.h"

namespace cocos2d
{
	class Sprite;
};

class WorldSound;

class CastArrowVolley : public PlatformerAttack
{
public:
	static CastArrowVolley* create(float attackDuration, float recoverDuration, Priority priority, std::string arrowResource);

	bool isWorthUsing(PlatformerEntity* caster, const std::vector<PlatformerEntity*>& sameTeam, const std::vector<PlatformerEntity*>& otherTeam) override;
	float getUseUtility(PlatformerEntity* caster, PlatformerEntity* target, const std::vector<PlatformerEntity*>& sameTeam, const std::vector<PlatformerEntity*>& otherTeam) override;
	LocalizedString* getString() override;
	std::string getAttackAnimation() override;

protected:
	CastArrowVolley(float attackDuration, float recoverDuration, Priority priority, std::string arrowResource);
	virtual ~CastArrowVolley();

	void initializePositions() override;

	void performAttack(PlatformerEntity* owner, std::vector<PlatformerEntity*> targets) override;
	void onCleanup() override;

private:
	typedef PlatformerAttack super;
	
	PlatformerAttack* cloneInternal() override;
	
	WorldSound* castSound = nullptr;
	std::string arrowResource;
};
