#pragma once

#include "Engine/Components/Component.h"

class Gecky;

class GeckyEqBehavior : public Component
{
public:
	static GeckyEqBehavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	GeckyEqBehavior(GameObject* owner);
	virtual ~GeckyEqBehavior();

	void onLoad() override;
	void onDisable() override;

private:
	typedef Component super;

	Gecky* guano;
};
