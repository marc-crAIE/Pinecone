#include "pcpch.h"
#include "UICore.h"

#include <imgui.h>

namespace Pinecone::UI
{
	static int s_UIContextID = 0;
	static uint32_t s_Counter = 0;
	static char s_IDBuffer[16 + 2 + 1] = "##";
	static char s_LabelIDBuffer[1024 + 1];

	const char* GenerateID()
	{
		snprintf(s_IDBuffer + 2, 16, "%u", s_Counter++);
		return s_IDBuffer;
	}

	const char* GenerateLabelID(std::string_view label)
	{
		*fmt::format_to_n(s_LabelIDBuffer, std::size(s_LabelIDBuffer), "{}##{}", label, s_Counter++).out = 0;
		return s_LabelIDBuffer;
	}

	void PushID()
	{
		ImGui::PushID(s_UIContextID++);
		s_Counter = 0;
	}

	void PopID()
	{
		ImGui::PopID();
		s_UIContextID--;
	}
}