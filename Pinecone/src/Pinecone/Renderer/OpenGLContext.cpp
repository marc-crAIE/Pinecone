#include "pcpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Pinecone
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        // Ensure that we have a valid GLFW window handle
        PC_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        // Make the context of our window handle current to do our OpenGL drawing in
        glfwMakeContextCurrent(m_WindowHandle);
        // Initialize Glad to be able to call the OpenGL API functions
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        PC_CORE_ASSERT(status, "Failed to initialize Glad!");

        // Print out the OpenGL version info on the system
        PC_CORE_INFO("OpenGL Info: {0} {1}", (const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER));
        PC_CORE_INFO("  Vendor:   {0}", (const char*)glGetString(GL_VENDOR));
        PC_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
        PC_CORE_INFO("  Version:  {0}", (const char*)glGetString(GL_VERSION));

        // Make sure that we are using the correct OpenGL version required.
        // This is done to ensure that we do not have to support much older OpenGL functions that may complicate
        // the engine.
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
