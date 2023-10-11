#pragma once

#include "Pinecone/ImGui/ImGuiUI.h"

namespace Pinecone::UI
{
	const char* GenerateID();
	const char* GenerateLabelID(std::string_view label);
	void PushID();
	void PopID();
}