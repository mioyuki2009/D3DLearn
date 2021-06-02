#pragma once
#include <chrono>

class TimerClass
{
public:
	TimerClass();
	~TimerClass();

	bool Initialize();
	void Frame();

	float GetTime();

private:
	std::chrono::time_point<std::chrono::steady_clock> m_startTime;
	int64_t m_frameTime;
};