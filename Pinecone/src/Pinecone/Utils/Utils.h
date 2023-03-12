#pragma once

#include <GLFW/glfw3.h>

namespace Pinecone
{
	class Time
	{
	public:
		static float GetTime()
		{
			return glfwGetTime();
		}
	};
}