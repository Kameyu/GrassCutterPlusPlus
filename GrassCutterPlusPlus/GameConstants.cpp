#include <vector>
#include <string>

#include "GameConstants.h"
#include "Utils.h"

GameConstants::GameConstants()
{
	for (const auto& str : DEFAULT_ABILITY_STRINGS)
		DEFAULT_ABILITY_HASHES.push_back(abilityHash(str));
}