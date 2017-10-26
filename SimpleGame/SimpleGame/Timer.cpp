#include "stdafx.h"
#include "Timer.h"

#define FPS_LIMIT 144


void Timer::Init()
{
	m_CurrentTime = std::chrono::system_clock::now();
	m_fps = 0.f;
}

void Timer::getTimeset()
{
	m_TimeElapsed = std::chrono::system_clock::now() - m_CurrentTime;
	if (m_TimeElapsed.count() < EPSILON) 
		m_TimeElapsed = std::chrono::duration<double>::zero();

	if (m_TimeElapsed.count() > (1 / FPS_LIMIT)) {
		m_CurrentTime = std::chrono::system_clock::now();
	}
}


