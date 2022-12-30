#include <string>

#include "Grasscutter.h"
#include "Crypto.h"

Grasscutter::Grasscutter()
{
	init();
}

inline void Grasscutter::init() const
{
	cryptoModule->setEncryptSeed(std::stoull("11468049314633205968"));
	cryptoModule->loadKeys(); // Load keys once Crypto module is up
}

void Grasscutter::setCrypto(Crypto* crypto)
{
	this->cryptoModule = crypto;
}

void Grasscutter::setConstants(GameConstants* game_constants)
{
	this->constants = game_constants;
}