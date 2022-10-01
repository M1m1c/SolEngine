#pragma once

namespace Sol
{
	class ISystem
	{
	public:
		ISystem();
		~ISystem();

		virtual void OnStartSystem() = 0;
		virtual void OnUpdateSystem() = 0;

	private:

	};

	
}