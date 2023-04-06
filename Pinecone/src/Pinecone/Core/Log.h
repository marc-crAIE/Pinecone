#pragma once

#include "Pinecone/Core/Base.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Pinecone {

	class Log
	{
	public:
		/// <summary>
		/// Initialize the logger. Needs to be called before using logging functions/macros
		/// </summary>
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
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

// Core log macros 
#define PC_CORE_TRACE(...)		::Pinecone::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PC_CORE_INFO(...)		::Pinecone::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PC_CORE_WARN(...)		::Pinecone::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PC_CORE_ERROR(...)		::Pinecone::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PC_CORE_CRITICAL(...)   ::Pinecone::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros 
#define PC_TRACE(...)			::Pinecone::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PC_INFO(...)			::Pinecone::Log::GetClientLogger()->info(__VA_ARGS__)
#define PC_WARN(...)			::Pinecone::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PC_ERROR(...)			::Pinecone::Log::GetClientLogger()->error(__VA_ARGS__)
#define PC_CRITICAL(...)		::Pinecone::Log::GetClientLogger()->critical(__VA_ARGS__)
