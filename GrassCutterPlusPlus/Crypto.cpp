#include <string>
#include <cstring>
#include <iostream>

#include <filesystem>

#include "Crypto.h"
#include "Utils.h"
#include "cryptopp/files.h"

using namespace CryptoPP;

void Crypto::_xor(char* packet, const char* key)
{
	try
	{
		for (size_t i = 0; i < strlen(packet); i++)
			packet[i] ^= key[i % strlen(key)];
	}
	catch(int cursor)
	{
		std::cerr << "Crypto xor error @ cursor " << cursor << std::endl;
	}
}

uint8_t Crypto::loadKeys()
{
	this->setDispatchKey(getFileContent("./resources/keys/dispatchKey.bin")); // DISPATCH_KEY
	this->setDispatchSeed(getFileContent("./resources/keys/dispatchSeed.bin")); // DISPATCH_SEED
	this->setEncryptKey(getFileContent("./resources/keys/secretKey.bin")); // ENCRYPT_KEY
	this->setEncryptSeedBuffer(getFileContent("./resources/keys/secretKeyBuffer.bin")); // ENCRYPT_SEED_BUFFER

	// CUR_SIGNING_KEY
	this->setSigningKey("./resources/keys/SigningKey.der"); // Not sure this works, but my guess is it will

	try
	{
		for (const auto& entry : std::filesystem::directory_iterator("./resources/keys/game_keys/"))
		{
			if (entry.path().string().find("_Pub.der") != std::string::npos)
			{ // Found our pub_key
				RSA::PublicKey pubKey;
				int key = std::stoi(entry.path().filename().string()); // 2_Pub.der, 3_Pub.der, etc

				// Load the public key to BT as FS
				pubKey.Load(FileSource(entry.path().string().c_str(), true, nullptr, true).Ref());
				EncryptionKeys.try_emplace(key, pubKey);
				std::cout << " " << entry.path().filename().string() << std::endl;
			}
		}
	}
	catch(errno_t err)
	{
		std::cerr << "Error loading RSA keys, errno: " << errno << err << std::endl;
	}
	return 0;
} 

char* Crypto::createSessionKey(const int length)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	const auto buf = new char[length];
	for (int i = 0; i < length; i++)
		buf[i] = static_cast<char>(rand() % 256 - 128); // Fill array with generated random byte (-128 to 127)
	setSessionKey(buf); // Set the freshly created session key
	return buf;
}

void Crypto::setDispatchKey(const char* buf)
{
	this->DISPATCH_KEY = buf;
}

void Crypto::setDispatchSeed(const char* buf)
{
	this->DISPATCH_SEED = buf;
}

void Crypto::setEncryptKey(const char* buf)
{
	this->ENCRYPT_KEY = buf;
}

void Crypto::setEncryptSeedBuffer(const char* buf)
{
	this->ENCRYPT_SEED_BUFFER = buf;
}

void Crypto::setSigningKey(const char* path)
{
	this->CUR_SIGNING_KEY.Load(FileSource(path, true, nullptr, true).Ref());
}

void Crypto::setSessionKey(const char* buf)
{
	this->SESSION_KEY = buf;
}

void Crypto::setEncryptSeed(const uint64_t buf)
{
	this->ENCRYPT_SEED = buf;
}
