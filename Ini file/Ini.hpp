/*=========================================
	Copyright (c) 2025 Real Ratty Games
	All rights reserved.
=========================================*/
#ifndef INI_HPP
#define INI_HPP
#include "Types.hpp"
#include <unordered_map>
#include <vector>

class Ini
{
public:
	using Section = std::unordered_map<strg, strg>;
	using Content = std::unordered_map<strg, Section>;

	bool Open(strgv filename);
	bool OpenMem(std::vector<uint8>* mem);
	strg Read(strgv section, strgv key, strgv value);
	void Write(strgv section, strgv key, strgv value);
	void CloseMem();
	void Close();

private:
	strg mFilepath;
	Content mBuffer;
	std::vector<uint8>* mMemBuffer;

	strg Trim(const strg& s);
	bool ReadString(const strg& content);
	strg WriteString() const;
};
#endif
