#pragma once
#include <unordered_map>

class Keyboard
{
private:

	static std::unordered_map<char, bool> keys;

public:
	static void Add(const char c);
	static bool Press(const char c);
	static bool Release(const char c);
	static bool IsPressing(const char c);
	static bool IsPressing(const std::string);
};

