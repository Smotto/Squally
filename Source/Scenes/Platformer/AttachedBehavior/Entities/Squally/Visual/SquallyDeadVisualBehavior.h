#pragma once

#include "Engine/AttachedBehavior/AttachedBehavior.h"

class Squally;

class SquallyDeadVisualBehavior : public AttachedBehavior
{
public:
	static SquallyDeadVisualBehavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	SquallyDeadVisualBehavior(GameObject* owner);
	virtual ~SquallyDeadVisualBehavior();

	void onLoad() override;
	void onDisable() override;

private:
	typedef AttachedBehavior super;

	Squally* squally;
};
