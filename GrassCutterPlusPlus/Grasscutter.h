#pragma once
#include "Crypto.h"

class Grasscutter
{
private:
	Crypto* cryptoModule ={};

public:
	Grasscutter();
	~Grasscutter() = default;

	Grasscutter(const Grasscutter&) = delete;
	Grasscutter& operator =(Grasscutter const&) = delete;
	Grasscutter(Grasscutter&&) = delete;
	Grasscutter& operator=(Grasscutter&&) = delete;

	// Getters
	[[nodiscard]] Crypto* getCrypto() const { return this->cryptoModule; }

	// Setters
	void setCrypto(Crypto*);

	void init();
};
