#pragma once

class Timer
{
public:
	Timer();

	float GetTime() const;
	float GetDeltaTime() const;

	void Reset();

	// Used for pausing
	void Start();
	void Stop();

	void Tick();

private:
	// Doubles are more precise than float
	double m_SecondsPerCount;
	double m_DeltaTime;

	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PreviousTime;
	__int64 m_CurrentTime;

	bool m_Stopped;
};


