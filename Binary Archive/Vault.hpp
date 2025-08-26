/*=========================================
	Copyright (c) 2025 Real Ratty Games
	All rights reserved.
=========================================*/
#ifndef VAULT_HPP
#define VAULT_HPP
#include "Types.hpp"
#include <vector>

enum class VaultDataType
{
	RAW = 0,	// Raw file
	TXT = 1,	// Text file
	TEX = 2,	// Texture
	SND = 3,	// Sound
	MDL = 4		// Model
};

struct VaultData
{
	std::vector<uint8>	mBuffer;
	VaultDataType		mType;
	strg				mName;

	VaultData()
	{
		mType = VaultDataType::RAW;
	}

	VaultData(std::vector<uint8>& buffer, VaultDataType type, strgv name)
	{
		mBuffer = buffer;
		mType = type;
		mName = name;
	}
};

struct VaultFile
{
	std::vector<uint8>	mBuffer;
	std::vector<strg>	mFileNames;
	std::vector<uint>	mFileSizes;
	std::vector<uint>	mFileOffsets;
	std::vector<uint8>	mFileTypes;
};

class Vault
{
public:
	void						Open(strgv filepath);
	VaultData					Read(strgv itemname);
	int							Size();
	std::vector<strg>&			FileNames();
	void						Close();

private:
	VaultFile mCurrent;
};
#endif
