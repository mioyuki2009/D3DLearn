#pragma once
#include <chrono>

class FpsClass
{
public:
	FpsClass();
	~FpsClass();

	void Initialize();
	void Frame();
	int GetFps();

private:
	int m_fps, m_count;
	std::chrono::time_point<std::chrono::steady_clock> m_startTime;
};
