#pragma once

#include <glm/glm.hpp>

#include "Pinecone/Core/KeyCodes.h"
#include "Pinecone/Core/MouseCodes.h"

namespace Pinecone
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}