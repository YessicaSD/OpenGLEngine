#pragma once

#include "swpch.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h" //in order to log custom types


NAMESPACE_BEGAN
	class SHADOW_API Log
	{
		public:
			static void Init();

			inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger;  }
			inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
			
	};

NAMESPACE_END

#define SW_CORE_ERROR(...)	::Shadow::Log::GetCoreLogger()->error(__VA_ARGS__);
#define SW_CORE_WARN(...)	::Shadow::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define SW_CORE_INFO(...)	::Shadow::Log::GetCoreLogger()->info(__VA_ARGS__);
#define SW_CORE_TRACE(...)	::Shadow::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define SW_CORE_FATAL(...)	::Shadow::Log::GetCoreLogger()->fatal(__VA_ARGS__);

#define SW_ERROR(...)	::Shadow::Log::GetClientLogger()->error(__VA_ARGS__);
#define SW_WARN(...)	::Shadow::Log::GetClientLogger()->warn(__VA_ARGS__);
#define SW_INFO(...)	::Shadow::Log::GetClientLogger()->info(__VA_ARGS__);
#define SW_TRACE(...)	::Shadow::Log::GetClientLogger()->trace(__VA_ARGS__);
#define SW_FATAL(...)	::Shadow::Log::GetClientLogger()->fatal(__VA_ARGS__);

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