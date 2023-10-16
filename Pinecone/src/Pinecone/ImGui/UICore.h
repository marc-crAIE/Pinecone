#pragma once

#include "Pinecone/ImGui/ImGuiUI.h"

namespace Pinecone::UI
{
	const char* GenerateID();
	const char* GenerateLabelID(std::string_view label);
	void PushID();
	void PopID();

	void ShiftCursor(float x, float y);
	void ShiftCursorX(float distance);
	void ShiftCursorY(float distance);

	void BeginPropertyGrid(uint32_t columns = 2U);
	void EndPropertyGrid();

	bool Property(const char* label, bool& value, const char* helpText = "");

	bool Property(const char* label, int32_t& value, int32_t min = 0, int32_t max = 0, const char* helpText = "");
	bool Property(const char* label, uint32_t& value, uint32_t min = 0, uint32_t max = 0, const char* helpText = "");

	bool Property(const char* label, float& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f, const char* helpText = "");

	bool PropertyDropdown(const char* label, const char** options, int32_t optionCount, int32_t* selected, const char* helpText = "");
	bool PropertyDropdownNoLabel(const char* strID, const char** options, int32_t optionCount, int32_t* selected);

	bool PropertyColor(const char* label, glm::vec3& value, const char* helpText = "");
	bool PropertyColor(const char* label, glm::vec4& value, const char* helpText = "");

	bool Property(const char* label, std::string& value, const char* helpText = "");
	void Property(const char* label, const std::string& value, const char* helpText = "");
	bool PropertyMultiline(const char* label, std::string& value, const char* helpText = "");

	void HelpMarker(const char* desc);
}