#include "pcpch.h"
#include "UICore.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

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

	void ShiftCursor(float x, float y)
	{
		const ImVec2 cursor = ImGui::GetCursorPos();
		ImGui::SetCursorPos(ImVec2(cursor.x + x, cursor.y + y));
	}

	void ShiftCursorX(float distance)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + distance);
	}

	void ShiftCursorY(float distance)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + distance);
	}

	void BeginPropertyGrid(uint32_t columns)
	{
		PushID();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
		ImGui::Columns(columns);
	}

	void EndPropertyGrid()
	{
		ImGui::Columns(1);
		UI::Draw::Underline();
		ImGui::PopStyleVar(2); // ItemSpacing, FramePadding
		ShiftCursorY(18.0f);
		PopID();
	}

	bool Property(const char* label, bool& value, const char* helpText)
	{
		bool modified = false;

		ShiftCursor(10.0f, 9.0f);
		ImGui::Text(label);

		if (std::strlen(helpText) != 0)
		{
			ImGui::SameLine();
			HelpMarker(helpText);
		}

		ImGui::NextColumn();
		ShiftCursorY(4.0f);
		ImGui::PushItemWidth(-1);

		modified = UI::Checkbox(fmt::format("##{0}", label).c_str(), &value);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		Draw::Underline();

		return modified;
	}

	bool Property(const char* label, int32_t& value, int32_t min, int32_t max, const char* helpText)
	{
		ShiftCursor(10.0f, 9.0f);
		ImGui::Text(label);

		if (std::strlen(helpText) != 0)
		{
			ImGui::SameLine();
			HelpMarker(helpText);
		}

		ImGui::NextColumn();
		ShiftCursorY(4.0f);
		ImGui::PushItemWidth(-1);

		bool modified = UI::DragInt32(fmt::format("##{0}", label).c_str(), &value, 1.0f, min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		Draw::Underline();

		return modified;
	}

	bool Property(const char* label, uint32_t& value, uint32_t min, uint32_t max, const char* helpText)
	{
		ShiftCursor(10.0f, 9.0f);
		ImGui::Text(label);

		if (std::strlen(helpText) != 0)
		{
			ImGui::SameLine();
			HelpMarker(helpText);
		}

		ImGui::NextColumn();
		ShiftCursorY(4.0f);
		ImGui::PushItemWidth(-1);

		bool modified = UI::DragUInt32(fmt::format("##{0}", label).c_str(), &value, 1.0f, min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		Draw::Underline();

		return modified;
	}

	bool Property(const char* label, float& value, float delta, float min, float max, const char* helpText)
	{
		ShiftCursor(10.0f, 9.0f);
		ImGui::Text(label);

		if (std::strlen(helpText) != 0)
		{
			ImGui::SameLine();
			HelpMarker(helpText);
		}

		ImGui::NextColumn();
		ShiftCursorY(4.0f);
		ImGui::PushItemWidth(-1);

		bool modified = UI::DragFloat(fmt::format("##{0}", label).c_str(), &value, delta, min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		Draw::Underline();

		return modified;
	}

	bool PropertyDropdown(const char* label, const char** options, int32_t optionCount, int32_t* selected, const char* helpText)
	{
		const char* current = options[*selected];
		ShiftCursor(10.0f, 9.0f);
		ImGui::Text(label);

		if (std::strlen(helpText) != 0)
		{
			ImGui::SameLine();
			HelpMarker(helpText);
		}

		ImGui::NextColumn();
		ShiftCursorY(4.0f);
		ImGui::PushItemWidth(-1);

		bool modified = false;

		const std::string id = "##" + std::string(label);
		if (UI::BeginCombo(id.c_str(), current))
		{
			for (int i = 0; i < optionCount; i++)
			{
				const bool is_selected = (current == options[i]);
				if (ImGui::Selectable(options[i], is_selected))
				{
					current = options[i];
					*selected = i;
					modified = true;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			UI::EndCombo();
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		Draw::Underline();

		return modified;
	}

	bool PropertyDropdownNoLabel(const char* strID, const char** options, int32_t optionCount, int32_t* selected)
	{
		const char* current = options[*selected];
		ShiftCursorY(4.0f);
		ImGui::PushItemWidth(-1);

		bool modified = false;

		const std::string id = "##" + std::string(strID);
		if (UI::BeginCombo(id.c_str(), current))
		{
			for (int i = 0; i < optionCount; i++)
			{
				const bool is_selected = (current == options[i]);
				if (ImGui::Selectable(options[i], is_selected))
				{
					current = options[i];
					*selected = i;
					modified = true;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			UI::EndCombo();
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		Draw::Underline();

		return modified;
	}

	bool PropertyColor(const char* label, glm::vec3& value, const char* helpText)
	{
		ShiftCursor(10.0f, 9.0f);
		ImGui::Text(label);

		if (std::strlen(helpText) != 0)
		{
			ImGui::SameLine();
			HelpMarker(helpText);
		}

		ImGui::NextColumn();
		ShiftCursorY(4.0f);
		ImGui::PushItemWidth(-1);

		bool modified = UI::ColorEdit3(GenerateID(), glm::value_ptr(value));

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		Draw::Underline();

		return modified;
	}

	bool PropertyColor(const char* label, glm::vec4& value, const char* helpText)
	{
		ShiftCursor(10.0f, 9.0f);
		ImGui::Text(label);

		if (std::strlen(helpText) != 0)
		{
			ImGui::SameLine();
			HelpMarker(helpText);
		}

		ImGui::NextColumn();
		ShiftCursorY(4.0f);
		ImGui::PushItemWidth(-1);

		bool modified = UI::ColorEdit4(GenerateID(), glm::value_ptr(value));

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		Draw::Underline();

		return modified;
	}

	bool Property(const char* label, std::string& value, const char* helpText)
	{
		bool modified = false;

		ShiftCursor(10.0f, 9.0f);
		ImGui::Text(label);

		if (std::strlen(helpText) != 0)
		{
			ImGui::SameLine();
			HelpMarker(helpText);
		}

		ImGui::NextColumn();
		ShiftCursorY(4.0f);
		ImGui::PushItemWidth(-1);

		modified = UI::InputText(fmt::format("##{0}", label).c_str(), &value);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		Draw::Underline();

		return modified;
	}

	void Property(const char* label, const std::string& value, const char* helpText)
	{
		ShiftCursor(10.0f, 9.0f);
		ImGui::Text(label);

		if (std::strlen(helpText) != 0)
		{
			ImGui::SameLine();
			HelpMarker(helpText);
		}

		ImGui::NextColumn();
		ShiftCursorY(4.0f);
		ImGui::PushItemWidth(-1);
		BeginDisabled();
		UI::InputText(fmt::format("##{0}", label).c_str(), (char*)value.c_str(), value.size(), ImGuiInputTextFlags_ReadOnly);
		EndDisabled();

		ImGui::PopItemWidth();
		ImGui::NextColumn();
		Draw::Underline();
	}

	bool PropertyMultiline(const char* label, std::string& value, const char* helpText)
	{
		bool modified = false;

		ImGui::Text(label);

		if (std::strlen(helpText) != 0)
		{
			ImGui::SameLine();
			HelpMarker(helpText);
		}

		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		modified = UI::InputTextMultiline(fmt::format("##{0}", label).c_str(), &value);

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}