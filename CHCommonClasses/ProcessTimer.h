#pragma once

class CProcessTimer
{
public:

	CProcessTimer(void)
	{
		m_swFreq.LowPart = m_swFreq.HighPart = 0;
		m_swStart = m_swFreq;
		m_swEnd = m_swFreq;
		m_fTimeforDuration = 0;
		QueryPerformanceFrequency(&m_swFreq);
	}

	void Start(void)
	{
		QueryPerformanceCounter(&m_swStart);
	}

	void End(void)
	{
		QueryPerformanceCounter(&m_swEnd);
		m_fTimeforDuration = (m_swEnd.QuadPart - m_swStart.QuadPart)/(float)m_swFreq.QuadPart;
	}

	const float GetDurationSecond(void) const		{ return m_fTimeforDuration; }
	const float GetDurationMilliSecond(void) const	{ return m_fTimeforDuration*1000.f; }
	LARGE_INTEGER GetStartInteger(void) const		{ return m_swStart; }
	LARGE_INTEGER GetEndInteger(void) const			{ return m_swEnd; }
	LARGE_INTEGER GetFrequency(void) const			{ return m_swFreq; }

private:
	LARGE_INTEGER	m_swFreq, m_swStart, m_swEnd;
	float			m_fTimeforDuration;

};
