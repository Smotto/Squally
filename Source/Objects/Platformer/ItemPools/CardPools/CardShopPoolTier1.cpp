#include "CardShopPoolTier1.h"

#include "cocos/base/CCValue.h"

#include "Scenes/Platformer/Inventory/Items/Collectables/HexusCards/HexusCards.h"

using namespace cocos2d;

CardShopPoolTier1* CardShopPoolTier1::create()
{
	CardShopPoolTier1* instance = new CardShopPoolTier1();

	instance->autorelease();

	return instance;
}

CardShopPoolTier1::CardShopPoolTier1() : super(ValueMap(), "card-shop-tool-t1", 2, 3)
{
	this->addItemToPool(Binary0::create(), 1.75f);
	this->addItemToPool(Binary1::create(), 1.25f);
	
	this->addItemToPool(Decimal0::create(), 1.75f);
	this->addItemToPool(Decimal1::create(), 1.25f);
	
	this->addItemToPool(Hex0::create(), 1.75f);
	this->addItemToPool(Hex1::create(), 1.25f);
}

CardShopPoolTier1::~CardShopPoolTier1()
{
}