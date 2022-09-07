#pragma once

namespace Sol 
{
	//by default TimeStep returns in seconds
	class TimeStep
	{
	public:
		TimeStep(float time) : m_Time(time){}

		operator float() const { return m_Time; }
		float GetMilliSeconds() const { return m_Time * 1000.f; }
	private:
		float m_Time;
	};
}