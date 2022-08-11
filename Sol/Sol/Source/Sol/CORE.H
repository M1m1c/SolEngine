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
