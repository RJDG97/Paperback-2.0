#pragma once

//#include "paperback_pch.h"
#include "Json/paperback_json.h"

struct window_system : paperback::system::instance
{
    bool m_FullScreen;
    bool m_UpdateResolution;
    int m_Width;
    int m_Height;

    GLFWwindow* m_pWindow;
    paperback::JsonFile JFile;

    engine E;
	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "window_system"
	};

	PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
        m_FullScreen = false;
        m_UpdateResolution = false;

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
        glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        JFile.StartReader("../../resources/assetloading/config.json").LoadObjects(E).EndReader();

        m_pWindow = glfwCreateWindow( E.m_Width, E.m_Height, E.m_WinName.c_str(), NULL, NULL );

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
        PPB_ASSERT_MSG( GLEW_OK != Err, reinterpret_cast<const char*>(glewGetErrorString(Err)) );

        std::string Version = "Using GLEW Version: " + std::string( reinterpret_cast<const char*>(glewGetString(GLEW_VERSION)) );
        INFO_PRINT( Version );

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glViewport(0, 0, E.m_Width, E.m_Height);
	}

    PPB_FORCEINLINE
    void Update( void ) noexcept
    {
        //if (m_Coordinator.IsKeyPress(GLFW_KEY_F))
        //{
        //    FullScreen();
        //}

        m_Coordinator.UpdateInputs();
        glfwPollEvents();
    }

    PPB_FORCEINLINE
    void OnSystemTerminated( void ) noexcept 
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

    //void FullScreen()
    //{
    //    if (!m_FullScreen)
    //    {
    //        //Temp values
    //        m_Width = 1920;
    //        m_Height = 1080;
    //        glfwSetWindowMonitor(m_pWindow, glfwGetPrimaryMonitor(), 0, 0, m_Width, m_Height, GLFW_DONT_CARE);
    //    }
    //    else
    //    {
    //        m_Width = E.m_Width;
    //        m_Height = E.m_Height;
    //        glfwSetWindowMonitor(m_pWindow, NULL, 250, 250, E.m_Width, E.m_Height, GLFW_DONT_CARE);
    //    }

    //    m_FullScreen = !m_FullScreen;
    //    m_UpdateResolution = true;
    //}
};