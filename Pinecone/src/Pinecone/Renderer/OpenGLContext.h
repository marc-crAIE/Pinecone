#pragma once

struct GLFWwindow;

namespace Pinecone
{
	class OpenGLContext
	{
	public:
		/// <summary>
		/// The OpenGL context constructor.
		/// </summary>
		/// <param name="windowHandle">The GLFW window handle</param>
		OpenGLContext(GLFWwindow* windowHandle);
		/// <summary>
		/// The OpenGL context destructor.
		/// </summary>
		~OpenGLContext() = default;

		/// <summary>
		/// Initialize our OpenGL graphics context
		/// </summary>
		void Init();
		/// <summary>
		/// Swap the front and back buffers
		/// </summary>
		void SwapBuffers();

		/// <summary>
		/// Create a smart unique pointer to a new OpenGL context.
		/// </summary>
		/// <param name="window">The GLFW window the OpenGL context will use</param>
		/// <returns>A unique pointer to a new OpenGL context</returns>
		static Scope<OpenGLContext> Create(GLFWwindow* window);
	private:
		GLFWwindow* m_WindowHandle;
	};
}