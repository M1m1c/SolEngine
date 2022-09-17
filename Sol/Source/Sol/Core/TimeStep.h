#pragma once

namespace Sol 
{
	//by default TimeStep returns in seconds
	class TimeStep
	{
	public:
		TimeStep(float time) : m_Time(time){}

		operator float() const { return m_Time; }
		TimeStep& operator+=(const float& addative) {
			m_Time += addative;
			return *this;
		}
		TimeStep& operator-=(const float& subtractor) {
			m_Time -= subtractor;
			return *this;
		}
		TimeStep& operator*=(const float& multiplier) {
			m_Time *= multiplier;
			return *this;
		}

		float GetMilliSeconds() const { return m_Time * 1000.f; }
	private:
		float m_Time;
	};
}