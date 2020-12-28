#include "FriendlyExpBarBehavior.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"

#include "Engine/Localization/ConstantString.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/Particles/SmartParticles.h"
#include "Engine/Sound/Sound.h"
#include "Engine/UI/Controls/ProgressBar.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/AttachedBehavior/Entities/Stats/EntityEqBehavior.h"

#include "Resources/ParticleResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string FriendlyExpBarBehavior::MapKey = "friendly-exp-bars";

FriendlyExpBarBehavior* FriendlyExpBarBehavior::create(GameObject* owner)
{
	FriendlyExpBarBehavior* instance = new FriendlyExpBarBehavior(owner);

	instance->autorelease();

	return instance;
}

FriendlyExpBarBehavior::FriendlyExpBarBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);
	this->deltaString = Strings::Common_PlusConstant::create();
	this->deltaLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::H2, deltaString);
	this->levelUpLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::H2, Strings::Platformer_Combat_LevelUp::create());
	this->expProgressBar = ProgressBar::create(Sprite::create(UIResources::HUD_StatFrame), Sprite::create(UIResources::HUD_FillYellow));
	this->levelUpFx = SmartParticles::create(ParticleResources::Platformer_Combat_LevelUp);
	this->levelUpSound = Sound::create(SoundResources::Platformer_Combat_LevelUp2);
	this->tickCounterA = 0;
	this->tickCounterB = 0;

	this->deltaLabel->setTextColor(Color4B::YELLOW);
	this->deltaLabel->enableOutline(Color4B::BLACK, 2);
	this->levelUpLabel->setTextColor(Color4B::YELLOW);
	this->levelUpLabel->enableOutline(Color4B::BLACK, 2);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}

	this->addChild(this->levelUpFx);
	this->addChild(this->expProgressBar);
	this->addChild(this->deltaLabel);
	this->addChild(this->levelUpLabel);
	this->addChild(this->levelUpSound);
}

FriendlyExpBarBehavior::~FriendlyExpBarBehavior()
{
}

void FriendlyExpBarBehavior::onLoad()
{
	this->expProgressBar->setOpacity(0);
	this->deltaLabel->setOpacity(0);
	this->levelUpLabel->setOpacity(0);

	const Vec2 entityCenter = this->entity->getEntityCenterPoint();
	const float offetY =  this->entity->getEntitySize().height / 2.0f + 32.0f;

	this->deltaLabel->setPosition(entityCenter + Vec2(0.0f, offetY + 48.0f));
	this->levelUpLabel->setPosition(entityCenter + Vec2(0.0f, offetY + 48.0f));
	this->expProgressBar->setPosition(entityCenter + Vec2(0.0f, offetY));
	this->levelUpFx->setPosition(entityCenter + Vec2(0.0f, offetY - 16.0f));
}

void FriendlyExpBarBehavior::onDisable()
{
	super::onDisable();
}

void FriendlyExpBarBehavior::giveExp(float startProgress, float endProgress, bool didLevelUp, int expGain)
{
	this->expProgressBar->runAction(Sequence::create(
		FadeTo::create(0.25f, 255),
		DelayTime::create(5.0f),
		FadeTo::create(0.25f, 0),
		nullptr
	));

	this->deltaLabel->runAction(Sequence::create(
		FadeTo::create(0.25f, 255),
		DelayTime::create(1.0f),
		FadeTo::create(0.25f, 0),
		CallFunc::create([=]()
		{
			if (didLevelUp)
			{
				this->levelUpLabel->runAction(Sequence::create(
					FadeTo::create(0.25f, 255),
					DelayTime::create(2.0f),
					FadeTo::create(0.25f, 0),
					nullptr
				));
			}
		}),
		nullptr
	));
	this->deltaString->setStringReplacementVariables(ConstantString::create(std::to_string(expGain)));
	
	this->entity->getAttachedBehavior<EntityEqBehavior>([=](EntityEqBehavior* eqBehavior)
	{
		const float StartDelay = 0.5f;
		const float TimePerPercent = 0.85f;
		const float RestartDelay = 0.0f;
		const float Phase1Duration = (1.0f - startProgress) * TimePerPercent;
		const float Phase2Duration = endProgress * TimePerPercent;
		
		if (didLevelUp)
		{
			this->fillBar(startProgress, 1.0f, Phase1Duration, StartDelay, &this->tickCounterA, [=]()
			{
				this->runLevelUpEffect();
				this->fillBar(0.0f, endProgress, Phase2Duration, RestartDelay, &this->tickCounterB);
			});
		}
		else
		{
			this->fillBar(startProgress, endProgress, Phase1Duration, StartDelay, &this->tickCounterA);
		}
	});
}

void FriendlyExpBarBehavior::fillBar(float startProgress, float endProgress, float fillDuration, float startDelay, int* tickCounter, std::function<void()> onComplete)
{
	static int UniqueId = 0;
	UniqueId++;
	
	const int UpdatesPerSecond = 30;
	const float Interval = fillDuration / float(UpdatesPerSecond);
	const int Ticks = int(fillDuration * float(UpdatesPerSecond));
	const float Increment = (endProgress - startProgress) / float(Ticks);
	*tickCounter = 0;

	expProgressBar->setProgress(startProgress);

	this->runAction(Sequence::create(
		DelayTime::create(startDelay),
		CallFunc::create([=]()
		{
			expProgressBar->schedule([=](float dt)
			{
				if (onComplete != nullptr && *tickCounter == Ticks)
				{
					onComplete();
				}
				else if (*tickCounter < Ticks)
				{
					expProgressBar->setProgress(expProgressBar->getProgress() + Increment);
				}

				++*tickCounter;

			}, "EVENT_EXP_BAR_UPDATE_" + std::to_string(UniqueId), Interval, Ticks);
		}),
		nullptr
	));
}

void FriendlyExpBarBehavior::runLevelUpEffect()
{
	this->levelUpSound->play();
	this->levelUpFx->start();
}
