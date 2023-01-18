#pragma once
#include <string>
#include <vector>

class Position
{
private:
	float x{};
	float y{};
	float z{};

public:
	Position() = default;
	~Position() = default;

	Position(const Position&) = delete;
	Position& operator =(Position const&) = delete;
	Position(Position&&) = delete;
	Position& operator=(Position&&) = delete;

	Position(float x, float y);
	Position(float x, float y, float z);

	Position* set(float x, float y);
	Position* set(float x, float y, float z);
	Position* set(const std::vector<float>&);
	Position* set(std::string&, const std::string&); // args: str, delimiter
};
