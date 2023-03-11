#pragma once

struct GLFWwindow;

namespace Pinecone
{
	class OpenGLContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext() = default;

		void Init();
		void SwapBuffers();

		static Scope<OpenGLContext> Create(GLFWwindow* window);
	private:
		GLFWwindow* m_WindowHandle;
	};
}