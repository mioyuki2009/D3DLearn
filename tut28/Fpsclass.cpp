#include "Fpsclass.h"
#include "fpsclass.h"


FpsClass::FpsClass()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = std::chrono::steady_clock::now();
}


FpsClass::~FpsClass()
{
}

void FpsClass::Initialize()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = std::chrono::steady_clock::now();
	return;
}

void FpsClass::Frame()
{
	m_count++;

	auto curTime = std::chrono::steady_clock::now();
	auto bwTime = curTime - m_startTime;
	std::chrono::milliseconds millSecond = std::chrono::duration_cast<std::chrono::milliseconds>(bwTime);
	int64_t realMillTime = millSecond.count();
	if (realMillTime>= 1000)
	{
		m_fps = m_count;
		m_count = 0;

		m_startTime = std::chrono::steady_clock::now();
	}
}

int FpsClass::GetFps()
{
	return m_fps;
}
