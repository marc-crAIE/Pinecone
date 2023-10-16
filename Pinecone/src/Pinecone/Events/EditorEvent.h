#pragma once

#include "Pinecone/Events/Event.h"
#include "Pinecone/Editor/SelectionManager.h"

namespace Pinecone
{
	class EditorSelectionChangedEvent : public Event
	{
	public:
		EditorSelectionChangedEvent(SelectionContext contextID, UUID selectionID, bool selected)
			: m_Context(contextID), m_SelectionID(selectionID), m_Selected(selected)
		{}

		SelectionContext GetContextID() const { return m_Context; }
		UUID GetSelectionID() const { return m_SelectionID; }
		bool IsSelected() const { return m_Selected; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "EditorSelectionChangedEvent: Context(" << (int32_t)m_Context << "), Selection(" << m_SelectionID << "), " << m_Selected;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryEditor)
		EVENT_CLASS_TYPE(EditorSelectionChanged)
	private:
		SelectionContext m_Context;
		UUID m_SelectionID;
		bool m_Selected;
	};
}