#include "pcpch.h"
#include "SelectionManager.h"

#include "Pinecone/Core/Application.h"
#include "Pinecone/Events/EditorEvent.h"

namespace Pinecone
{
	void SelectionManager::Select(SelectionContext context, UUID selectionID)
	{
		auto& contextSelections = s_Contexts[context];
		if (std::find(contextSelections.begin(), contextSelections.end(), selectionID) != contextSelections.end())
			return;

		//TODO: Verify that a selection ID is already in another context

		contextSelections.push_back(selectionID);
		Application::Get().DispatchEvent<EditorSelectionChangedEvent>(context, selectionID, true);
	}

	bool SelectionManager::IsSelected(UUID selectionID)
	{
		for (const auto& [contextID, contextSelections] : s_Contexts)
		{
			if (std::find(contextSelections.begin(), contextSelections.end(), selectionID) != contextSelections.end())
				return true;
		}

		return false;
	}

	bool SelectionManager::IsSelected(SelectionContext context, UUID selectionID)
	{
		const auto& contextSelections = s_Contexts[context];
		return std::find(contextSelections.begin(), contextSelections.end(), selectionID) != contextSelections.end();
	}

	void SelectionManager::Deselect(UUID selectionID)
	{
		for (auto& [contextID, contextSelections] : s_Contexts)
		{
			auto it = std::find(contextSelections.begin(), contextSelections.end(), selectionID);
			if (it == contextSelections.end())
				continue;

			Application::Get().DispatchEvent<EditorSelectionChangedEvent>(contextID, selectionID, false);
			contextSelections.erase(it);
			break;
		}
	}

	void SelectionManager::Deselect(SelectionContext context, UUID selectionID)
	{
		auto& contextSelections = s_Contexts[context];
		auto it = std::find(contextSelections.begin(), contextSelections.end(), selectionID);
		if (it == contextSelections.end())
			return;

		contextSelections.erase(it);
	}

	void SelectionManager::DeselectAll()
	{
		for (auto& [ctxID, contextSelections] : s_Contexts)
		{
			for (const auto& selectionID : contextSelections)
				Application::Get().DispatchEvent<EditorSelectionChangedEvent>(ctxID, selectionID, false);
			contextSelections.clear();
		}
	}

	void SelectionManager::DeselectAll(SelectionContext context)
	{
		auto& contextSelections = s_Contexts[context];

		for (const auto& selectionID : contextSelections)
			Application::Get().DispatchEvent<EditorSelectionChangedEvent>(context, selectionID, false);

		contextSelections.clear();
	}

	UUID SelectionManager::GetSelection(SelectionContext context, size_t index)
	{
		auto& contextSelections = s_Contexts[context];
		PC_CORE_VERIFY(index >= 0 && index < contextSelections.size());
		return contextSelections[index];
	}

	size_t SelectionManager::GetSelectionCount(SelectionContext contextID)
	{
		return s_Contexts[contextID].size();
	}
}