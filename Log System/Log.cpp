/*=========================================
	Copyright (c) 2025 Real Ratty Games
	All rights reserved.
=========================================*/
#include "Log.hpp"
#include <fstream>

Log::~Log()
{
	if (Count(LogMessageType::T_ERROR) != 0)
		Write2Disk("ERRORDUMP.LOG");
	Clear();
}

void Log::Push(strgv msg, LogMessageType type)
{
	mMessages.push_back(LogMessage(msg, type));
}

void Log::Clear()
{
	mMessages.clear();
}

const int Log::Count(LogMessageType type)
{
	if (type == LogMessageType::T_NONE)
		return mMessages.size();

	int rtVl = 0;
	for (auto& it : mMessages)
		if (it.mType == type) rtVl++;
	return rtVl;
}

void Log::Write2Disk(strgv filepath)
{
	strg OUT = "\n--------------------\nLog for App\n--------------------\n\n--------------------\n";

	const uint64 totalMsgCnt = mMessages.size();
	const int infoCnt = Count(LogMessageType::T_INFO);
	const int warnCnt = Count(LogMessageType::T_WARNING);
	const int errCnt = Count(LogMessageType::T_ERROR);

	OUT += "total   messages  : " + std::to_string(totalMsgCnt) + "\n";
	OUT += "info    messages  : " + std::to_string(infoCnt) + "\n";
	OUT += "warning messages  : " + std::to_string(warnCnt) + "\n";
	OUT += "error   messages  : " + std::to_string(errCnt) + "\n";
	OUT += "----------------------------\n\n";

	for (auto& it : mMessages)
	{
		strg _out;
		switch (it.mType)
		{
		case LogMessageType::T_INFO:
			_out = "info     : ";
			break;
		case LogMessageType::T_WARNING:
			_out = "warning  : ";
			break;
		case LogMessageType::T_ERROR:
			_out = "error    : ";
			break;
		default: break;
		}
		_out += it.mText;
		OUT += _out;
		OUT += "\n";
	}

	std::ofstream file(filepath.data());
	file << OUT;
	file.close();
}
