#pragma once

#include <glm/glm.hpp>

#include "Pinecone/Core/KeyCodes.h"
#include "Pinecone/Core/MouseCodes.h"

namespace Pinecone
{
	class Input
	{
	public:
		/// <summary>
		/// Check if a key is pressed
		/// </summary>
		/// <param name="key">The key to check</param>
		/// <returns>True if the key is pressed</returns>
		static bool IsKeyPressed(KeyCode key);

		/// <summary>
		/// Check if a mouse button is pressed
		/// </summary>
		/// <param name="button">The mouse button to check</param>
		/// <returns>True if the mouse button is pressed</returns>
		static bool IsMouseButtonPressed(MouseCode button);
		/// <summary>
		/// Get the mouse cursor position on the window (in pixels)
		/// </summary>
		/// <returns>The mouse cursor position</returns>
		static glm::vec2 GetMousePosition();
		/// <summary>
		/// Get the X position of the mouse cursor
		/// </summary>
		/// <returns>The mouse cursors X position</returns>
		static float GetMouseX();
		/// <summary>
		/// Get the Y position of the mouse cursor
		/// </summary>
		/// <returns>The mouse cursors Y position</returns>
		static float GetMouseY();
	};
}