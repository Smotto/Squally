#pragma once

#include "Objects/Platformer/Shops/ShopPool.h"

class ShopPoolInnT6 : public ShopPool
{
public:
	static ShopPoolInnT6* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;
	static const std::string PoolName;

protected:
	ShopPoolInnT6(cocos2d::ValueMap& properties);
	virtual ~ShopPoolInnT6();

private:
	typedef ShopPool super;
};
