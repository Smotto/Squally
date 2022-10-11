#pragma once

#include "Objects/Platformer/Shops/ShopPool.h"

class ShopPoolGeryon : public ShopPool
{
public:
	static ShopPoolGeryon* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;
	static const std::string PoolName;

protected:
	ShopPoolGeryon(cocos2d::ValueMap& properties);
	virtual ~ShopPoolGeryon();

private:
	typedef ShopPool super;
};
