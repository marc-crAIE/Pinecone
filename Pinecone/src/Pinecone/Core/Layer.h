#pragma once

#include "Pinecone/Core/Base.h"
#include "Pinecone/Core/Timestep.h"
#include "Pinecone/Events/Event.h"

namespace Pinecone
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer")
			: m_DebugName(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() { }
		virtual void OnDetach() { }
		virtual void OnUpdate(Timestep ts) { }
		virtual void OnEvent(Event& event) { }

		const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}