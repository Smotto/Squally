#include "KillingMachineDamageBehavior.h"

#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Engine/Events/ObjectEvents.h"
#include "Engine/Events/SaveEvents.h"
#include "Engine/Hackables/Menus/HackablePreview.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Localization/ConcatString.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Optimization/LazyNode.h"
#include "Engine/Save/SaveManager.h"
#include "Engine/Utils/GameUtils.h"
#include "Entities/Platformer/PlatformerEnemy.h"
#include "Entities/Platformer/StatsTables/StatsTables.h"
#include "Scenes/Platformer/Components/Entities/Collision/EntityCollisionBehaviorBase.h"
#include "Scenes/Platformer/Components/Entities/Squally/Movement/SquallyRespawnBehavior.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Level/Huds/Components/StatsBars.h"
#include "Scenes/Platformer/Level/Huds/FadeHuds/FadeHud.h"
#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"
#include "Scenes/Platformer/Save/SaveKeys.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

#define LOCAL_FUNC_ID_COMPARE_TEAM_1 1
#define LOCAL_FUNC_ID_COMPARE_TEAM_2 2
#define LOCAL_FUNC_ID_COMPARE_TEAM_3 3
#define LOCAL_FUNC_ID_COMPARE_TEAM_4 4
#define LOCAL_FUNC_ID_COMPARE_TEAM_5 5
#define LOCAL_FUNC_ID_COMPARE_TEAM_6 6

#define CMOVL_PROBABILITY_CONST 5
#define CMOVLE_PROBABILITY_CONST 3
#define CMOVE_PROBABILITY_CONST 1
#define CMOVNE_PROBABILITY_CONST 1
#define CMOVG_PROBABILITY_CONST 95
#define CMOVGE_PROBABILITY_CONST 97

const std::string KillingMachineDamageBehavior::MapKey = "killing-machine-damage-behavior";
const std::string KillingMachineDamageBehavior::PropertyMachineId = "machine-id";
const std::string KillingMachineDamageBehavior::HackIdentifierKillingMachineCompare1 = "killing-machine-compare-1";
const std::string KillingMachineDamageBehavior::HackIdentifierKillingMachineCompare2 = "killing-machine-compare-2";
const std::string KillingMachineDamageBehavior::HackIdentifierKillingMachineCompare3 = "killing-machine-compare-3";
const std::string KillingMachineDamageBehavior::HackIdentifierKillingMachineCompare4 = "killing-machine-compare-4";
const std::string KillingMachineDamageBehavior::HackIdentifierKillingMachineCompare5 = "killing-machine-compare-5";
const std::string KillingMachineDamageBehavior::HackIdentifierKillingMachineCompare6 = "killing-machine-compare-6";
Value KillingMachineDamageBehavior::DamageStorageAntiOptimize = Value(0);
std::map<int, KillingMachineDamageBehavior::MachineAsmConstants> KillingMachineDamageBehavior::MachineAsmConstantsMap
{
	{ 1, KillingMachineDamageBehavior::MachineAsmConstants("cmovl", HackableCode::Register::zax, CMOVL_PROBABILITY_CONST) },
	{ 2, KillingMachineDamageBehavior::MachineAsmConstants("cmovle", HackableCode::Register::zbx, CMOVLE_PROBABILITY_CONST) },
	{ 3, KillingMachineDamageBehavior::MachineAsmConstants("cmove", HackableCode::Register::zcx, CMOVE_PROBABILITY_CONST) },
	{ 4, KillingMachineDamageBehavior::MachineAsmConstants("cmovne", HackableCode::Register::zdx, CMOVNE_PROBABILITY_CONST) },
	{ 5, KillingMachineDamageBehavior::MachineAsmConstants("cmovg", HackableCode::Register::zdi, CMOVG_PROBABILITY_CONST) },
	{ 6, KillingMachineDamageBehavior::MachineAsmConstants("cmovge", HackableCode::Register::zsi, CMOVGE_PROBABILITY_CONST) },
};
const int KillingMachineDamageBehavior::DefaultDamage = 1;
const int KillingMachineDamageBehavior::CritDamage = 15;

KillingMachineDamageBehavior* KillingMachineDamageBehavior::create(GameObject* owner)
{
	KillingMachineDamageBehavior* instance = new KillingMachineDamageBehavior(owner);

	instance->autorelease();

	return instance;
}

KillingMachineDamageBehavior::KillingMachineDamageBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEnemy*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
}

KillingMachineDamageBehavior::~KillingMachineDamageBehavior()
{
}

void KillingMachineDamageBehavior::onLoad()
{
	super::onLoad();

	if (this->entity == nullptr)
	{
		return;
	}
	
	this->entity->watchForComponent<EntityCollisionBehaviorBase>([=](EntityCollisionBehaviorBase* collisionBehavior)
	{
		if (collisionBehavior->entityCollision == nullptr)
		{
			return;
		}

		collisionBehavior->entityCollision->whenCollidesWith({(int)PlatformerCollisionType::Player }, [=](CollisionData collisionData)
		{
			PlatformerEntity* target = GameUtils::GetFirstParentOfType<PlatformerEntity>(collisionData.other);

			// Even though we force a map reload on a delay, still kill the player to prevent additional inputs
			if (this->entity->getRuntimeStateOrDefault(StateKeys::IsAlive, Value(true)).asBool() && target != nullptr)
			{
				target->getComponent<SquallyRespawnBehavior>([target](SquallyRespawnBehavior* respawnBehavior)
				{
					respawnBehavior->respawnWithMapReload();
				});
			}

			return CollisionResult::DoNothing;
		});
	});
	

	this->machineId = GameUtils::getKeyOrDefault(this->entity->properties, KillingMachineDamageBehavior::PropertyMachineId, Value(1)).asInt();
	int functionId = 0;
	void* functionPtr = nullptr;
	std::string icon = "";
	std::string functionIdentifier = "";
	KillingMachineDamageBehavior::MachineAsmConstants machineAsmConstants;
	LocalizedString* commandComment = nullptr;
	bool defaultAndCritReversed = false;
	bool probabilityReversed = false;
	bool setHintVariables = false;
	LocalizedString* hintStr = nullptr;

	switch(this->machineId)
	{
		default:
		case 1:
		{
			functionId = LOCAL_FUNC_ID_COMPARE_TEAM_1;
			functionIdentifier = KillingMachineDamageBehavior::HackIdentifierKillingMachineCompare1;
			icon = UIResources::Menus_Icons_Tools;
			functionPtr = []() { auto func = &KillingMachineDamageBehavior::compareDamage1; return (void*&)func; }();
			machineAsmConstants = KillingMachineDamageBehavior::MachineAsmConstantsMap[1];
			commandComment = ConcatString::create({
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentCmovl::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentC::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentMov::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentL::create())
			});
			hintStr = Strings::Menus_Hacking_Objects_KillingMachine_CommentHintGeneric::create();
			break;
		}
		case 2:
		{
			functionId = LOCAL_FUNC_ID_COMPARE_TEAM_2;
			functionIdentifier = KillingMachineDamageBehavior::HackIdentifierKillingMachineCompare2;
			icon = UIResources::Menus_Icons_RobotArm;
			functionPtr = []() { auto func = &KillingMachineDamageBehavior::compareDamage2; return (void*&)func; }();
			machineAsmConstants = KillingMachineDamageBehavior::MachineAsmConstantsMap[2];
			commandComment = ConcatString::create({
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentCmovle::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentC::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentMov::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentLe::create())
			});
			hintStr = Strings::Menus_Hacking_Objects_KillingMachine_CommentHintGeneric::create();
			break;
		}
		case 3:
		{
			functionId = LOCAL_FUNC_ID_COMPARE_TEAM_3;
			functionIdentifier = KillingMachineDamageBehavior::HackIdentifierKillingMachineCompare3;
			icon = UIResources::Menus_Icons_Pickaxe;
			functionPtr = []() { auto func = &KillingMachineDamageBehavior::compareDamage3; return (void*&)func; }();
			machineAsmConstants = KillingMachineDamageBehavior::MachineAsmConstantsMap[3];
			commandComment = ConcatString::create({
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentCmove::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentC::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentMov::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentE::create())
			});
			hintStr = Strings::Menus_Hacking_Objects_KillingMachine_CommentHintCmove::create();
			setHintVariables = true;
			break;
		}
		case 4:
		{
			functionId = LOCAL_FUNC_ID_COMPARE_TEAM_4;
			functionIdentifier = KillingMachineDamageBehavior::HackIdentifierKillingMachineCompare4;
			icon = UIResources::Menus_Icons_Hammer;
			functionPtr = []() { auto func = &KillingMachineDamageBehavior::compareDamage4; return (void*&)func; }();
			machineAsmConstants = KillingMachineDamageBehavior::MachineAsmConstantsMap[4];
			commandComment = ConcatString::create({
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentCmovne::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentC::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentMov::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentNe::create())
			});
			hintStr = Strings::Menus_Hacking_Objects_KillingMachine_CommentHintCmovne::create();
			setHintVariables = true;
			defaultAndCritReversed = true;
			break;
		}
		case 5:
		{
			functionId = LOCAL_FUNC_ID_COMPARE_TEAM_5;
			functionIdentifier = KillingMachineDamageBehavior::HackIdentifierKillingMachineCompare5;
			icon = UIResources::Menus_Icons_HammerSmithing;
			functionPtr = []() { auto func = &KillingMachineDamageBehavior::compareDamage5; return (void*&)func; }();
			machineAsmConstants = KillingMachineDamageBehavior::MachineAsmConstantsMap[5];
			commandComment = ConcatString::create({
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentCmovg::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentC::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentMov::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentG::create())
			});
			hintStr = Strings::Menus_Hacking_Objects_KillingMachine_CommentHintGeneric::create();
			probabilityReversed = true;
			break;
		}
		case 6:
		{
			functionId = LOCAL_FUNC_ID_COMPARE_TEAM_6;
			functionIdentifier = KillingMachineDamageBehavior::HackIdentifierKillingMachineCompare6;
			icon = UIResources::Menus_Icons_GearBroken;
			functionPtr = []() { auto func = &KillingMachineDamageBehavior::compareDamage6; return (void*&)func; }();
			machineAsmConstants = KillingMachineDamageBehavior::MachineAsmConstantsMap[6];
			commandComment = ConcatString::create({
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentCmovge::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentC::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentMov::create()),
				COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentGe::create())
			});
			hintStr = Strings::Menus_Hacking_Objects_KillingMachine_CommentHintGeneric::create();
			probabilityReversed = true;
			break;
		}
	}

	LocalizedString* command = ConstantString::create(machineAsmConstants.command);
	const int probabilityConstantPercent = probabilityReversed ? (100 - machineAsmConstants.probabilityConstant) : machineAsmConstants.probabilityConstant;
	LocalizedString* probabilityConstantStr = ConstantString::create(std::to_string(machineAsmConstants.probabilityConstant));
	const std::string probabilityConstantPercentStr = std::to_string(probabilityConstantPercent);
	const std::string probabilityConstantPercentReverseStr = std::to_string(100 - probabilityConstantPercent);

	LocalizedString* defaultDamageStr = Strings::Menus_Hacking_Objects_KillingMachine_RegisterEdi::create();
	LocalizedString* critDamageStr = Strings::Menus_Hacking_Objects_KillingMachine_RegisterEsi::create();
	LocalizedString* critChanceComment = nullptr;
	
	if (setHintVariables)
	{
		if (defaultAndCritReversed)
		{
			hintStr->setStringReplacementVariables({
				ConstantString::create(probabilityConstantPercentReverseStr),
				ConstantString::create(probabilityConstantPercentStr)
			});
		}
		else
		{
			hintStr->setStringReplacementVariables({
				ConstantString::create(probabilityConstantPercentStr),
				ConstantString::create(probabilityConstantPercentReverseStr)
			});
		}
	}

	if (!defaultAndCritReversed)
	{
		LocalizedString* tempStr = defaultDamageStr;
		defaultDamageStr = critDamageStr;
		critDamageStr = tempStr;
		critChanceComment = Strings::Menus_Hacking_Objects_KillingMachine_CommentCritChance::create()->setStringReplacementVariables(
			ConstantString::create(probabilityConstantPercentStr)
		);
	}
	else
	{
		critChanceComment = Strings::Menus_Hacking_Objects_KillingMachine_CommentCritChanceNegate::create()->setStringReplacementVariables({
			ConstantString::create(probabilityConstantPercentReverseStr),
			ConstantString::create(probabilityConstantPercentStr)
		});
	}

	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			functionId,
			HackableCode::HackableCodeInfo(
				functionIdentifier,
				Strings::Menus_Hacking_Objects_KillingMachine_TakeDamage::create(),
				HackableBase::HackBarColor::Purple,
				icon,
				LazyNode<HackablePreview>::create([=](){ return this->createDefaultPreview(); }),
				{
					{
						HackableCode::Register::zax, Strings::Menus_Hacking_Objects_KillingMachine_RegisterEax::create(), HackableDataType::Int32
					},
					{
						HackableCode::Register::zdi, defaultDamageStr, HackableDataType::Int32
					},
					{
						HackableCode::Register::zsi, critDamageStr, HackableDataType::Int32
					},
				},
				int(HackFlags::None),
				15.0f,
				0.0f,
				{
					HackableCode::ReadOnlyScript(
						Strings::Menus_Hacking_CodeEditor_OriginalCode::create(),
						// x86
						ConcatString::create({
							COMMENT(Strings::Menus_Hacking_Objects_KillingMachine_CommentCompare::create()),
							ConstantString::create("cmp eax, "),
							probabilityConstantStr,
							ConstantString::create("\n\n"),
							COMMENT(critChanceComment),
							command,
							ConstantString::create(" edi, esi\n\n"),
							commandComment,
							ConstantString::create("\n"),
							COMMENT(hintStr)
						})
						, // x64
						ConcatString::create({
							COMMENT(Strings::Menus_Hacking_Objects_KillingMachine_CommentCompare::create()),
							ConstantString::create("cmp eax, "), // eax usage intentional
							probabilityConstantStr->clone(),
							ConstantString::create("\n\n"),
							COMMENT(critChanceComment->clone()),
							command->clone(),
							ConstantString::create(" rdi, rsi\n\n"),
							commandComment->clone(),
							ConstantString::create("\n"),
							COMMENT(hintStr->clone())
						})
					),
				},
				true
			)
		},
	};

	std::vector<HackableCode*> hackables = HackableCode::create(functionPtr, codeInfoMap);

	for (HackableCode* next : hackables)
	{
		this->entity->registerCode(next);
	}
}

void KillingMachineDamageBehavior::onDisable()
{
	super::onDisable();
}

HackablePreview* KillingMachineDamageBehavior::createDefaultPreview()
{
	return nullptr;
}

NO_OPTIMIZE int KillingMachineDamageBehavior::compare()
{
	switch(this->machineId)
	{
		default:
		case 1:
		{
			this->compareDamage1();
			break;
		}
		case 2:
		{
			this->compareDamage2();
			break;
		}
		case 3:
		{
			this->compareDamage3();
			break;
		}
		case 4:
		{
			this->compareDamage4();
			break;
		}
		case 5:
		{
			this->compareDamage5();
			break;
		}
		case 6:
		{
			this->compareDamage6();
			break;
		}
	}

	return KillingMachineDamageBehavior::DamageStorageAntiOptimize.asInt();
}
END_NO_OPTIMIZE

// Less than
NO_OPTIMIZE void KillingMachineDamageBehavior::compareDamage1()
{
	static volatile int damage;
	static int probability;

	damage = KillingMachineDamageBehavior::DefaultDamage;
	probability = RandomHelper::random_int(0, 100);

	ASM_PUSH_EFLAGS();
	ASM(push ZAX);
	ASM(push ZDI);
	ASM(push ZSI);
	
	// Create a cmp failure condition by default
	ASM(push ZCX);
	ASM(push ZDX);
	ASM(mov ZCX, 1);
	ASM(mov ZDX, 0);
	ASM(cmp ZCX, ZDX);
	ASM(pop ZDX);
	ASM(pop ZCX);

	// Load probability into register
	ASM_MOV_REG_VAR(ZAX, probability);
	ASM_MOV_REG_VAR(ZDI, KillingMachineDamageBehavior::DefaultDamage);
	ASM_MOV_REG_VAR(ZSI, KillingMachineDamageBehavior::CritDamage);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_COMPARE_TEAM_1);
	ASM(cmp eax, CMOVL_PROBABILITY_CONST);
	ASM(cmovl ZDI, ZSI);
	ASM_NOP8();
	HACKABLE_CODE_END();

	ASM_MOV_VAR_REG(damage, edi);

	ASM(pop ZSI);
	ASM(pop ZDI);
	ASM(pop ZAX);
	ASM_POP_EFLAGS();

	HACKABLES_STOP_SEARCH();

	KillingMachineDamageBehavior::DamageStorageAntiOptimize = Value(damage);
}
END_NO_OPTIMIZE

// Less than or equal
NO_OPTIMIZE void KillingMachineDamageBehavior::compareDamage2()
{
	static volatile int damage;
	static int probability;

	damage = KillingMachineDamageBehavior::DefaultDamage;
	probability = RandomHelper::random_int(0, 100);

	ASM_PUSH_EFLAGS();
	ASM(push ZAX);
	ASM(push ZDI);
	ASM(push ZSI);
	
	// Create a cmp failure condition by default
	ASM(push ZCX);
	ASM(push ZDX);
	ASM(mov ZCX, 1);
	ASM(mov ZDX, 0);
	ASM(cmp ZCX, ZDX);
	ASM(pop ZDX);
	ASM(pop ZCX);

	// Load probability into register
	ASM_MOV_REG_VAR(ZAX, probability);
	ASM_MOV_REG_VAR(ZDI, KillingMachineDamageBehavior::DefaultDamage);
	ASM_MOV_REG_VAR(ZSI, KillingMachineDamageBehavior::CritDamage);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_COMPARE_TEAM_2);
	ASM(cmp eax, CMOVL_PROBABILITY_CONST);
	ASM(cmovl ZDI, ZSI);
	ASM_NOP8();
	HACKABLE_CODE_END();

	ASM_MOV_VAR_REG(damage, edi);

	ASM(pop ZSI);
	ASM(pop ZDI);
	ASM(pop ZAX);
	ASM_POP_EFLAGS();

	HACKABLES_STOP_SEARCH();

	KillingMachineDamageBehavior::DamageStorageAntiOptimize = Value(damage);
}
END_NO_OPTIMIZE

// Equal
NO_OPTIMIZE void KillingMachineDamageBehavior::compareDamage3()
{
	static volatile int damage;
	static int probability;

	damage = KillingMachineDamageBehavior::DefaultDamage;
	probability = RandomHelper::random_int(0, 100);

	ASM_PUSH_EFLAGS();
	ASM(push ZAX);
	ASM(push ZDI);
	ASM(push ZSI);
	
	// Create a cmp failure condition by default
	ASM(push ZCX);
	ASM(push ZDX);
	ASM(mov ZCX, 1);
	ASM(mov ZDX, 0);
	ASM(cmp ZCX, ZDX);
	ASM(pop ZDX);
	ASM(pop ZCX);

	// Load probability into register
	ASM_MOV_REG_VAR(ZAX, probability);
	ASM_MOV_REG_VAR(ZDI, KillingMachineDamageBehavior::DefaultDamage);
	ASM_MOV_REG_VAR(ZSI, KillingMachineDamageBehavior::CritDamage);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_COMPARE_TEAM_3);
	ASM(cmp eax, CMOVE_PROBABILITY_CONST);
	ASM(cmove ZDI, ZSI);
	ASM_NOP8();
	HACKABLE_CODE_END();

	ASM_MOV_VAR_REG(damage, edi);

	ASM(pop ZSI);
	ASM(pop ZDI);
	ASM(pop ZAX);
	ASM_POP_EFLAGS();

	HACKABLES_STOP_SEARCH();

	KillingMachineDamageBehavior::DamageStorageAntiOptimize = Value(damage);
}
END_NO_OPTIMIZE

// Not equal
NO_OPTIMIZE void KillingMachineDamageBehavior::compareDamage4()
{
	static volatile int damage;
	static int probability;

	damage = KillingMachineDamageBehavior::CritDamage;
	probability = RandomHelper::random_int(0, 100);

	ASM_PUSH_EFLAGS();
	ASM(push ZAX);
	ASM(push ZDI);
	ASM(push ZSI);
	
	// Create a cmp failure condition by default
	ASM(push ZCX);
	ASM(push ZDX);
	ASM(mov ZCX, 1);
	ASM(mov ZDX, 0);
	ASM(cmp ZCX, ZDX);
	ASM(pop ZDX);
	ASM(pop ZCX);

	// Load probability into register
	ASM_MOV_REG_VAR(ZAX, probability);

	// Note: reversed from normal.
	ASM_MOV_REG_VAR(ZDI, KillingMachineDamageBehavior::CritDamage);
	ASM_MOV_REG_VAR(ZSI, KillingMachineDamageBehavior::DefaultDamage);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_COMPARE_TEAM_4);
	ASM(cmp eax, CMOVNE_PROBABILITY_CONST);
	ASM(cmovne ZDI, ZSI);
	ASM_NOP8();
	HACKABLE_CODE_END();

	ASM_MOV_VAR_REG(damage, edi);

	ASM(pop ZSI);
	ASM(pop ZDI);
	ASM(pop ZAX);
	ASM_POP_EFLAGS();

	HACKABLES_STOP_SEARCH();

	KillingMachineDamageBehavior::DamageStorageAntiOptimize = Value(damage);
}
END_NO_OPTIMIZE

// Greater than
NO_OPTIMIZE void KillingMachineDamageBehavior::compareDamage5()
{
	static volatile int damage;
	static int probability;

	damage = KillingMachineDamageBehavior::DefaultDamage;
	probability = RandomHelper::random_int(0, 100);

	ASM_PUSH_EFLAGS();
	ASM(push ZAX);
	ASM(push ZDI);
	ASM(push ZSI);
	
	// Create a cmp failure condition by default
	ASM(push ZCX);
	ASM(push ZDX);
	ASM(mov ZCX, 1);
	ASM(mov ZDX, 0);
	ASM(cmp ZCX, ZDX);
	ASM(pop ZDX);
	ASM(pop ZCX);

	// Load probability into register
	ASM_MOV_REG_VAR(ZAX, probability);
	ASM_MOV_REG_VAR(ZDI, KillingMachineDamageBehavior::DefaultDamage);
	ASM_MOV_REG_VAR(ZSI, KillingMachineDamageBehavior::CritDamage);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_COMPARE_TEAM_5);
	ASM(cmp eax, CMOVG_PROBABILITY_CONST);
	ASM(cmovg ZDI, ZSI);
	ASM_NOP8();
	HACKABLE_CODE_END();

	ASM_MOV_VAR_REG(damage, edi);

	ASM(pop ZSI);
	ASM(pop ZDI);
	ASM(pop ZAX);
	ASM_POP_EFLAGS();

	HACKABLES_STOP_SEARCH();

	KillingMachineDamageBehavior::DamageStorageAntiOptimize = Value(damage);
}

END_NO_OPTIMIZE
// Greater than or equal
NO_OPTIMIZE void KillingMachineDamageBehavior::compareDamage6()
{
	static volatile int damage;
	static int probability;

	damage = KillingMachineDamageBehavior::DefaultDamage;
	probability = RandomHelper::random_int(0, 100);

	ASM_PUSH_EFLAGS();
	ASM(push ZAX);
	ASM(push ZDI);
	ASM(push ZSI);
	
	// Create a cmp failure condition by default
	ASM(push ZCX);
	ASM(push ZDX);
	ASM(mov ZCX, 1);
	ASM(mov ZDX, 0);
	ASM(cmp ZCX, ZDX);
	ASM(pop ZDX);
	ASM(pop ZCX);

	// Load probability into register
	ASM_MOV_REG_VAR(ZAX, probability);
	ASM_MOV_REG_VAR(ZDI, KillingMachineDamageBehavior::DefaultDamage);
	ASM_MOV_REG_VAR(ZSI, KillingMachineDamageBehavior::CritDamage);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_COMPARE_TEAM_6);
	ASM(cmp eax, CMOVGE_PROBABILITY_CONST);
	ASM(cmovge ZDI, ZSI);
	ASM_NOP8();
	HACKABLE_CODE_END();

	ASM_MOV_VAR_REG(damage, edi);

	ASM(pop ZSI);
	ASM(pop ZDI);
	ASM(pop ZAX);
	ASM_POP_EFLAGS();

	HACKABLES_STOP_SEARCH();

	KillingMachineDamageBehavior::DamageStorageAntiOptimize = Value(damage);
}
END_NO_OPTIMIZE
