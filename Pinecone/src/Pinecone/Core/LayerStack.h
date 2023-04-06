#pragma once

#include "Pinecone/Core/Base.h"
#include "Pinecone/Core/Layer.h"

#include <vector>

namespace Pinecone
{
	class LayerStack
	{
	public:
		/// <summary>
		/// The LayerStack constructor
		/// </summary>
		LayerStack() = default;
		/// <summary>
		/// The LayerStack destructor
		/// </summary>
		~LayerStack();

		/// <summary>
		/// Push a layer onto the layer stack. This layer will sit above the previous layer but
		/// come before the lowest overlay layer
		/// </summary>
		/// <param name="layer">The layer to push to the stack</param>
		void PushLayer(Layer* layer);
		/// <summary>
		/// Push a layer as an overlay onto the layer stack. This layer will sit above all other 
		/// layers including the top-most overlay
		/// </summary>
		/// <param name="layer">The layer to push to the stack</param>
		void PushOverlay(Layer* overlay);
		/// <summary>
		/// Remove a layer from the stack
		/// </summary>
		/// <param name="layer">The layer to remove</param>
		void PopLayer(Layer* layer);
		/// <summary>
		/// Remove an overlay layer from the stack
		/// </summary>
		/// <param name="layer">The layer to remove</param>
		void PopOverlay(Layer* overlay);

		// Iterator functions for dealing with our range-based for loops

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		std::vector<Layer*> m_Layers;
		uint32_t m_LayerInsertIndex = 0;
	};
}