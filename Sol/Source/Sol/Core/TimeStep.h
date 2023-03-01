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

		TimeStep operator+(const float& addative) const {
			return TimeStep(m_Time + addative);
		}

		TimeStep operator-(const float& subtractor) const {
			return TimeStep(m_Time - subtractor);
		}

		TimeStep operator*(const float& multiplier) const {
			return TimeStep(m_Time * multiplier);
		}

		float GetMilliSeconds() const { return m_Time * 1000.f; }
	private:
		float m_Time;
	};

}