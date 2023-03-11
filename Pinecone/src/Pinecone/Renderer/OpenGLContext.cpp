#include "pcpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Pinecone
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        PC_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        PC_CORE_ASSERT(status, "Failed to initialize Glad!");

        PC_CORE_INFO("OpenGL Info: {0} {1}", (const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER));
        PC_CORE_INFO("  Vendor:   {0}", (const char*)glGetString(GL_VENDOR));
        PC_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
        PC_CORE_INFO("  Version:  {0}", (const char*)glGetString(GL_VERSION));

        PC_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Pinecone requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

    Scope<OpenGLContext> OpenGLContext::Create(GLFWwindow* window)
    {
        return CreateScope<OpenGLContext>(window);
    }
}
