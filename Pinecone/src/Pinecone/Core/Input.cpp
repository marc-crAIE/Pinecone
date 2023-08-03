#include "pcpch.h"
#include "Input.h"

#include "Pinecone/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Pinecone
{
	bool Input::IsKeyPressed(const KeyCode key)
	{
		// Get the GLFW window handle
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		// Get the state of the key and return if it is pressed
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		// Get the GLFW window handle
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		// Get the state of the mouse button and return if it is pressed
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonReleased(MouseCode button)
	{
		// Get the GLFW window handle
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		// Get the state of the mouse button and return if it is released
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_RELEASE;
	}

	glm::vec2 Input::GetMousePosition()
	{
		// Get the GLFW window handle
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		// Get the position of the mouse cursors x and y
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// Return the x and y position as a 2D vector
		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}