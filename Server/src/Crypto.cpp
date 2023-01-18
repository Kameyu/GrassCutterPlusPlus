#include <random>
#include <string>
#include <cstring>
#include <iostream>
#include <filesystem>

#include "Utils.h"
#include "cryptopp/files.h"
#include "Crypto.h"


#define NOMINMAX // prevents mongocxx conflict with Windows.h
#include <Windows.h> // used by Crypto::createSessionKey()

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
	setDispatchKey(RC_DIR+"keys/dispatchKey.bin"); // DISPATCH_KEY
	setDispatchSeed(RC_DIR+"keys/dispatchSeed.bin"); // DISPATCH_SEED
	setEncryptKey(RC_DIR+"keys/secretKey.bin"); // ENCRYPT_KEY
	setEncryptSeedBuffer(RC_DIR+"keys/secretKeyBuffer.bin"); // ENCRYPT_SEED_BUFFER

	// CUR_SIGNING_KEY
	setSigningKey(RC_DIR+"keys/signingKey.der"); // Not sure this works, but my guess is it will
	// Load each X_Pub.der key
	for (const auto& entry : std::filesystem::directory_iterator(RC_DIR+"keys/game_keys/"))
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

void Crypto::createSessionKey(const int length)
{
	std::random_device rd;
    std::mt19937 gen(rd());

	std::uniform_int_distribution<> distr(-128, 127);
	
	std::vector<char> buf;
	buf.reserve(length); // pre-allocate buffer since we know the size
	for (int i = 0; i < length; i++)
	{
		buf.push_back(static_cast<char>(distr(gen))); // Fill vector with generated random byte (-128 to 127)
		//std::cout << "Generated char : " << buf[i] << std::endl;
	}
	setSessionKey(buf.data()); // Set the freshly created session key
}

void Crypto::setDispatchKey(const std::string& buf)
{
	this->DISPATCH_KEY = buf;
}

void Crypto::setDispatchSeed(const std::string& buf)
{
	this->DISPATCH_SEED = buf;
}

void Crypto::setEncryptKey(const std::string& buf)
{
	this->ENCRYPT_KEY = buf;
}

void Crypto::setEncryptSeedBuffer(const std::string& buf)
{
	this->ENCRYPT_SEED_BUFFER = buf;
}

void Crypto::setSigningKey(const std::string& path)
{
	try
	{
		CryptoPP::FileSource file(path.c_str(), true);
		CUR_SIGNING_KEY.Load(file);
	}
	catch (const CryptoPP::FileStore::OpenErr& e)
	{
		std::cerr << "Error opening private key file: " << e.what() << std::endl;
	}
}

void Crypto::setSessionKey(const std::string& buf)
{
	this->SESSION_KEY = buf;
}

void Crypto::setEncryptSeed(const uint64_t buf)
{
	this->ENCRYPT_SEED = buf;
}