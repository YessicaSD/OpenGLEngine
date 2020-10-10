#pragma once

#ifdef SW_PLATFORM_WINDOWS
	#ifdef SW_BUILD_DLL
		#define SHADOW_API __declspec(dllexport)
	#else 
		#define SHADOW_API __declspec(dllimport)
	#endif // HZ_BUILD_DLL
#else
	#error Hazel only supports Windows!
#endif // SW_PLATFORM_WINDOWS
