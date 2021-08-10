#pragma once

#include "Engine/Components/GameComponent.h"

class Scrappy;
class PlatformerEntity;
class PlatformerEntityDeserializer;

class ScrappyCombatManagerBehavior : public GameComponent
{
public:
	static ScrappyCombatManagerBehavior* create(GameObject* owner);

	static const std::string MapKey;

protected:
	ScrappyCombatManagerBehavior(GameObject* owner);
	virtual ~ScrappyCombatManagerBehavior();

	void onLoad() override;
	void onDisable() override;

private:
	typedef GameComponent super;

	void spawnScrappy();

	PlatformerEntity* entity;
	Scrappy* helperRef;
	PlatformerEntityDeserializer* platformerEntityDeserializer;
};
