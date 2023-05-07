#include "Timer.h"

#include <Windows.h>

Timer::Timer() : m_SecondsPerCount(0.0), m_DeltaTime(-1.0), m_BaseTime(0), m_PausedTime(0),
m_PreviousTime(0), m_CurrentTime(0), m_Stopped(false)
{
    __int64 countsPerSeconds;
    // Query Frequency of the performance counter
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSeconds);
    m_SecondsPerCount = 1.0 / (double)countsPerSeconds;
}

float Timer::GetTime() const
{

    if (m_Stopped)
    {
        return (float)(((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
    }
    else
    {
        return (float)(((m_CurrentTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
    }
}

float Timer::GetDeltaTime() const
{
    return (float)m_DeltaTime;
}

void Timer::Reset()
{
    __int64 currentTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

    m_BaseTime = currentTime;
    m_PreviousTime = currentTime;
    m_StopTime = 0;
    m_Stopped = false;

}

void Timer::Start()
{
    if (m_Stopped)
    {
        __int64 startTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

        m_PausedTime += (startTime - m_StopTime);

        m_PreviousTime = startTime;
        m_StopTime = 0;
        m_Stopped = false;
    }
}

void Timer::Stop()
{
    // Don't do anything if already stopped
    if (!m_Stopped)
    {
        __int64 currentTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

        // Note the stop time
        m_StopTime = currentTime;

        m_Stopped = true;
    }
}

void Timer::Tick()
{
    if (m_Stopped)
    {
        m_DeltaTime = 0.0;
        return;
    }

    // Current time of this frame
    __int64 currentTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
    m_CurrentTime = currentTime;

    // Delta time is the diference in time between previous frame and current frame
    m_DeltaTime = (m_CurrentTime - m_PreviousTime) * m_SecondsPerCount;

    // Set previous time for next frame
    m_PreviousTime = m_CurrentTime;


    // Stops the delta time from going negative
    // When the computer goes into power save mode or it gets shuffled 
    // to another processor then the delta time can become negative
    if (m_DeltaTime < 0.0)
        m_DeltaTime = 0.0;
}
