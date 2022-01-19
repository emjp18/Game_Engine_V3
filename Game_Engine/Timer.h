#pragma once
#ifndef TIMER_H
#define TIMER_H
#include "common.h"
class Timer
{
private:
	double m_fps;
	double m_deltaTime;
	double m_elapsedDeltaTime = 0.0f;
	double m_lerpedDT;
	int m_countedFrames = 0;
	std::chrono::steady_clock::time_point m_lastUpdate;


public:
	double getFPS() const { return m_fps; }
	void start();
	void reset();
	double getDeltaTime() const { return m_deltaTime; }
	double getfps() const { return m_fps; }
	void run();
	double getElapsedDeltaTime() const { return m_elapsedDeltaTime; }
	double getLerpedDT() { return m_lerpedDT; }
};

#endif