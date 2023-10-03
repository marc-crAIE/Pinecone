#pragma once
#include <functional>

#include "Pinecone/Core/Base.h"
#include "Pinecone/Debug/Instrumentor.h"

namespace Pinecone
{
	/// The different event types
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowDrop,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// The different event categories
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput		 = BIT(1),
		EventCategoryKeyboard	 = BIT(2),
		EventCategoryMouse		 = BIT(3),
		EventCategoryMouseButton = BIT(4),
	};

	// Used as a shortcut for defining these functions when creating the event types
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

	// 
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		/// <summary>
		/// The event destructor
		/// </summary>
		virtual ~Event() = default;

		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		/// <summary>
		/// Returns if the event is part of a specific category
		/// </summary>
		/// <param name="category">The catagory to check</param>
		/// <returns>If the event is in the specified category</returns>
		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		/// <summary>
		/// The EventDispatcher constructor. Creates a new event dispater to handle the 
		/// specified event.
		/// </summary>
		/// <param name="event">The event to handle</param>
		EventDispatcher(Event& event)
			: m_Event(event)
		{ }

		/// <summary>
		/// Check if an event is of a specific type, if so dispatch the event by calling
		/// a function.
		/// </summary>
		/// <typeparam name="T">The event type</typeparam>
		/// <typeparam name="F">The function to call</typeparam>
		/// <param name="func">The function to handle the event</param>
		/// <returns>True if the event type was the type specified</returns>
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			// Check if the event the dispatcher was setup with is the same type as the one we want
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				// Set if the event was handled from the output of the function
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	/// <summary>
	/// Put the event as a string to an output stream buffer
	/// </summary>
	/// <param name="os">The output stream</param>
	/// <param name="e">The event to print</param>
	/// <returns>The output stream with the event string pushed to it</returns>
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}