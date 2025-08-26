/*=========================================
	Copyright (c) 2025 Real Ratty Games
	All rights reserved.
=========================================*/
#include "Vault.hpp"
#include <fstream>
#include <sstream>

void Vault::Open(strgv filepath)
{
	std::ifstream file(filepath.data(), std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<uint8> buffer(size);
	if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
	{
		file.close();
		return;
	}
	file.close();

	std::string buffer_str(buffer.begin(), buffer.end());
	std::istringstream stream(buffer_str, std::ios::binary);

	uint fileCnt = 0;
	stream.read(reinterpret_cast<char*>(&fileCnt), sizeof(fileCnt));

	mCurrent.mBuffer = buffer;

	for (uint i = 0; i < fileCnt; ++i)
	{
		uint16 nameLength = 0;
		stream.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

		strg fileName(nameLength, '\0');
		stream.read(&fileName[0], nameLength);

		uint fileSize, fileOffset;
		uint8 fileType;
		stream.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
		stream.read(reinterpret_cast<char*>(&fileOffset), sizeof(fileOffset));
		stream.read(reinterpret_cast<char*>(&fileType), sizeof(fileType));

		mCurrent.mFileNames.push_back(fileName);
		mCurrent.mFileSizes.push_back(fileSize);
		mCurrent.mFileOffsets.push_back(fileOffset);
		mCurrent.mFileTypes.push_back(fileType);
	}
	stream.clear();
	buffer.clear();
}

VaultData Vault::Read(strgv itemname)
{
	VaultData rtVl;

	auto it = std::find(mCurrent.mFileNames.begin(), mCurrent.mFileNames.end(), itemname);
	if (it == mCurrent.mFileNames.end())
		return rtVl;

	const int64 itemId = std::distance(mCurrent.mFileNames.begin(), it);

	std::string buffer_str(mCurrent.mBuffer.begin(), mCurrent.mBuffer.end());
	std::istringstream stream(buffer_str, std::ios::binary);

	stream.seekg(static_cast<std::streampos>(mCurrent.mFileOffsets[itemId]), std::ios::beg);

	std::vector<uint8> buffer(mCurrent.mFileSizes[itemId]);
	if (!stream.read(reinterpret_cast<char*>(buffer.data()), buffer.size()))
		return rtVl;

	rtVl.mBuffer = buffer;
	rtVl.mName = itemname;
	rtVl.mType = (VaultDataType)mCurrent.mFileTypes[itemId];
	return rtVl;
}

int Vault::Size()
{
	return mCurrent.mFileNames.size();
}

std::vector<strg>& Vault::FileNames()
{
	return mCurrent.mFileNames;
}

void Vault::Close()
{
	mCurrent.mBuffer.clear();
	mCurrent.mFileNames.clear();
	mCurrent.mFileOffsets.clear();
	mCurrent.mFileSizes.clear();
	mCurrent.mFileTypes.clear();
}
