#include "Timerclass.h"

TimerClass::TimerClass()
{
}

TimerClass::~TimerClass()
{
}

bool TimerClass::Initialize()
{
	m_startTime = std::chrono::steady_clock::now();
	m_frameTime = 0;

	return true;
}

void TimerClass::Frame()
{
	auto curTime = std::chrono::steady_clock::now();
	auto bwTime = curTime - m_startTime;
	std::chrono::milliseconds millSecond = std::chrono::duration_cast<std::chrono::milliseconds>(bwTime);
	m_frameTime = millSecond.count();
	m_startTime = std::chrono::steady_clock::now();
	return;
}

float TimerClass::GetTime()
{
	return m_frameTime;
}
