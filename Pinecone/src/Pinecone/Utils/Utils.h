#pragma once

#include <GLFW/glfw3.h>

namespace Pinecone
{
	class Time
	{
	public:
		/// <summary>
		/// Returns the time since GLFW was initialized
		/// </summary>
		/// <returns>Time since GLFW was initialized</returns>
		static float GetTime()
		{
			return glfwGetTime();
		}
	};
}