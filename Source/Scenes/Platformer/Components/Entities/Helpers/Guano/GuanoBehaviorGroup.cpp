#include "GuanoBehaviorGroup.h"

#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"

#include "Engine/Maps/GameObject.h"
#include "Scenes/Platformer/Components/Entities/Helpers/HelperBehaviorGroup.h"
#include "Scenes/Platformer/Components/Entities/Helpers/Guano/GuanoEqBehavior.h"
#include "Scenes/Platformer/Components/Entities/Helpers/Guano/GuanoHealthBehavior.h"
#include "Scenes/Platformer/Components/Entities/Helpers/Guano/GuanoManaBehavior.h"
#include "Scenes/Platformer/Components/Entities/Helpers/Guano/GuanoPickPocketBehavior.h"
#include "Scenes/Platformer/Components/Entities/Helpers/HelperFollowMovementBehavior.h"

using namespace cocos2d;

const std::string GuanoBehaviorGroup::MapKey = "guano";

GuanoBehaviorGroup* GuanoBehaviorGroup::create(GameObject* owner)
{
	GuanoBehaviorGroup* instance = new GuanoBehaviorGroup(owner);

	instance->autorelease();

	return instance;
}

GuanoBehaviorGroup::GuanoBehaviorGroup(GameObject* owner) : super(owner, {
	HelperBehaviorGroup::create(owner),
	GuanoEqBehavior::create(owner),
	GuanoHealthBehavior::create(owner),
	GuanoManaBehavior::create(owner),
	GuanoPickPocketBehavior::create(owner),
	HelperFollowMovementBehavior::create(owner),
	})
{
}

GuanoBehaviorGroup::~GuanoBehaviorGroup()
{
}

void GuanoBehaviorGroup::onLoad()
{
}
