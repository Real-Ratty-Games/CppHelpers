/*=========================================
	Copyright (c) 2025 Real Ratty Games
	All rights reserved.
=========================================*/
#include "VaultMaker.hpp"
#include "Ini.hpp"
#include <filesystem>
#include <fstream>

std::vector<strg> VaultMaker_GetFilesinDir(strgv filepath, bool on = false);
std::vector<uint8> VaultMaker_GetFiletypesinDir(strgv filepath);

VaultMaker::VaultMaker(strgv outloc)
{
	mOutloc = outloc;
}

void VaultMaker::Make(strgv filepath)
{
	strg infoFilepath(filepath);
	infoFilepath += "info.ini";

	if (!THelp::FileExists(infoFilepath))
		return;

	Ini ini;
	ini.Open(infoFilepath);
	strg outname = ini.Read("default", "vaultname", "UNKNOWN");
	outname += "." + ini.Read("default", "extension", "tv");
	ini.Close();

	std::vector<strg> files = VaultMaker_GetFilesinDir(filepath);
	std::vector<strg> filenames = VaultMaker_GetFilesinDir(filepath, true);
	std::vector<uint8> filetypes = VaultMaker_GetFiletypesinDir(filepath);
	std::vector<std::vector<uint8>> data;

	for (auto& it : files)
	{
		if (!THelp::FileExists(it))
			return;

		std::ifstream file(it, std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8> buffer(size);
		if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
			return;

		file.close();
		data.push_back(buffer);
	}

	std::ofstream OUT(mOutloc + outname, std::ios::binary);

	uint fileCnt = filenames.size();
	OUT.write(reinterpret_cast<const char*>(&fileCnt), sizeof(fileCnt));

	uint currentOffset = sizeof(fileCnt);

	for (uint i = 0; i < fileCnt; ++i)
	{
		currentOffset += sizeof(uint16);
		currentOffset += filenames[i].size();
		currentOffset += sizeof(uint);
		currentOffset += sizeof(uint);
		currentOffset += sizeof(uint8);
	}

	for (uint i = 0; i < fileCnt; ++i)
	{
		uint16 nameLength = filenames[i].size();
		uint fileSize = data[i].size();
		uint fileOffset = currentOffset;
		uint8 fileType = filetypes[i];

		OUT.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
		OUT.write(filenames[i].data(), nameLength);
		OUT.write(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize));
		OUT.write(reinterpret_cast<const char*>(&fileOffset), sizeof(fileOffset));
		OUT.write(reinterpret_cast<const char*>(&fileType), sizeof(fileType));

		currentOffset += fileSize;
	}

	for (auto& it : data)
		OUT.write(reinterpret_cast<const char*>(it.data()), it.size());

	OUT.close();
}

void VaultMaker::MakeAll(strgv fileroot)
{
	std::vector<strg> totalpaths;
	for (auto& it : std::filesystem::recursive_directory_iterator(fileroot))
	{
		if (it.is_regular_file() && it.path().filename() == "info.ini")
			totalpaths.push_back(it.path().parent_path().string() + "\\");
	}

	for (auto& it : totalpaths) Make(it);
}

void VaultMaker::MakeUnique(strgv filepath, std::vector<VaultData>& data)
{
	std::ofstream OUT(filepath.data(), std::ios::binary);

	uint fileCnt = data.size();
	OUT.write(reinterpret_cast<const char*>(&fileCnt), sizeof(fileCnt));

	uint currentOffset = sizeof(fileCnt);

	for (uint i = 0; i < fileCnt; ++i)
	{
		currentOffset += sizeof(uint16);
		currentOffset += data[i].mName.size();
		currentOffset += sizeof(uint);
		currentOffset += sizeof(uint);
		currentOffset += sizeof(uint8);
	}

	for (uint i = 0; i < fileCnt; ++i)
	{
		uint16 nameLength = data[i].mName.size();
		uint fileSize = data[i].mBuffer.size();
		uint fileOffset = currentOffset;
		uint8 fileType = (uint8)data[i].mType;

		OUT.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
		OUT.write(data[i].mName.data(), nameLength);
		OUT.write(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize));
		OUT.write(reinterpret_cast<const char*>(&fileOffset), sizeof(fileOffset));
		OUT.write(reinterpret_cast<const char*>(&fileType), sizeof(fileType));

		currentOffset += fileSize;
	}

	for (auto& it : data)
		OUT.write(reinterpret_cast<const char*>(it.mBuffer.data()), it.mBuffer.size());

	OUT.close();
}

std::vector<strg> VaultMaker_GetFilesinDir(strgv filepath, bool on)
{
	if (std::filesystem::exists(filepath))
	{
		std::vector<strg> rtVl;
		for (const auto& entry : std::filesystem::directory_iterator(filepath))
		{
			if (entry.is_regular_file())
			{
				strg pb = entry.path().string();

				if (pb.find("info.ini") != strg::npos) continue;

				if (on) pb = entry.path().stem().string();
				rtVl.push_back(pb);
			}
		}
		return rtVl;
	}
	return {};
}

std::vector<uint8> VaultMaker_GetFiletypesinDir(strgv filepath)
{
	std::vector<strg> image_ext = {
		"png", "PNG", "Png",
		"jpg", "JPG", "Jpg",
		"bmp", "BMP", "Bmp",
		"jpeg", "JPEG", "Jpeg",
		"dds", "DDS", "Dds",
		"gif", "GIF", "Gif",
		"tga", "TGA", "Tga"
	};

	std::vector<strg> sound_ext = {
		"wav", "WAV", "Wav",
		"mp3", "MP3", "Mp3",
		"aif", "AIF", "Aif",
		"mid", "MID", "Mid",
		"flac", "FLAC", "Flac",
		"m4a", "M4A", "M4a",
		"mp4", "MP4", "Mp4",
		"wma", "WMA", "Wma",
		"aac", "AAC", "Aac"
	};

	std::vector<strg> text_ext = {
		"txt", "TXT", "Txt",
		"ini", "INI", "Ini",
		"json", "JSON", "Json",
		"xml", "XML", "Xml",
		"rtf", "RTF", "Rtf",
		"xaml", "XAML", "Xaml"
	};

	std::vector<strg> model_ext = {
		"3ds", "3DS",
		"ac", "AC", "Ac",
		"ase", "ASE", "Ase",
		"b3d", "B3D", "B3d",
		"blend", "BLEND", "Blend",
		"bvh", "BVH", "Bvh",
		"cob", "COB", "Cob",
		"dae", "DAE", "Dae",
		"fbx", "FBX", "Fbx",
		"glb", "GLB", "Glb",
		"gltf", "GLTF", "Gltf",
		"ifc", "IFC", "Ifc",
		"irrmesh", "IRRMESH", "Irrmesh",
		"lwo", "LWO", "Lwo",
		"lws", "LWS", "Lws",
		"lxo", "LXO", "Lxo",
		"md2", "MD2", "Md2",
		"md3", "MD3", "Md3",
		"mdl", "MDL", "Mdl",
		"ms3d", "MS3D", "Ms3d",
		"ndo", "NDO", "Ndo",
		"nff", "NFF", "Nff",
		"obj", "OBJ", "Obj",
		"off", "OFF", "Off",
		"pk3", "PK3", "Pk3",
		"ply", "PLY", "Ply",
		"q3d", "Q3D", "Q3d",
		"scn", "SCN", "Scn",
		"stl", "STL", "Stl",
		"ter", "TER", "Ter",
		"x", "X",
		"xgl", "XGL", "Xgl",
		"zgl", "ZGL", "Zgl",
		"mesh", "MESH", "Mesh"
	};

	if (std::filesystem::exists(filepath))
	{
		std::vector<uint8> rtVl;
		for (const auto& entry : std::filesystem::directory_iterator(filepath))
		{
			if (entry.is_regular_file())
			{
				if (entry.path().string().find("info.ini") != strg::npos) continue;

				strg pb = entry.path().extension().string();
				pb = pb.erase(0, 1);

				uint8 vl = 0;
				if (std::find(text_ext.begin(), text_ext.end(), pb) != text_ext.end()) vl = 1;
				else if (std::find(image_ext.begin(), image_ext.end(), pb) != image_ext.end()) vl = 2;
				else if (std::find(sound_ext.begin(), sound_ext.end(), pb) != sound_ext.end()) vl = 3;
				else if (std::find(model_ext.begin(), model_ext.end(), pb) != model_ext.end()) vl = 4;

				rtVl.push_back(vl);
			}
		}
		return rtVl;
	}
	return {};
}
