#pragma once
#include "stdafx.h"

class Timer
{
public:
	chrono::steady_clock::time_point curTime = chrono::high_resolution_clock::now();
	chrono::steady_clock::time_point preTime = curTime;
	chrono::nanoseconds deltaTime;
public:
	Timer() {}
	~Timer() {}

	void Update()
	{
		curTime = chrono::high_resolution_clock::now();
		deltaTime = curTime - preTime;
		preTime = curTime;
	}

	float GetDelteTime() { return static_cast<float>(GetDeltaTimeMilli()) * 1e-03; }
	int GetDeltaTimeMilli() { return chrono::duration_cast<chrono::milliseconds>(deltaTime).count(); }
};