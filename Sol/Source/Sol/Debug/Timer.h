#pragma once
#include <chrono>
#include <iostream>

namespace Sol 
{
#define TIMER_PROFILE_SCOPE(name) Sol::Timer timer##__LINE__(name, [&](ProfileResult profileResult){m_ProfileResults.push_back(profileResult);})

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	template<typename Fn>
	class Timer
	{
	#define CHRONO std::chrono
	public:
		Timer(const char* name, Fn&& func):
			m_Name(name),
			m_Func(func),
			m_Stopped(false)
		{
			m_StartTimePoint = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			if(!m_Stopped)
			{
				Stop();
			}
		}
		void Stop()
		{
			auto endTimePoint = CHRONO::high_resolution_clock::now();

			long long start = CHRONO::time_point_cast<CHRONO::microseconds>(m_StartTimePoint).time_since_epoch().count();
			long long end = CHRONO::time_point_cast<CHRONO::microseconds>(endTimePoint).time_since_epoch().count();

			m_Stopped = true;
			float duration = (end - start) * 0.001f;
			//std::cout << m_Name << "-Duration: " << duration << "ms" << std::endl;
			m_Func({ m_Name,duration });
		}
	private:
		const char* m_Name;
		Fn m_Func;
		bool m_Stopped;
		CHRONO::time_point<CHRONO::steady_clock> m_StartTimePoint;
	};
}