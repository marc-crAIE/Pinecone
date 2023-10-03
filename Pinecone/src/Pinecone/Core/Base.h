#pragma once

#include <memory>

#ifdef PC_DEBUG
	// Allow the use of __debugbreak when debugging, this macro does nothing in release mode
	#define PC_DEBUGBREAK() __debugbreak()
	// Specify that assertion is allowed, not defined in release mode
	#define PC_ENABLE_ASSERTS
#else
	#define PC_DEBUGBREAK()
#endif

#ifndef PC_DIST
	#define PC_ENABLE_VERIFY
#endif

#define PC_EXPAND_MACRO(x) x
#define PC_STRINGIFY_MACRO(x) #x

// Bit shifting
#define BIT(x) (1 << x)

// Used to call event functions with the EventDispatcher::Dispatch function
#define PC_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Pinecone
{
	// This code here is just a short hand names for unique and shared smart pointers.
	// This also includes the functions to create them. Useful if I want to have pointer 
	// but not always have to manage all of them. At the cost of memory of course

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