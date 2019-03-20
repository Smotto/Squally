//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// THIS C++ FILE IS GENERATED. ONLY EDIT NON-GENERATED SECTIONS. RUN GenerateDataFiles.py TO GENERATE THIS FILE //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Snowman.h"

#include "cocos/math/CCGeometry.h"

#include "Scenes/Hexus/Card.h"
#include "Scenes/Hexus/CardData/CardData.h"
#include "Scenes/Hexus/CardData/CardKeys.h"
#include "Scenes/Hexus/CardData/CardList.h"
#include "Scenes/Hexus/Opponents/HexusOpponentData.h"

#include "Resources/EntityResources.h"
#include "Resources/UIResources.h"

///////////////////////////////////////////////////
// BEGIN: CODE NOT AFFECTED BY GENERATE SCRIPTS: //
////A////A////A////A////A////A////A////A////A////A/

////B////B////B////B////B////B////B////B////B////B/
// END: CODE NOT AFFECTED BY GENERATE SCRIPTS    //
///////////////////////////////////////////////////

using namespace cocos2d;

const std::string Snowman::MapKeySnowman = "snowman";
HexusOpponentData* Snowman::HexusOpponentDataInstance = nullptr;
const std::string Snowman::HexusSaveKey = "HEXUS_OPPONENT_SAVE_KEY_SNOWMAN";

Snowman* Snowman::deserialize(ValueMap& initProperties)
{
	Snowman* instance = new Snowman(initProperties);

	instance->autorelease();

	return instance;
}

Snowman::Snowman(ValueMap& initProperties) : PlatformerEntity(initProperties,
	EntityResources::Helpers_BalmerPeaks_Snowman_Animations,
	EntityResources::Helpers_BalmerPeaks_Snowman_Emblem,
	PlatformerCollisionType::FriendlyNpc,
	Size(224.0f, 440.0f),
	0.3f,
	Vec2(0.0f, 0.0f),
	10,
	10)
{
	this->hexusOpponentData = Snowman::getHexusOpponentData();

	///////////////////////////////////////////////////
	// BEGIN: CODE NOT AFFECTED BY GENERATE SCRIPTS: //
	////Y////Y////Y////Y////Y////Y////Y////Y////Y////Y/

	////Z////Z////Z////Z////Z////Z////Z////Z////Z////Z/
	// END: CODE NOT AFFECTED BY GENERATE SCRIPTS    //
	///////////////////////////////////////////////////
}

Snowman::~Snowman()
{
}

///////////////////////////////////////////////////
// BEGIN: CODE NOT AFFECTED BY GENERATE SCRIPTS: //
////X////X////X////X////X////X////X////X////X////X/

////O////O////O////O////O////O////O////O////O////O/
// END: CODE NOT AFFECTED BY GENERATE SCRIPTS    //
///////////////////////////////////////////////////

Vec2 Snowman::getAvatarFrameOffset()
{
	return Vec2(-32.0f, -112.0f);
}

HexusOpponentData* Snowman::getHexusOpponentData()
{
	if (Snowman::HexusOpponentDataInstance == nullptr)
	{
		Snowman::HexusOpponentDataInstance = new HexusOpponentData(
			EntityResources::Helpers_BalmerPeaks_Snowman_Animations,
			UIResources::Menus_Hexus_HexusFrameBalmerPeaks,
			0.3f,
			Vec2(0.0f, 0.0f),
			Vec2(-48.0f, -144.0f),
			Vec2(-32.0f, -112.0f),
			Snowman::HexusSaveKey,
			HexusOpponentData::Strategy::Random,
			Card::CardStyle::Water,
			1.000f,
			HexusOpponentData::generateDeck(25, 1.000f,
			{

			}),
			nullptr
		);
	}

	return Snowman::HexusOpponentDataInstance;
}