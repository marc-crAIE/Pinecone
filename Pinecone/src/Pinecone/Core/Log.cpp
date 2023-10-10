#include "pcpch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#define PC_HAS_CONSOLE !PC_DIST

namespace Pinecone
{
	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;
	Ref<spdlog::logger> Log::s_EditorConsoleLogger;

	void Log::Init()
	{
		// Create a new console logger with color and another logger to a file
		// These are setup as sinks that are then used by the actual loggers to output data
		// https://github.com/gabime/spdlog/wiki/2.-Creating-loggers
		std::vector<spdlog::sink_ptr> logSinks;
#if PC_HAS_CONSOLE
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
#endif
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Pinecone.log", true));

		// Set the log format (reference https://github.com/gabime/spdlog/wiki/3.-Custom-formatting)
		logSinks[0]->set_pattern("%^[%T] %n: %$%v");
		logSinks[1]->set_pattern("[%T] [%l] %n: %$%v");

		// Create the core logger, this is to be used internally by the engine itself
		s_CoreLogger = std::make_shared<spdlog::logger>("PINECONE", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		// Create the client logger, this is to be used by the application
		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);

		// Create the editor console logger, this is to be used by the editor
		s_EditorConsoleLogger = std::make_shared<spdlog::logger>("EDITOR", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_EditorConsoleLogger);
		s_EditorConsoleLogger->set_level(spdlog::level::trace);
		s_EditorConsoleLogger->flush_on(spdlog::level::trace);
	}

	void Log::Shutdown()
	{
		s_EditorConsoleLogger.reset();
		s_ClientLogger.reset();
		s_CoreLogger.reset();
		spdlog::drop_all();
	}
}