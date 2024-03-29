#pragma once
#include <memory>


#ifdef SOL_PLATFORM_WINDOWS
#if SOL_DYNAMIC_LINK
	#ifdef SOL_BUILD_DLL
		#define SOL_API __declspec(dllexport)
	#else
		#define SOL_API __declspec(dllimport)
	#endif // SOL_BUILD_DLL
#else
#define SOL_API
#endif
#else
#error Sol only supports windows!
#endif // SOL_PLATFORM_WINDOWS


#ifdef SOL_DEBUG
	#define SOL_ENABLE_ASSERTS
#endif


#ifdef SOL_ENABLE_ASSERTS
#include "Log.h"
#define SOL_ASSERT(x, ...) { if(!(x)) { SOL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define SOL_CORE_ASSERT(x, ...) { if(!(x)) { SOL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define SOL_ASSERT(x, ...)
#define SOL_CORE_ASSERT(x, ...)
#endif // SOl_ENABLE_ASSERT


#define BIT(x) (1<<x)

#define SOL_BIND_EVENT_FN(x)std::bind(&x, this, std::placeholders::_1)

namespace Sol
{
	template<typename T> 
	using u_ptr = std::unique_ptr<T>;

	template<typename T> 
	using s_ptr = std::shared_ptr<T>;
}