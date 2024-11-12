#pragma once
#include "Deer/Core/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

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

		static inline Ref<spdlog::logger>& getCoreLogger() { return coreLogger; }
		static inline Ref<spdlog::logger>& getClientLogger() { return clientLogger; }
		static inline Ref<spdlog::logger>& getScriptLogger() { return scriptLogger; }
	private:
		static Ref<spdlog::logger> coreLogger;
		static Ref<spdlog::logger> clientLogger;
		static Ref<spdlog::logger> scriptLogger;
	};
}

#define DEER_CORE_TRACE( ... ) Deer::Log::getCoreLogger()->trace(__VA_ARGS__)
#define DEER_CORE_INFO( ... ) Deer::Log::getCoreLogger()->info(__VA_ARGS__)
#define DEER_CORE_WARN( ... ) Deer::Log::getCoreLogger()->warn(__VA_ARGS__)
#define DEER_CORE_ERROR( ... ) Deer::Log::getCoreLogger()->error(__VA_ARGS__)

#define DEER_SCRIPT_TRACE( ... ) Deer::Log::getScriptLogger()->trace(__VA_ARGS__)
#define DEER_SCRIPT_INFO( ... ) Deer::Log::getScriptLogger()->info(__VA_ARGS__)
#define DEER_SCRIPT_WARN( ... ) Deer::Log::getScriptLogger()->warn(__VA_ARGS__)
#define DEER_SCRIPT_ERROR( ... ) Deer::Log::getScriptLogger()->error(__VA_ARGS__)

#define DEER_CORE_ASSERT(condition , ... ) if (!(condition)) {Deer::Log::getCoreLogger()->error(__VA_ARGS__);__debugbreak();}
#define DEER_SCRIPT_ASSERT(condition , ... ) if (!(condition)) {Deer::Log::getScriptLogger()->error(__VA_ARGS__);__debugbreak();}
