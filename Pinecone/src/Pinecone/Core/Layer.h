#pragma once

#include "Pinecone/Core/Base.h"
#include "Pinecone/Core/Timestep.h"
#include "Pinecone/Events/Event.h"

namespace Pinecone
{
	class Layer
	{
	public:
		/// <summary>
		/// The Layer constructor. Sets the debug name of the layer to the name specified.
		/// </summary>
		/// <param name="name">The layer name</param>
		Layer(const std::string& name = "Layer")
			: m_DebugName(name) {}
		/// <summary>
		/// The Layer deconstructor
		/// </summary>
		virtual ~Layer() = default;

		/// <summary>
		/// Called when the layer is attached to a layer stack
		/// </summary>
		virtual void OnAttach() { }
		/// <summary>
		/// Called when removed from a layer stack
		/// </summary>
		virtual void OnDetach() { }
		/// <summary>
		/// Called when the layer should be updated. Typically called in Application::Run
		/// </summary>
		/// <param name="ts">The amount of time passed in seconds</param>
		virtual void OnUpdate(Timestep ts) { }
		/// <summary>
		/// Called when an event has occured
		/// </summary>
		/// <param name="event">The event</param>
		virtual void OnEvent(Event& event) { }

		/// <summary>
		/// Get the debug name of the layer
		/// </summary>
		/// <returns>The layer name</returns>
		const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}