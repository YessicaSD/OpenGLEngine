#pragma once

#define NAMESPACE_BEGAN namespace Shadow {
#define NAMESPACE_END }


// THIS DEFINES ARE FOR WINDOWS 
#ifdef SW_PLATFORM_WINDOWS
#if HZ_DYNAMIC_LINK
	// IF ENGINE EXPORT DLL
	#ifdef SW_BUILD_DLL
		#define SHADOW_API __declspec(dllexport)

	// IF GAME IMPORT
	#else 
		#define SHADOW_API __declspec(dllimport)
	#endif
#else
	#define SHADOW_API
#endif
#else
	#error Shadow only supports Windows!
#endif

#define BIT(x) (1 << x)	

#define ARRAY_COUNT(x) (sizeof(x)/sizeof(x[0]))

#define SW_BIND_FN(fn) std::bind(&fn, this, std::placeholders::_1)