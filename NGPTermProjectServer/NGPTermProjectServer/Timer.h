#pragma once
#include "stdafx.h"

class Timer
{
public:
	Timer() {}
	~Timer() {}

	void Begin()
	{
		begin = chrono::high_resolution_clock::now();	
	}

	double GetDelteTime() { return GetDeltaTimeMilli() * 1e-03; }
	int GetDeltaTimeMilli() { return (chrono::high_resolution_clock::now() - begin).count(); }

private:
	chrono::steady_clock::time_point begin = chrono::high_resolution_clock::now();
};