#pragma once

#include "Engine/Components/GameComponent.h"

namespace cocos2d
{
	class Sprite;
}

class ConstantString;
class LocalizedLabel;
class PlatformerEntity;
class ProgressBar;
class WorldSound;

class UnknownValueBarInt32Behavior : public GameComponent
{
public:
	static UnknownValueBarInt32Behavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	UnknownValueBarInt32Behavior(GameObject* owner);
	virtual ~UnknownValueBarInt32Behavior();

	void onLoad() override;
	void onDisable() override;
	void update(float dt) override;

private:
	typedef GameComponent super;

	void addHealth(int newHealth);
	void setHealth(int newHealth);
	void onDeath();

	PlatformerEntity* entity = nullptr;
	cocos2d::Sprite* spellAura = nullptr;

	ProgressBar* healthBar = nullptr;
	ConstantString* deltaString = nullptr;
	LocalizedLabel* deltaLabel = nullptr;
	WorldSound* healSound = nullptr;

	static int Health;
	static const int MaxHealth;
};
