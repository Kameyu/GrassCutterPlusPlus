#pragma once
#include "Crypto.h"
#include "GameConstants.h"
#include "json/json.h"

class Grasscutter
{
private:
	Crypto* cryptoModule{};
	GameConstants* constants{};
	Json::Value configFile{};
	Json::Value defaultConfig{};

public:
	Grasscutter();
	~Grasscutter() = default;

	Grasscutter(const Grasscutter&) = delete;
	Grasscutter& operator =(Grasscutter const&) = delete;
	Grasscutter(Grasscutter&&) = delete;
	Grasscutter& operator=(Grasscutter&&) = delete;

	// Methods
	void init();
	void loadConfig();
	void generateDefaultConfig();

	// Getters
	[[nodiscard]] Crypto* getCrypto() const { return this->cryptoModule; }
	[[nodiscard]] GameConstants* getConstants() const { return this->constants; }

	// Setters
	void setCrypto(Crypto*);
	void setConstants(GameConstants*);

	
};
