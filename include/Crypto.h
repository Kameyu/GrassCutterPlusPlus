#pragma once
#include <cstdint>
#include <string>
#include "cryptopp/rsa.h"

class Crypto final
{
private:
	std::string DISPATCH_KEY;
	std::string DISPATCH_SEED;
	std::string ENCRYPT_KEY;
	std::string ENCRYPT_SEED_BUFFER;
	std::string SESSION_KEY;
	uint64_t ENCRYPT_SEED = {}; // must be 8 bytes
	CryptoPP::RSA::PrivateKey CUR_SIGNING_KEY;
	std::map<const int, CryptoPP::RSA::PublicKey> EncryptionKeys = {};

	friend class Grasscutter; // Befriend class Grasscutter

public:
	Crypto() = default;
	~Crypto() = default;

	// Explicit declaration to prevent unexpected behavior
	Crypto(const Crypto&) = delete;
	Crypto& operator =(Crypto const&) = delete;
	Crypto(Crypto&&) = delete;
	Crypto& operator=(Crypto&&) = delete;

	static void _xor(char* packet, const char* key);
	uint8_t loadKeys();
	char* createSessionKey(int length);

	//Getters
	[[nodiscard]] std::string getDispatchKey() const { return DISPATCH_KEY; }
	[[nodiscard]] std::string getDispatchSeed() const { return DISPATCH_SEED; }
	[[nodiscard]] std::string getEncryptKey() const { return ENCRYPT_KEY; }
	[[nodiscard]] std::string getEncryptSeedBuffer() const { return ENCRYPT_SEED_BUFFER; }
	[[nodiscard]] std::string getSessionKey() const { return SESSION_KEY; }
	[[nodiscard]] int64_t getEncryptSeed() const { return ENCRYPT_SEED; }

	//Setters
	void setDispatchKey(const char*);
	void setDispatchSeed(const char*);
	void setEncryptKey(const char*);
	void setEncryptSeedBuffer(const char*);
	void setSigningKey(const char*); // Handled by CryptoPP
	void setSessionKey(const char*);
	void setEncryptSeed(uint64_t);
};
