#pragma once

#include <memory>

#ifdef PC_DEBUG
	#define PC_DEBUGBREAK() __debugbreak()
	#define PC_ENABLE_ASSERTS
#else
	#define PC_DEBUGBREAK()
#endif

#define PC_EXPAND_MACRO(x) x
#define PC_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define PC_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Pinecone
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "Pinecone/Core/Log.h"
#include "Pinecone/Core/Assert.h"