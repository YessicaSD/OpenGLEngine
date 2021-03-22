#pragma once

#define NAMESPACE_BEGAN namespace Shadow {
#define NAMESPACE_END }


// THIS DEFINES ARE FOR WINDOWS 
#ifdef SW_PLATFORM_WINDOWS
	
	// IF ENGINE EXPORT DLL
	#ifdef SW_BUILD_DLL
		#define SHADOW_API __declspec(dllexport)

	// IF GAME IMPORT
	#else 
		#define SHADOW_API __declspec(dllimport)
	#endif


#else
	#error Hazel only supports Windows!
#endif

#define BIT(x) (1 << x)	

#define SW_ENABLE_ASSERTS

#ifdef SW_ENABLE_ASSERTS
	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define SW_ASSERT(x, ...)							\
	{													\
		if(!(x))										\
		{												\
			SW_ERROR("Assertion Fail: {0}", __VA_ARGS__); \
			__debugbreak();								\
		}												\
	} 

	#define SW_CORE_ASSERT(x,...)							 \
	{														 \
		if (!(x))											 \
		{													 \
			SW_CORE_ERROR("Assertion Fail: {0}", __VA_ARGS__); \
			__debugbreak();									 \
		}													 \
	}
#else
	#define SW_ASSERT(x, ...)
	#define SW_CORE_ASSERT(x, ...)
#endif

#define SW_BIND_FN(fn) std::bind(&fn, this, std::placeholders::_1)