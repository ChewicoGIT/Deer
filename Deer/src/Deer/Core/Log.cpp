#include "Log.h"

namespace Deer {
	std::shared_ptr<spdlog::logger> Log::coreLogger;
	std::shared_ptr<spdlog::logger> Log::clientLogger;
	std::shared_ptr<spdlog::logger> Log::scriptLogger;

	void Log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		coreLogger = spdlog::stdout_color_mt("Core");
		clientLogger = spdlog::stdout_color_mt("Client");
		scriptLogger = spdlog::stdout_color_mt("Script");
		
		coreLogger->set_level(spdlog::level::level_enum::trace);
		clientLogger->set_level(spdlog::level::level_enum::trace);
		scriptLogger->set_level(spdlog::level::level_enum::trace);
	}

	void Log::shutdown() {
		coreLogger.reset();
		clientLogger.reset();
		scriptLogger.reset();

		spdlog::drop_all();
	}

	void Log::coreTrace(const char* msg)
	{
		//coreLogger->trace(msg);
	}
}
