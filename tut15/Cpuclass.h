#pragma once
#include <pdh.h>
#include <chrono>
class CpuClass
{
public:
	CpuClass();
	~CpuClass();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCpuPercentage();

private:
	bool m_canReadCpu;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	std::chrono::time_point<std::chrono::steady_clock> m_lastSampleTime;
	long m_cpuUsage;
};