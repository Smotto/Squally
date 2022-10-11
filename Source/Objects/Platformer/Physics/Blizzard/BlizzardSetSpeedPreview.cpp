#include "BlizzardSetSpeedPreview.h"

#include "cocos/2d/CCActionEase.h"
#include "cocos/2d/CCSprite.h"

#include "Engine/Particles/SmartParticles.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Localization/LocalizedLabel.h"

#include "Resources/ParticleResources.h"

using namespace cocos2d;

BlizzardSetSpeedPreview* BlizzardSetSpeedPreview::create()
{
	BlizzardSetSpeedPreview* instance = new BlizzardSetSpeedPreview();

	instance->autorelease();

	return instance;
}

BlizzardSetSpeedPreview::BlizzardSetSpeedPreview()
{
	this->blizzardParticles = SmartParticles::create(ParticleResources::Gust);

	this->blizzardParticles->setGrouped();
	this->blizzardParticles->setScale(0.4f);

	this->blizzardParticles->setAnchorPoint(Vec2::ZERO);
	this->blizzardParticles->setPosVar(Vec2(HackablePreview::PreviewRadius, HackablePreview::PreviewRadius));

	this->eaxNegative = this->createRegisterEqualsValueLabel(HackableCode::Register::zax, true, ConstantString::create("-1.0f"));
	this->eaxPositive = this->createRegisterEqualsValueLabel(HackableCode::Register::zax, true, ConstantString::create("1.0f"));
	this->ebxNegative = this->createRegisterEqualsValueLabel(HackableCode::Register::zbx, true, ConstantString::create("-1.0f"));
	this->ebxPositive = this->createRegisterEqualsValueLabel(HackableCode::Register::zbx, true, ConstantString::create("1.0f"));

	this->previewNode->addChild(this->blizzardParticles);
	this->assemblyTextNode->addChild(this->eaxNegative);
	this->assemblyTextNode->addChild(this->eaxPositive);
	this->assemblyTextNode->addChild(this->ebxNegative);
	this->assemblyTextNode->addChild(this->ebxPositive);
}

BlizzardSetSpeedPreview::~BlizzardSetSpeedPreview()
{
}

HackablePreview* BlizzardSetSpeedPreview::clone()
{
	return BlizzardSetSpeedPreview::create();
}

void BlizzardSetSpeedPreview::onEnter()
{
	super::onEnter();
	
	this->blizzardParticles->start();
}

void BlizzardSetSpeedPreview::initializePositions()
{
	super::initializePositions();

	const float offset = 24.0f;

	this->eaxNegative->setPosition(Vec2(-(HackablePreview::PreviewRadius - offset), 0.0f));
	this->eaxPositive->setPosition(Vec2(HackablePreview::PreviewRadius - offset, 0.0f));
	this->ebxNegative->setPosition(Vec2(0.0f, -(HackablePreview::PreviewRadius - offset)));
	this->ebxPositive->setPosition(Vec2(0.0f, HackablePreview::PreviewRadius - offset));
}
