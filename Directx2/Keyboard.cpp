#include "Keyboard.h"

std::unordered_map<char, bool> Keyboard::keys;

void Keyboard::Add(const char c)
{
	if(keys.find(c) == keys.end())
		keys.insert(std::pair<char, bool>(c, false));
}

bool Keyboard::Press(const char c)
{
	if (keys.find(c) != keys.end())
	{
		keys[c] = true;

		return true;
	}

	return false;
}

bool Keyboard::Release(const char c)
{
	if (keys.find(c) != keys.end())
	{
		keys[c] = false;
		return true;
	}
	return false;
}

bool Keyboard::IsPressing(const char c)
{
	if (keys.find(c) != keys.end())
		return keys[c];

	return false;
}

bool Keyboard::IsPressing(const std::string s)
{
	for (const char c : s) {

		if (keys.find(c) != keys.end()) {
			if (keys[c])
				return true;
		}
	}

	return false;
}
