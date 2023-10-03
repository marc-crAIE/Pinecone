#pragma once

// Imcluding ImGui here has conflicts with out the editor includes ImGui... not sure why
// So here are some forward declarations of a few ImGui types
typedef int ImGuiCol;
typedef unsigned int ImU32;
struct ImVec4;

namespace Pinecone::UI
{
	struct ScopedStyleColor
	{
		ScopedStyleColor() = default;
		ScopedStyleColor(ImGuiCol idx, ImVec4 color, bool predicate = true);
		ScopedStyleColor(ImGuiCol idx, ImU32 color, bool predicate = true);

		~ScopedStyleColor();
	private:
		bool m_Set = false;
	};
}