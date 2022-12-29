#include <string>

#include "Grasscutter.h"
#include "Crypto.h"
#include "Utils.h"

Grasscutter::Grasscutter()
{
	init();
}

inline void Grasscutter::init()
{
	setCrypto(new Crypto); // init crypto module
	cryptoModule->setEncryptSeed(std::stoull("11468049314633205968"));
	cryptoModule->loadKeys(); // Load keys once Crypto module is up
}

void Grasscutter::setCrypto(Crypto* crypto)
{
	this->cryptoModule = crypto;
}
