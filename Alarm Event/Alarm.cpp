/*=========================================
	Copyright (c) 2025 Real Ratty Games
	All rights reserved.
=========================================*/
#include "Alarm.hpp"

void Alarm::Push(strgv name, std::function<void()> fnc)
{
	if (mModules.find(name.data()) != mModules.end()) return;
	AlarmModule rtVl(fnc, -1);
	mModules[name.data()] = rtVl;
}

void Alarm::Tick()
{
	for (auto& it : mModules)
	{
		auto& cnt = it.second.mCnt;
		if (cnt == -1) continue;
		cnt--;
		if (cnt <= 0)
		{
			cnt = -1;
			it.second.mFnc();
		}
	}
}

/// Set alarm to -1 to deactivate it
void Alarm::Set(strgv name, int frames)
{
	if (mModules.find(name.data()) == mModules.end()) return;
	mModules[name.data()].mCnt = frames;
}
