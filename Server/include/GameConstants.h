#pragma once

#include <string>

#include "Position.h"
#include "Utils.h"

class GameConstants
{
public:
	GameConstants();
	~GameConstants()=default;

	GameConstants(const GameConstants&) = delete;
	GameConstants& operator =(GameConstants const&) = delete;
	GameConstants(GameConstants&&) = delete;
	GameConstants& operator=(GameConstants&&) = delete;

	std::string VERSION = "3.3.0";

	const uint32_t DEFAULT_TEAMS = 4;
	const uint32_t MAX_TEAMS = 10;
	const uint32_t MAIN_CHARACTER_MALE = 10000005;
	const uint32_t MAIN_CHARACTER_FEMALE = 10000007;

	Position* START_POSITION = new Position(2747.0, 194.0, -1719.0);

	const uint32_t MAX_FRIENDS = 60;
	const uint32_t MAX_FRIEND_REQUESTS = 50;

	const uint32_t SERVER_CONSOLE_UID = 99; // The UID of the server console's "player".

	const uint32_t BATTLE_PASS_MAX_LEVEL = 50;
	const uint32_t BATTLE_PASS_POINT_PER_LEVEL = 1000;
	const uint32_t BATTLE_PASS_POINT_PER_WEEK = 10000;
	const uint32_t BATTLE_PASS_LEVEL_PRICE = 150;
	const uint32_t BATTLE_PASS_CURRENT_INDEX = 2;

	std::vector<std::string> DEFAULT_ABILITY_STRINGS = {
		"Avatar_DefaultAbility_VisionReplaceDieInvincible",
		"Avatar_DefaultAbility_AvartarInShaderChange",
		"Avatar_SprintBS_Invincible",
		"Avatar_Freeze_Duration_Reducer",
		"Avatar_Attack_ReviveEnergy",
		"Avatar_Component_Initializer",
		"Avatar_FallAnthem_Achievement_Listener"
	};

	std::vector<int> DEFAULT_ABILITY_HASHES; // set up in Constructor
	const uint32_t DEFAULT_ABILITY_NAME = abilityHash("Default");
};
