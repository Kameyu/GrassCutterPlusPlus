#pragma once
#include <cstdint>
#include <string>
#include "cryptopp/rsa.h"

class Crypto
{
	std::string DISPATCH_KEY;
	std::string DISPATCH_SEED;
	std::string ENCRYPT_KEY;
	std::string ENCRYPT_SEED_BUFFER;
	std::string SESSION_KEY;
	uint64_t ENCRYPT_SEED = {}; // must be 8 bytes
	CryptoPP::RSA::PrivateKey CUR_SIGNING_KEY;
	std::map<const int, CryptoPP::RSA::PublicKey> EncryptionKeys = {};

public:
	Crypto() = default;
	~Crypto() = default;

	// Explicit declaration to prevent unexpected behavior
	Crypto(const Crypto&) = delete;
	Crypto& operator =(Crypto const&) = delete;
	Crypto(Crypto&&) = delete;
	Crypto& operator=(Crypto&&) = delete;

	static void _xor(char* packet, const char* key);
	void loadKeys(const std::string&);
	void createSessionKey(int length);

	//Getters
	[[nodiscard]] std::string getDispatchKey() const { return DISPATCH_KEY; }
	[[nodiscard]] std::string getDispatchSeed() const { return DISPATCH_SEED; }
	[[nodiscard]] std::string getEncryptKey() const { return ENCRYPT_KEY; }
	[[nodiscard]] std::string getEncryptSeedBuffer() const { return ENCRYPT_SEED_BUFFER; }
	[[nodiscard]] std::string getSessionKey() const { return SESSION_KEY; }
	[[nodiscard]] uint64_t getEncryptSeed() const { return ENCRYPT_SEED; }

	//Setters
	void setDispatchKey(const std::string& buf);
	void setDispatchSeed(const std::string& buf);
	void setEncryptKey(const std::string& buf);
	void setEncryptSeedBuffer(const std::string& buf);
	void setSigningKey(const std::string& path); // Handled by CryptoPP
	void setSessionKey(const std::string& buf);
	void setEncryptSeed(uint64_t);
};
