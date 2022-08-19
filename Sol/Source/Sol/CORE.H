#pragma once

#ifdef SOL_PLATFORM_WINDOWS
	#ifdef SOL_BUILD_DLL
		#define SOL_API __declspec(dllexport)
	#else
		#define SOL_API __declspec(dllimport)
	#endif // SOL_BUILD_DLL
#else
#error Sol only supports windows!
#endif // SOL_PLATFORM_WINDOWS

#ifdef SOl_ENABLE_ASSERT
#define SOL_ASSERT(x, ...) 
{
	if(!(x)) 
	{
		SOL_ERROR("Assertion failed: {0}", __VA_ARGS__);
		__debugbreak;
	} 
}
#define SOL_CORE_ASSERT(x, ...)
{
	if (!(x))
	{
		SOL_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__);
		__debugbreak;
	}
}
#else
#define SOL_ASSERT(x, ...)
#define SOL_CORE_ASSERT(x, ...)
#endif // SOl_ENABLE_ASSERT


#define BIT(x) (1<<x)

#define SOL_BIND_EVENT_FN(x)std::bind(&x, this, std::placeholders::_1)