#include "ManaOrb.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Engine/Events/ObjectEvents.h"
#include "Engine/Inventory/CurrencyInventory.h"
#include "Engine/Particles/SmartParticles.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Sound/WorldSound.h"
#include "Entities/Platformer/Squally/Squally.h"
#include "Scenes/Platformer/Components/Entities/Stats/EntityManaBehavior.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ParticleResources.h"
#include "Resources/SoundResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string ManaOrb::MapKey = "mana-orb";

ManaOrb* ManaOrb::create(ValueMap& properties)
{
	ManaOrb* instance = new ManaOrb(properties);

	instance->autorelease();

	return instance;
}

ManaOrb::ManaOrb(ValueMap& properties) : super(properties)
{
	this->collectSound = WorldSound::create(SoundResources::Notifications_NotificationShine1);
	this->manaOrb = SmartParticles::create(ParticleResources::Objects_ManaOrb, SmartParticles::CullInfo(CSize(128.0f, 128.0f)));

	this->collectableNode->addChild(this->manaOrb);
	this->addChild(this->collectSound);
}

ManaOrb::~ManaOrb()
{
}

void ManaOrb::onEnter()
{
	super::onEnter();

	this->collectableCollision->setGravityEnabled(false);
	this->manaOrb->start();
}

void ManaOrb::initializeListeners()
{
	super::initializeListeners();

	this->onCollected([=]()
	{
		ObjectEvents::WatchForObject<Squally>(this, [=](Squally* squally)
		{
			squally->watchForComponent<EntityManaBehavior>([&](EntityManaBehavior* manaBehavior)
			{
				// Restore 30% of mana
				manaBehavior->setMana(manaBehavior->getMana() + int(std::ceil(float(manaBehavior->getMaxMana()) * 0.3f)));
			});
		}, Squally::MapKey);

		this->collectSound->play();
	});
}

void ManaOrb::hideCollectable()
{
	this->runAction(Sequence::create(
		DelayTime::create(0.5f),
		CallFunc::create([=]()
		{
			this->manaOrb->stop(2.0f);
		}),
		nullptr
	));
}
