#include "pcpch.h"
#include "ImGuiUI.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>

namespace Pinecone::UI
{
	ScopedStyleColor::ScopedStyleColor(ImGuiCol idx, ImVec4 color, bool predicate)
		: m_Set(predicate)
	{
		if (predicate)
			ImGui::PushStyleColor(idx, color);
	}

	ScopedStyleColor::ScopedStyleColor(ImGuiCol idx, ImU32 color, bool predicate)
		: m_Set(predicate)
	{
		if (predicate)
			ImGui::PushStyleColor(idx, color);
	}

	ScopedStyleColor::~ScopedStyleColor()
	{
		if (m_Set)
			ImGui::PopStyleColor();
	}
}