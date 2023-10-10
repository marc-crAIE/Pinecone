#pragma once

#include "Pinecone/Core/Base.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <map>

namespace Pinecone {

	class Log
	{
	public:
		enum class Type : uint8_t
		{
			Core = 0, Client = 1
		};
		enum class Level : uint8_t
		{
			Trace = 0, Info, Warn, Error, Fatal
		};
		struct TagDetails
		{
			bool Enabled = true;
			Level LevelFilter = Level::Trace;
		};

	public:
		/// <summary>
		/// Initialize the logger. Needs to be called before using logging functions/macros
		/// </summary>
		static void Init();
		static void Shutdown();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		static Ref<spdlog::logger>& GetEditorConsoleLogger() { return s_EditorConsoleLogger; }

		static bool HasTag(const std::string& tag) { return s_EnabledTags.find(tag) != s_EnabledTags.end(); }
		static std::map<std::string, TagDetails>& EnabledTags() { return s_EnabledTags; }

		template<typename... Args>
		static void PrintMessage(Log::Type type, Log::Level level, std::string_view tag, Args&&... args);

		template<typename... Args>
		static void PrintAssertMessage(Log::Type type, std::string_view prefix, Args&&... args);
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
		static Ref<spdlog::logger> s_EditorConsoleLogger;

		inline static std::map<std::string, TagDetails> s_EnabledTags;
	};
}

// These functions are here so that we can easily print glm vectors, matrices, and quaternions
// to the logger

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

/*
*		TAGGED LOGS
*/

#define PC_CORE_TRACE_TAG(tag, ...) ::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Core, ::Pinecone::Log::Level::Trace, tag, __VA_ARGS__)
#define PC_CORE_INFO_TAG(tag, ...)  ::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Core, ::Pinecone::Log::Level::Info, tag, __VA_ARGS__)
#define PC_CORE_WARN_TAG(tag, ...)  ::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Core, ::Pinecone::Log::Level::Warn, tag, __VA_ARGS__)
#define PC_CORE_ERROR_TAG(tag, ...) ::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Core, ::Pinecone::Log::Level::Error, tag, __VA_ARGS__)
#define PC_CORE_FATAL_TAG(tag, ...) ::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Core, ::Pinecone::Log::Level::Error, tag, __VA_ARGS__)

#define PC_TRACE_TAG(tag, ...)		::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Client, ::Pinecone::Log::Level::Trace, tag, __VA_ARGS__)
#define PC_INFO_TAG(tag, ...)		::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Client, ::Pinecone::Log::Level::Info, tag, __VA_ARGS__)
#define PC_WARN_TAG(tag, ...)		::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Client, ::Pinecone::Log::Level::Warn, tag, __VA_ARGS__)
#define PC_ERROR_TAG(tag, ...)		::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Client, ::Pinecone::Log::Level::Error, tag, __VA_ARGS__)
#define PC_FATAL_TAG(tag, ...)		::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Client, ::Pinecone::Log::Level::Error, tag, __VA_ARGS__)

/*
*		NORMAL LOGS
*/

// Core log macros 
#define PC_CORE_TRACE(...)		::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Core, ::Pinecone::Log::Level::Trace, "", __VA_ARGS__)
#define PC_CORE_INFO(...)		::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Core, ::Pinecone::Log::Level::Info, "", __VA_ARGS__)
#define PC_CORE_WARN(...)		::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Core, ::Pinecone::Log::Level::Warn, "", __VA_ARGS__)
#define PC_CORE_ERROR(...)		::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Core, ::Pinecone::Log::Level::Error, "", __VA_ARGS__)
#define PC_CORE_CRITICAL(...)   ::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Core, ::Pinecone::Log::Level::Error, "", __VA_ARGS__)

// Client log macros 
#define PC_TRACE(...)			::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Client, ::Pinecone::Log::Level::Trace, "", __VA_ARGS__)
#define PC_INFO(...)			::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Client, ::Pinecone::Log::Level::Info, "", __VA_ARGS__)
#define PC_WARN(...)			::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Client, ::Pinecone::Log::Level::Warn, "", __VA_ARGS__)
#define PC_ERROR(...)			::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Client, ::Pinecone::Log::Level::Error, "", __VA_ARGS__)
#define PC_CRITICAL(...)		::Pinecone::Log::PrintMessage(::Pinecone::Log::Type::Client, ::Pinecone::Log::Level::Error, "", __VA_ARGS__)

// Editor Console log macros 
#define PC_CONSOLE_LOG_TRACE(...)			::Pinecone::Log::GetEditorConsoleLogger()->trace(__VA_ARGS__)
#define PC_CONSOLE_LOG_INFO(...)			::Pinecone::Log::GetEditorConsoleLogger()->info(__VA_ARGS__)
#define PC_CONSOLE_LOG_WARN(...)			::Pinecone::Log::GetEditorConsoleLogger()->warn(__VA_ARGS__)
#define PC_CONSOLE_LOG_ERROR(...)			::Pinecone::Log::GetEditorConsoleLogger()->error(__VA_ARGS__)
#define PC_CONSOLE_LOG_CRITICAL(...)		::Pinecone::Log::GetEditorConsoleLogger()->critical(__VA_ARGS__)

namespace Pinecone
{
	template<typename ...Args>
	inline void Pinecone::Log::PrintMessage(Log::Type type, Log::Level level, std::string_view tag, Args && ...args)
	{
		auto detail = s_EnabledTags[std::string(tag)];
		if (detail.Enabled && detail.LevelFilter <= level)
		{
			auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
			std::string logString = tag.empty() ? "{0}{1}" : "[{0}] {1}";
			switch (level)
			{
			case Level::Trace:
				logger->trace(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Info:
				logger->info(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Warn:
				logger->warn(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Error:
				logger->error(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Fatal:
				logger->critical(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			}
		}
	}

	template<typename ...Args>
	inline void Pinecone::Log::PrintAssertMessage(Log::Type type, std::string_view prefix, Args && ...args)
	{
		auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
		logger->error("{0}: {1}", prefix, fmt::format(std::forward<Args>(args)...));
	}

	template<>
	inline void Pinecone::Log::PrintAssertMessage(Log::Type type, std::string_view prefix)
	{
		auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
		logger->error("{0}", prefix);
	}
}