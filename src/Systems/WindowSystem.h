#pragma once
#include "paperback_pch.h"

struct window_system : paperback::system::instance
{
    GLFWwindow* m_pWindow;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "window_system"
	};

	PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
		if (!glfwInit())
		{
            PPB_ERR_PRINT_N_LOG( "GLFW init has failed - Aborting" );
			std::exit(EXIT_FAILURE);
		}

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        m_pWindow = glfwCreateWindow( m_Engine.m_Width, m_Engine.m_Height, "Paperback Engine", NULL, NULL );

        if (!m_pWindow)
        {
            PPB_ERR_PRINT_N_LOG( "GLFW unable to create OpenGL context - Aborting" );
            glfwTerminate();
            std::exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(m_pWindow);
        glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        
        glfwSetKeyCallback(m_pWindow, KeyboardCallback);
        glfwSetMouseButtonCallback(m_pWindow, MouseCallback);

        // Init glew
        GLenum Err = glewInit();

        // Check that glew have no error
        if (GLEW_OK != Err)
            throw std::exception{ reinterpret_cast<const char*>(glewGetErrorString(Err)) };

        std::string Version = "Using GLEW Version: " + std::string( reinterpret_cast<const char*>(glewGetString(GLEW_VERSION)) );
        INFO_PRINT( Version );

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glViewport(0, 0, m_Engine.m_Width, m_Engine.m_Height);
	}

    PPB_FORCEINLINE
    void Update(void) noexcept
    {
        m_Coordinator.UpdateInputs();
        glfwPollEvents();

    }

    PPB_FORCEINLINE
    void Terminated(void) noexcept
    {
        glfwTerminate();
    }

    static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        (void)window;
        (void)scancode;
        (void)mods;
        PPB.SetKey(key, action);
    }

    static void MouseCallback(GLFWwindow* window, int key, int action, int mods)
    {
        (void)window;
        (void)mods;
        PPB.SetMouse(key, action);
    }
};