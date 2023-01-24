#pragma once
#include "Crypto.h"
#include "GameConstants.h"
#include "json/json.h"

class Grasscutter
{
//private: (implicit)
	Crypto* cryptoModule{};
	GameConstants* constants{};
	Json::Value configFile{};
	Json::Value defaultConfig{};

	std::string lang{};
	Json::Value languageFile{};

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
	bool integrityCheckup();
	void loadLanguage(std::string langCode);
	
	// Getters
	[[nodiscard]] Crypto* getCrypto() const { return cryptoModule; }
	[[nodiscard]] GameConstants* getConstants() const { return constants; }
	[[nodiscard]] const Json::Value* getConfig() const { return &configFile; }
	[[nodiscard]] const Json::Value* getDefaultConfig() const { return &defaultConfig; }

	// Setters
	void setCrypto(Crypto*);
	void setConstants(GameConstants*);
	void setConfig(const Json::Value& cnf);
};
