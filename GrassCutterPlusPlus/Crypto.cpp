#include <random>
#include <string>
#include <cstring>
#include <iostream>
#include <filesystem>

#include "Utils.h"
#include "cryptopp/files.h"
#include "Crypto.h"

void Crypto::_xor(char* packet, const char* key)
{
	try
	{
		for (size_t i = 0; i < strlen(packet); i++)
			packet[i] ^= key[i % strlen(key)];
	}
	catch (const std::exception& e)
	{
		std::cerr << "XOR Failed with Error: " << e.what() << std::endl;
	}
}

uint8_t Crypto::loadKeys()
{
	setDispatchKey(getFileContent("./resources/keys/dispatchKey.bin")); // DISPATCH_KEY
	setDispatchSeed(getFileContent("./resources/keys/dispatchSeed.bin")); // DISPATCH_SEED
	setEncryptKey(getFileContent("./resources/keys/secretKey.bin")); // ENCRYPT_KEY
	setEncryptSeedBuffer(getFileContent("./resources/keys/secretKeyBuffer.bin")); // ENCRYPT_SEED_BUFFER

	// CUR_SIGNING_KEY
	this->setSigningKey("./resources/keys/SigningKey.der"); // Not sure this works, but my guess is it will
	// Load each X_Pub.der key
	for (const auto& entry : std::filesystem::directory_iterator("./resources/keys/game_keys/"))
	{
		if (entry.path().extension() == ".der")
		{ // Found our pub_keys
			int idx = std::stoi(entry.path().filename().string()); // X_Pub.der
			try
			{ // Load the RSA keys from .der files
				CryptoPP::RSA::PublicKey pubKey;
				CryptoPP::FileSource key_file(entry.path().c_str(), true);
				pubKey.Load(key_file);
				EncryptionKeys.try_emplace(idx, pubKey);
			}
			catch (const CryptoPP::FileStore::OpenErr& e)
			{
				std::cerr << "Error opening public key file: " << e.what() << std::endl;
			}
		}
	}
	return 0;
} 

char* Crypto::createSessionKey(const int length)
{
	std::random_device rd;
    std::mt19937 gen(rd());

	std::uniform_int_distribution<> distr(-128, 127);

	const auto buf = new char[length];
	for (int i = 0; i < length; i++)
	{
		buf[i] = static_cast<char>(distr(gen)); // Fill array with generated random byte (-128 to 127)
		//std::cout << "Generated char : " << buf[i] << std::endl;
	}
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
	try
	{
		CryptoPP::FileSource file(path, true);
		CUR_SIGNING_KEY.Load(file);
	}
	catch (const CryptoPP::FileStore::OpenErr& e)
	{
		std::cerr << "Error opening private key file: " << e.what() << std::endl;
	}
}

void Crypto::setSessionKey(const char* buf)
{
	this->SESSION_KEY = buf;
}

void Crypto::setEncryptSeed(const uint64_t buf)
{
	this->ENCRYPT_SEED = buf;
}
