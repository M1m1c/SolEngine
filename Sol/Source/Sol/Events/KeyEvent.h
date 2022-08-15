#pragma once
#include "Event.h"
 
#include <sstream> 

namespace Sol
{
	class SOL_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const{ return m_KeyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode):m_KeyCode(keycode) {}
		int m_KeyCode;
	};

	class SOL_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode,int repeatCount): KeyEvent(keyCode),m_RepeatCount(repeatCount){}
		
		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override 
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)
	protected:
		int m_RepeatCount;
	};

	class SOL_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode){}


		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};
	
}