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
		// Call to the correct logger function depending on the severity of the OpenGL error
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
		PC_PROFILE_FUNCTION();

		// If we are in debug mode, setup our OpenGL debug output message settings and callback function
#ifdef PC_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
		// Enable blending and use the alpha channel
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Enable depth testing and anti-aliasing of lines
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

	void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		// Bind the vertex array
		vertexArray->Bind();
		// Get the count of the total number of indices from either the vertex array or the specified index count
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		// Draw the elements with OpenGL
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void RenderCommand::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void RenderCommand::SetLineWidth(float width)
	{
		glLineWidth(width);
	}
}