/*=========================================
	Copyright (c) 2025 Real Ratty Games
	All rights reserved.
=========================================*/
#ifndef LOG_HPP
#define LOG_HPP
#include "Types.hpp"
#include <vector>

enum class LogMessageType
{
	T_NONE,
	T_INFO,
	T_WARNING,
	T_ERROR
};

struct LogMessage
{
	LogMessageType mType;
	strg			mText;

	LogMessage(strgv text, LogMessageType type)
	{
		mText = text;
		mType = type;
	}
};

class Log
{
public:
	~Log();
	void		Push(strgv msg, LogMessageType type = LogMessageType::T_INFO);
	void		Clear();
	const int	Count(LogMessageType type = LogMessageType::T_NONE);
	void		Write2Disk(strgv filepath);

private:
	std::vector<LogMessage> mMessages;
};
#endif
