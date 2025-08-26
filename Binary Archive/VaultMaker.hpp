/*=========================================
	Copyright (c) 2025 Real Ratty Games
	All rights reserved.
=========================================*/
#ifndef VAULTMAKER_HPP
#define VAULTMAKER_HPP
#include "Types.hpp"
#include "Vault.hpp"

class VaultMaker
{
public:
	VaultMaker(strgv outloc);
	void Make(strgv filepath);
	void MakeAll(strgv fileroot);
	void MakeUnique(strgv filepath, std::vector<VaultData>& data);

private:
	strg mOutloc;
};
#endif
