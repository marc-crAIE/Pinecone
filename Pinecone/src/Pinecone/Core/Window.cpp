#include "pcpch.h"
#include "Window.h"

#include "Pinecone/Events/ApplicationEvent.h"
#include "Pinecone/Events/KeyEvent.h"
#include "Pinecone/Events/MouseEvent.h"

#include <GLFW/glfw3.h>

namespace Pinecone
{
	// This is for if we want to create multiple windows. This will keep track of the amount of windows
	// created. It is required to make sure GLFW is initialized only once and terminated once there are
	// no more windows.
	static uint8_t s_GLFWWindowCount = 0;

	/// <summary>
	/// Print out a GLFW error code and description using the Pinecone logging system
	/// </summary>
	/// <param name="error">The GLFW error code (ref https://www.glfw.org/docs/latest/group__errors.html)</param>
	/// <param name="description">The description of the error</param>
	static void GLFWErrorCallback(int error, const char* description)
	{
		PC_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window::Window(const WindowProps& props)
	{
		PC_PROFILE_FUNCTION();

		Init(props);
	}

	Window::~Window()
	{
		PC_PROFILE_FUNCTION();

		Shutdown();
	}

	void Window::Init(const WindowProps& props)
	{
		PC_PROFILE_FUNCTION();

		// Store the window title, width and height into our window data
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		PC_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			PC_PROFILE_SCOPE("glfwInit");
			// If there are no windows already, initialize GLFW
			int success = glfwInit();
			PC_CORE_ASSERT(success, "Could not initialize GLFW!");
			// Set the GLFW error callback function 
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			PC_PROFILE_SCOPE("glfwCreateWindow");
			#ifdef PC_DEBUG
				// Let the GLFW window know that we are in debug mode
				glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
			#endif

			// Create the GLFW window using our window properties
			m_Window = glfwCreateWindow(props.Width, props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		// Create an OpenGL context for the window passing our new GLFW window handle
		m_Context = OpenGLContext::Create(m_Window);
		// Initialize the OpenGL context
		m_Context->Init();

		// Associate the window data with our GLFW window handle
		glfwSetWindowUserPointer(m_Window, &m_Data);
		// Enable v-sync by default
		SetVSync(true);

		// Setup the window resize event callback
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				// Get the window data
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				// Update the window data with the new width and height
				data.Width = width;
				data.Height = height;

				// Create a new WindowResizeEvent with the new width and height
				WindowResizeEvent event(width, height);
				// Run the callback function (typically Application::OnEvent)
				data.EventCallback(event);
			});

		// Setup the window close event callback
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				// Get the window data
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				// Create a new WindowClose Event and run the callback function (typically Application::OnEvent)
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		// Setup the window key event callback
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				// Get the window data
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					// Create a new KeyPressedEvent without repeating as the key is just being pressed
					KeyPressedEvent event(key, false);
					// Run the callback function (typically Application::OnEvent)
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					// Create a new KeyReleasedEvent
					KeyReleasedEvent event(key);
					// Run the callback function (typically Application::OnEvent)
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					// Create a new KeyPressedEvent with repeating
					KeyPressedEvent event(key, true);
					// Run the callback function (typically Application::OnEvent)
					data.EventCallback(event);
					break;
				}
				}
			});

		// Setup the window key typed event callback
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				// Get the window data
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				// Create a new KeyTypedEvent and run the callback function (typically Application::OnEvent)
				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});

		// Setup the window mouse event callback
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				// Get the window data
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					// Create a new MouseButtonPressedEvent and run the callback function (typically Application::OnEvent)
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					// Create a new MouseButtonReleasedEvent and run the callback function (typically Application::OnEvent)
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});

		// Setup the window mouse scroll event callback
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				// Get the window data
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				// Create a new MouseScrolledEvent with the scroll x and y amounts
				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				// Run the callback function (typically Application::OnEvent)
				data.EventCallback(event);
			});

		// Setup the window mouse move event callback
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				// Get the window data
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				// Create a new MouseMovedEvent with the new mouse x and y coordinates
				MouseMovedEvent event((float)xPos, (float)yPos);
				// Run the callback function (typically Application::OnEvent)
				data.EventCallback(event);
			});

		glfwSetDropCallback(m_Window, [](GLFWwindow* window, int pathCount, const char* paths[])
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				std::vector<std::filesystem::path> filepaths(pathCount);
				for (int i = 0; i < pathCount; i++)
					filepaths[i] = paths[i];

				WindowDropEvent event(std::move(filepaths));
				data.EventCallback(event);
			});
	}

	void Window::Shutdown()
	{
		PC_PROFILE_FUNCTION();

		// Destroy the GLFw widnow and remove 1 from the window count
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		// If there are no more GLFW windows, terminate GLFW
		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void Window::OnUpdate()
	{
		PC_PROFILE_FUNCTION();

		// Process GLFW events and swap the front and back buffers
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void Window::SetVSync(bool enabled)
	{
		PC_PROFILE_FUNCTION();

		// Enable or disable v-sync
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		// Set in the data
		m_Data.VSync = enabled;
	}

	void Window::SetResizable(bool resizable) const
	{
		glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
	}

	void Window::Maximize()
	{
		glfwMaximizeWindow(m_Window);
	}

	void Window::CenterWindow()
	{
		const GLFWvidmode* videmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int x = (videmode->width / 2) - (m_Data.Width / 2);
		int y = (videmode->height / 2) - (m_Data.Height / 2);
		glfwSetWindowPos(m_Window, x, y);
	}

	void Window::SetTitle(const std::string& title)
	{
		m_Data.Title = title;
		glfwSetWindowTitle(m_Window, m_Data.Title.c_str());
	}

	Scope<Window> Pinecone::Window::Create(const WindowProps& props)
	{
		return CreateScope<Window>(props);
	}
}
