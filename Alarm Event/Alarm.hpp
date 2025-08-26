/*=========================================
	Copyright (c) 2025 Real Ratty Games
	All rights reserved.
=========================================*/
#ifndef ALARM_HPP
#define ALARM_HPP
#include "Types.hpp"
#include <functional>
#include <unordered_map>

#define ALARM_BIND(fnc) std::bind(&fnc, this)

struct AlarmModule
{
	std::function<void()>	mFnc;
	int						mCnt = 0;

	AlarmModule() {}
	AlarmModule(std::function<void()> fnc, int cnt)
	{
		mFnc = fnc;
		mCnt = cnt;
	}
};

class Alarm
{
public:
	void Push(strgv name, std::function<void()> fnc);
	void Tick();
	void Set(strgv name, int frames);

private:
	std::unordered_map<strg, AlarmModule> mModules;
};
#endif
