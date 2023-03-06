#pragma once

#include "Sol/SolDefines.h"

namespace Sol 
{
	struct NameComp
	{
		NameComp() = default;
		NameComp(const NameComp&) = default;
		NameComp(const std::string& name) :m_Name(name) {}
		~NameComp() = default;

		std::string GetName() { return m_Name; }
		operator std::string& () { return m_Name; }
		operator const std::string& () const { return m_Name; }
	private:
		std::string m_Name;
	};
 }