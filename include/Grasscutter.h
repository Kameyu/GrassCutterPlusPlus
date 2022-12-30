#pragma once
#include "Crypto.h"
#include "GameConstants.h"

class Grasscutter
{
private:
	Crypto* cryptoModule{};
	GameConstants* constants{};

public:
	Grasscutter();
	~Grasscutter() = default;

	Grasscutter(const Grasscutter&) = delete;
	Grasscutter& operator =(Grasscutter const&) = delete;
	Grasscutter(Grasscutter&&) = delete;
	Grasscutter& operator=(Grasscutter&&) = delete;

	void init();

	// Getters
	[[nodiscard]] Crypto* getCrypto() const { return this->cryptoModule; }
	[[nodiscard]] GameConstants* getConstants() const { return this->constants; }

	// Setters
	void setCrypto(Crypto*);
	void setConstants(GameConstants* game_constants);
};
