#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Sol
{
	class SOL_API Log
	{
	public:

		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

//CORE Log Macros
#define SOL_CORE_CRITICAL(...) ::Sol::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define SOL_CORE_ERROR(...) ::Sol::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SOL_CORE_WARN(...)  ::Sol::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SOL_CORE_INFO(...)  ::Sol::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SOL_CORE_TRACE(...) ::Sol::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Example of engine logging call SOL_CORE_TRACE("deltaTime {0}, ({1})ms", X, Y);

//Client Log Macros
#define SOL_CRITICAL(...) ::Sol::Log::GetClientLogger()->critical(__VA_ARGS__)
#define SOL_ERROR(...) ::Sol::Log::GetClientLogger()->error(__VA_ARGS__)
#define SOL_WARN(...)  ::Sol::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SOL_INFO(...)  ::Sol::Log::GetClientLogger()->info(__VA_ARGS__)
#define SOL_TRACE(...) ::Sol::Log::GetClientLogger()->trace(__VA_ARGS__)

//Example of client logging call SOL_TRACE("deltaTime {0}, ({1})ms", X, Y);