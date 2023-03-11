#include "pcpch.h"
#include "RenderCommand.h"

#include <glad/glad.h>

namespace Pinecone
{
	void OpenGLMessageCallback(
		unsigned int source,
		unsigned int type,
		unsigned int id,
		unsigned int severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         PC_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       PC_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          PC_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: PC_CORE_TRACE(message); return;
		}

		PC_CORE_ASSERT(false, "Unknown severity level!");
	}

	void RenderCommand::Init()
	{
#ifdef PC_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void RenderCommand::SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}