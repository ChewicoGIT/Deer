#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>

namespace spdlog {
	class logger;
}

namespace Deer {
	class Log
	{
	public:
		static void init();
		static void shutdown();

		static void coreTrace(const char* msg);

		static inline std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }
		static inline std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	};
}

#define DEER_CORE_TRACE( ... ) Deer::Log::getCoreLogger()->trace(__VA_ARGS__)
#define DEER_CORE_INFO( ... ) Deer::Log::getCoreLogger()->info(__VA_ARGS__)
#define DEER_CORE_WARN( ... ) Deer::Log::getCoreLogger()->warn(__VA_ARGS__)
#define DEER_CORE_ERROR( ... ) Deer::Log::getCoreLogger()->error(__VA_ARGS__)

#define DEER_CORE_ASSERT(condition , ... ) if (!(condition)) {Deer::Log::getCoreLogger()->error(__VA_ARGS__);__debugbreak();}
