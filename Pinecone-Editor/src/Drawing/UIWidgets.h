#pragma once

#include <Pinecone/Core/Base.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Pinecone {
	namespace UI
	{
		bool DrawVec2Control(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float columnWidth = 75.0f);
		bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 75.0f);
		bool DrawVec4Control(const std::string& label, glm::vec4& values, float resetValue = 0.0f, float columnWidth = 75.0f);

		bool DrawTreeNodeWithImage(ImTextureID user_texture_id, const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...);
	}
}