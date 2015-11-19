#include "Timer.h"

__int64 Timer::m_prevFrame = 0;
__int64 Timer::m_currentFrame = 0;
__int64 Timer::m_freq = 0;
float Timer::m_deltaTime = 0;

void Timer::Update()
{
	m_deltaTime = 0.0f;
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currentFrame);
	int deltaTicks = (int)(m_currentFrame - m_prevFrame);
	m_deltaTime = ((float)deltaTicks/(float)m_freq);
	m_prevFrame = m_currentFrame;
}

float Timer::GetDeltaTime()
{
	float dt = m_deltaTime;
	if(dt <= 0.0f || dt > 1.0f)
	{
		dt = 0.025f;
	}

	return dt;
}