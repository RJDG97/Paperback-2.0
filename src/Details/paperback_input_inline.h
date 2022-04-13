/**********************************************************************************
*\file         paperback_input_inline.h
*\brief        paperback_input_inline.h
*
*\author	   Lee Jun Pu, 50% Code Contribution
*\author	   Malcolm, 50% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "../paperback_camera.h"
#include "../../build/Sandbox/Systems/WindowSystem.h"

namespace paperback::input
{
	manager::manager( paperback::coordinator::instance& Coordinator) noexcept :
		m_Coordinator{ Coordinator }
	{
		// Register Button Events
		m_Coordinator.RegisterEvent<KeyPressed>();
		m_Coordinator.RegisterEvent<MousePressed>();

		// Register Devices
		m_Keyboard = RegisterDevice< device::Keyboard_Controls >( );
		m_Mouse = RegisterDevice< device::Mouse_Controls >( );
		m_Gamepad = RegisterDevice< device::Gamepad_Controls >( );

		// Assert If Registration Fails
		PPB_ASSERT_MSG( !m_Keyboard, "Keyboard Registration Failed!!" );
		PPB_ASSERT_MSG( !m_Mouse,    "Mouse Registration Failed!!" );
		PPB_ASSERT_MSG( !m_Gamepad,  "Gamepad Registration Failed!!" );
	}


	//-----------------------------------
    //      Controller Assignment
    //-----------------------------------

    template < typename T_DEVICE >
    T_DEVICE* manager::RegisterDevice( void ) noexcept
    {
        m_PlayerDeviceList.push_back({ &device::info_v<T_DEVICE>
                                     , std::make_unique<T_DEVICE>( ) });

        PPB_ASSERT_MSG( m_PlayerDeviceList.size() == 0
                      , "Player Device List Push Back Error!" );

        m_PlayerDeviceMap.emplace( device::info_v<T_DEVICE>.m_Guid.m_Value
                                 , m_PlayerDeviceList.size() - 1 );

		#ifdef PAPERBACK_DEBUG
		std::cout << "Device ( "
			      << device::info_v<T_DEVICE>.m_pName
			      << " ): Registered Successfully"
			      << std::endl;

	    std::cout << device::info_v<T_DEVICE>.m_pName
		          << " Guid: "
		          << device::info_v<T_DEVICE>.m_Guid.m_Value
		          << std::endl;
		#endif

		return std::bit_cast<T_DEVICE*>( m_PlayerDeviceList.back().second.get() );
    }

	PPB_INLINE
	input::device::Keyboard_Controls* manager::FindKeyboard( void ) const noexcept
	{
		return m_Keyboard;
	}

	PPB_INLINE
	input::device::Mouse_Controls* manager::FindMouse( void ) const noexcept
	{
		return m_Mouse;
	}

	PPB_INLINE
	input::device::Gamepad_Controls* manager::FindGamepad( void ) const noexcept
	{
		return m_Gamepad;
	}

	PPB_INLINE
	void manager::ResetDevices( void ) noexcept
	{
		if ( m_Keyboard ) m_Keyboard->Reset();
		if ( m_Mouse )    m_Mouse->Reset();
		if ( m_Gamepad )  m_Gamepad->Reset();
	}


    //-----------------------------------
    //         Input Bindings
    //-----------------------------------

	template < typename T_BINDING_CONSTRUCT >
    paperback::u64 manager::RegisterBinding( void ) noexcept
    {
		m_BindingConstructs.push_back({ &binding::info_v<T_BINDING_CONSTRUCT>
                                      , std::make_unique<T_BINDING_CONSTRUCT>( m_Coordinator ) });

        PPB_ASSERT_MSG( m_BindingConstructs.size() == 0
                      , "Binding Construct Push Back Error!" );

        m_BindingMap.emplace( binding::info_v<T_BINDING_CONSTRUCT>.m_Guid.m_Value
                            , m_BindingConstructs.size() - 1 );

		m_InputBindingEvents.push_back( binding::InputBinding
										{
											.m_pAction  = []( void* pBindingConstruct, const float& Dt ) constexpr noexcept
														  {
															  std::invoke( &T_BINDING_CONSTRUCT::OnInputAction
																		 , reinterpret_cast<T_BINDING_CONSTRUCT*>( pBindingConstruct )
																		 , std::forward<const float&>( Dt ) );
														  }
										,   .m_pClass   = m_BindingConstructs.back().second.get()
										} );

		//std::cout << binding::info_v<T_BINDING_CONSTRUCT>.m_pName
		//          << " Guid: "
		//          << binding::info_v<T_BINDING_CONSTRUCT>.m_Guid.m_Value
		//          << std::endl;

		return binding::info_v<T_BINDING_CONSTRUCT>.m_Guid.m_Value;
    }


    PPB_INLINE
    void manager::AssignBindingToAction( const binding::type::guid&       BindingGuid
                                       , const paperback::u32             Key
                                       , const device::type::id           Type
                                       , const action::BroadcastStatus    Status
                                       , const action::KeyPairing         Pairing ) noexcept
    {
		AssignBindingToAction( BindingGuid.m_Value
			                 , Key
			                 , Type
					         , Status
			                 , Pairing);
    }

	PPB_INLINE
    void manager::AssignBindingToAction( const paperback::u64&            BindingGuidValue
                                       , const paperback::u32             Key
                                       , const device::type::id           Type
                                       , const action::BroadcastStatus    Status
                                       , const action::KeyPairing         Pairing ) noexcept
    {
		switch ( Type )
        {
            case device::type::id::KEYBOARD:
            {
				if ( !m_Keyboard ) return;

				m_Keyboard->m_State.m_Actions[ Key ].SetBinding( BindingGuidValue
					                                           , Status
                                                               , Pairing );

                break;
            }
            case device::type::id::MOUSE:
            {
				if ( !m_Mouse ) return;

                m_Mouse->m_State.m_Actions[ Key ].SetBinding( BindingGuidValue
					                                        , Status
                                                            , Pairing );

                break;
            }
            case device::type::id::GAMEPAD:
            {
				if ( !m_Gamepad ) return;

                m_Gamepad->m_State.m_Actions[ Key ].SetBinding( BindingGuidValue
					                                          , Status
                                                              , Pairing );

                break;
            }
        }
	}

    template < typename... T_ARGS >
    void manager::BroadcastAction( const paperback::u32           Key
                                 , const device::type::id         Type
                                 , const action::BroadcastStatus  Status
                                 , T_ARGS&&...                    Args ) noexcept
    {
        switch ( Type )
        {
            case device::type::id::KEYBOARD:
            {
				if ( !m_Keyboard ) return;

                BEGIN_BINDING_EVENT_CALLBACK( m_Keyboard->m_State.m_Actions, Key, Status )
                    BindingEvent.m_pAction( BindingEvent.m_pClass
						                  , std::forward<T_ARGS&&>( Args )... );
                END_BINDING_EVENT_CALLBACK

                break;
            }
            case device::type::id::MOUSE:
            {
				if ( !m_Mouse ) return;

                BEGIN_BINDING_EVENT_CALLBACK( m_Mouse->m_State.m_Actions, Key, Status )
                    BindingEvent.m_pAction( BindingEvent.m_pClass
                                          , std::forward<T_ARGS&&>( Args )... );
                END_BINDING_EVENT_CALLBACK

                break;
            }
            case device::type::id::GAMEPAD:
            {
				if ( !m_Gamepad ) return;

                BEGIN_BINDING_EVENT_CALLBACK( m_Gamepad->m_State.m_Actions, Key, Status )
                    BindingEvent.m_pAction( BindingEvent.m_pClass
                                          , std::forward<T_ARGS&&>( Args )... );
                END_BINDING_EVENT_CALLBACK

                break;
            }
        }
    }


	//-----------------------------------
	//              Keys
	//-----------------------------------

	void manager::SetKey(int Key, int Action) noexcept
	{
		if ( m_Keyboard )
		{
			m_Keyboard->m_State.m_Current[ Key ] = !!Action;
		}
	}

	bool manager::IsKeyPress(int Key) const noexcept
	{
		if ( m_Keyboard )
		{
			auto KeyCode = m_Keyboard->m_State;
			return KeyCode.m_Current[Key] == GLFW_PRESS && 
				   KeyCode.m_Previous[Key] != GLFW_PRESS;
		}
		return false;
	}

	bool manager::IsKeyPressDown(int Key) const noexcept
	{
		if ( m_Keyboard )
		{
			auto KeyCode = m_Keyboard->m_State;
			return ( KeyCode.m_Current[Key] == GLFW_PRESS && 
				     KeyCode.m_Previous[Key] == GLFW_PRESS );
		}
		return false;
	}

	bool manager::IsKeyPressUp(int Key) const noexcept
	{
		if ( m_Keyboard )
		{
			auto KeyCode = m_Keyboard->m_State;
			return KeyCode.m_Current[Key] == GLFW_RELEASE && 
				   KeyCode.m_Previous[Key] != KeyCode.m_Current[Key];
		}
		return false;
	}


	//-----------------------------------
	//            Gamepad
	//-----------------------------------

	bool manager::IsGamepadButtonPress(int Key) const noexcept
	{
		if ( m_Gamepad )
		{
			auto GamepadCode = m_Gamepad->m_State;
			return GamepadCode.m_Current[Key] == GLFW_PRESS &&
				   GamepadCode.m_Previous[Key] != GLFW_PRESS;
		}
		return false;
	}

	bool manager::IsGamepadButtonPressDown(int Key) const noexcept
	{
		if ( m_Gamepad )
		{
			auto GamepadCode = m_Gamepad->m_State;
			return ( GamepadCode.m_Current[Key] == GLFW_PRESS && 
				     GamepadCode.m_Previous[Key] == GLFW_PRESS );
		}
		return false;
	}

	bool manager::IsGamepadButtonPressUp(int Key) const noexcept
	{
		if ( m_Gamepad )
		{
			auto GamepadCode = m_Keyboard->m_State;
			return GamepadCode.m_Current[Key] == GLFW_RELEASE && 
				   GamepadCode.m_Previous[Key] != GamepadCode.m_Current[Key];
		}
		return false;
	}


	//-----------------------------------
	//              Mouse
	//-----------------------------------

	void manager::SetMouse(int Key, int Action) noexcept
	{
		if ( m_Mouse )
		{
			m_Mouse->m_State.m_Current[ Key ] = !!Action;
		}
	}

	void manager::SetScroll(double Y) noexcept
	{
		if ( m_Mouse )
		{
			m_Mouse->m_State.m_YAxis = Y;
		}
	}

	bool manager::IsMousePress(int Key) const noexcept
	{
		if ( m_Mouse )
		{
			auto MouseCode = m_Mouse->m_State;
			return MouseCode.m_Current[Key] == GLFW_PRESS &&
				   MouseCode.m_Previous[Key] != MouseCode.m_Current[Key];
		}
		return false;
	}

	bool manager::IsMouseDown(int Key) const noexcept
	{
		if ( m_Mouse )
		{
			auto MouseCode = m_Mouse->m_State;
			return MouseCode.m_Current[Key] == GLFW_PRESS;
		}
		return false;
	}

	bool manager::IsMouseUp(int Key) const noexcept
	{
		if ( m_Mouse )
		{
			auto MouseCode = m_Mouse->m_State;
			return MouseCode.m_Current[Key] == GLFW_RELEASE && MouseCode.m_Previous[Key] == GLFW_PRESS;
		}
		return false;
	}

	bool manager::IsScrollUp() noexcept
	{
		if ( m_Mouse )
		{
			if ( m_Mouse->m_State.m_YAxis < 0.0 )
			{
				m_Mouse->m_State.m_YAxis = 0.0;
				return true;
			}
		}
		return false;
	}

	bool manager::IsScrollDown() noexcept
	{
		if ( m_Mouse )
		{
			if ( m_Mouse->m_State.m_YAxis > 0.0 )
			{
				m_Mouse->m_State.m_YAxis = 0.0;
				return true;
			}
		}
		return false;
	}

	glm::vec2 manager::GetMouseDirection() const noexcept
	{
		if ( m_Mouse )
		{
			return m_Mouse->m_State.m_CurrPos - m_Mouse->m_State.m_PrevPos;
		}
		return glm::vec2{};
	}

	glm::vec2 manager::Get2DMousePosition() const noexcept
	{
		GLFWwindow* m_pWindow = PPB.GetSystem< window_system >().m_pWindow;
		auto& WindowDetails = PPB.GetSystem< window_system >().E;

		// Get x and y values
		double X, Y;
		glfwGetCursorPos(m_pWindow, &X, &Y);

		// Find point in NDC
		glm::vec2 NDCPoint = glm::vec2{ (2.f * X) / WindowDetails.m_Width - 1.f, 1.f - (2.f * Y) / WindowDetails.m_Height };

		return NDCPoint;
	}

	glm::vec3 manager::GetMousePosition() const noexcept
	{
		GLFWwindow* m_pWindow = PPB.GetSystem< window_system >().m_pWindow;
		auto& WindowDetails = PPB.GetSystem< window_system >().E;

		// Get x and y values
		double X, Y;
		glfwGetCursorPos(m_pWindow, &X, &Y);

		// Find point in NDC
		glm::vec4 NDCPoint = glm::vec4{ (2.f * X) / WindowDetails.m_Width - 1.f, 1.f - (2.f * Y) / WindowDetails.m_Height, -1.f, 1.f };
		// Find point in view
		glm::vec4 ViewPoint = glm::inverse(cam::GetInstanced().GetProjection()) * NDCPoint;
		ViewPoint = glm::vec4{ ViewPoint.x, ViewPoint.y, -1.f, 0.f };
		// Find point world
		glm::vec3 WorldPoint = glm::inverse(cam::GetInstanced().GetView()) * ViewPoint;

		return WorldPoint;
	}

	glm::vec3 manager::GetViewportMousePosition(glm::mat4 projection, glm::mat4 view) const noexcept
	{
		window_system* Window_System{ &PPB.GetSystem< window_system >() };
		GLFWwindow* m_pWindow = Window_System->m_pWindow;
		auto& WindowDetails = Window_System->E;

		// Get x and y values
		double X{ static_cast<double>(Window_System->m_Width/2.0f) };
		double Y{ static_cast<double>(Window_System->m_Height/2.0f) };
		glm::vec2 viewport_min{ 0.0f, 0.0f };
		glm::vec2 viewport_max{ Window_System->m_Width, Window_System->m_Height };

		if (X < viewport_min.x || X > viewport_max.x || Y < viewport_min.y || Y > viewport_max.y)
		{
			return {};
		}

		// Find point in NDC
		glm::vec4 NDCPoint = glm::vec4{ (2.f * (X - viewport_min.x)) / (viewport_max.x - viewport_min.x) - 1.f, 1.f - (2.f * (Y - viewport_min.y)) / (viewport_max.y - viewport_min.y), -1.f, 1.f };
		// Find point in view
		glm::vec4 ViewPoint = glm::inverse(projection) * NDCPoint;
		ViewPoint = glm::vec4{ ViewPoint.x, ViewPoint.y, -1.f, 0.f };
		// Find point world
		glm::vec3 WorldPoint = glm::inverse(view) * ViewPoint;

		return WorldPoint;
	}

	glm::vec3 manager::GetViewportMousePosition(glm::vec2 viewport_min, glm::vec2 viewport_max) const noexcept
	{
		window_system* Window_System{ &PPB.GetSystem< window_system >() };
		GLFWwindow* m_pWindow = Window_System->m_pWindow;
		auto& WindowDetails = Window_System->E;

		// Get x and y values
		double X, Y;
		glfwGetCursorPos(m_pWindow, &X, &Y);

		if (X < viewport_min.x || X > viewport_max.x || Y < viewport_min.y || Y > viewport_max.y)
		{
			return {};
		}

		// Find point in NDC
		glm::vec4 NDCPoint = glm::vec4{ (2.f * (X- viewport_min.x)) / (viewport_max.x - viewport_min.x) - 1.f, 1.f - (2.f * (Y- viewport_min.y)) / (viewport_max.y - viewport_min.y), -1.f, 1.f };
		// Find point in view
		glm::vec4 ViewPoint = glm::inverse(cam::GetInstanced().GetProjection()) * NDCPoint;
		ViewPoint = glm::vec4{ ViewPoint.x, ViewPoint.y, -1.f, 0.f };
		// Find point world
		glm::vec3 WorldPoint = glm::inverse(cam::GetInstanced().GetView()) * ViewPoint;

		return WorldPoint;
	}

	//-----------------------------------
	//           Post Update
	//-----------------------------------

	void manager::UpdateInputs( void ) noexcept
	{
		auto DeltaTime = m_Coordinator.DeltaTime();

		if ( m_Keyboard )
		{
			auto& KeyState = m_Keyboard->m_State;

			for ( size_t i = 0, max = KeyState.m_Current.size(); i < max; ++i )
			{
				// Button Initial Press
				if ( KeyState.m_Current[i] == GLFW_PRESS && KeyState.m_Previous[i] == GLFW_RELEASE )
				{
					m_Coordinator.BroadcastEvent<KeyClicked>( i, true );

					// Action Binding Callback
					BroadcastAction( static_cast<const paperback::u32>( i )
						           , device::type::id::KEYBOARD
						           , action::BroadcastStatus::PRESSED
						           , DeltaTime );
				}

				// Button Held Down
				else if ( KeyState.m_Current[i] == GLFW_PRESS && KeyState.m_Previous[i] == GLFW_PRESS )
				{
					// Broadcast Key Event
					m_Coordinator.BroadcastEvent<KeyPressed>( i
						                                    , false );

					// Action Binding Callback
					BroadcastAction( static_cast<const paperback::u32>( i )
						           , device::type::id::KEYBOARD
						           , action::BroadcastStatus::CONTINUOUS
						           , DeltaTime );
				}

				// Button Released
				else if ( KeyState.m_Current[i] == GLFW_RELEASE && KeyState.m_Previous[i] == GLFW_PRESS )
				{
					// Action Binding Callback
					BroadcastAction( static_cast<const paperback::u32>( i )
						           , device::type::id::KEYBOARD
						           , action::BroadcastStatus::RELEASED
						           , DeltaTime );
				}

				KeyState.m_Previous[i] = KeyState.m_Current[i];
			}
		}

		if ( m_Mouse )
		{
			auto& MouseState = m_Mouse->m_State;

			// Update Mouse Coordinates
			MouseState.UpdateMouseCoordinates();

			for ( size_t i = 0, max = MouseState.m_Current.size(); i < max; ++i )
			{
				// Mouse Initial Press
				if ( MouseState.m_Current[i] == GLFW_PRESS && MouseState.m_Previous[i] == GLFW_RELEASE )
				{
					// Action Binding Callback
					BroadcastAction( static_cast<const paperback::u32>( i )
						           , device::type::id::MOUSE
						           , action::BroadcastStatus::PRESSED
						           , DeltaTime );
				}

				// Mouse Button Held Down
				else if ( MouseState.m_Current[i] == GLFW_PRESS && MouseState.m_Previous[i] == GLFW_PRESS )
				{
					m_Coordinator.BroadcastEvent<MousePressed>( i, false );

					// Action Binding Callback
					BroadcastAction( static_cast<const paperback::u32>( i )
						           , device::type::id::MOUSE
						           , action::BroadcastStatus::CONTINUOUS
						           , DeltaTime );
				}

				// Mouse Button Released
				else if ( MouseState.m_Current[i] == GLFW_RELEASE && MouseState.m_Previous[i] == GLFW_PRESS )
				{
					m_Coordinator.BroadcastEvent<MouseClicked>( i, true );

					// Action Binding Callback
					BroadcastAction( static_cast<const paperback::u32>( i )
						           , device::type::id::MOUSE
						           , action::BroadcastStatus::RELEASED
						           , DeltaTime );
				}
				
				MouseState.m_Previous[i] = MouseState.m_Current[i];
			}
		}

		if ( m_Gamepad )
		{
			GLFWgamepadstate GP_State;
			auto& GamepadState = m_Gamepad->m_State;

			// Valid Device Connected  AND  Device Is Gamepad
			if ( GamepadState.m_ControllerID != settings::invalid_controller_v && 
				 glfwJoystickIsGamepad( GamepadState.m_ControllerID ) )
			{
				// Get Gamepad State & Update
				if ( glfwGetGamepadState( GamepadState.m_ControllerID, &GP_State ) )
				{
					GamepadState.UpdateGamepadCurrentButtons( GP_State.buttons );
					GamepadState.UpdateGamepadAxis( GP_State.axes );
				}

				for ( size_t i = 0, max = GamepadState.m_Current.size(); i < max; ++i )
				{
					// Gamepad Initial Press
					if ( GamepadState.m_Current[i] == GLFW_PRESS && GamepadState.m_Previous[i] == GLFW_RELEASE )
					{
						// Action Binding Callback
						BroadcastAction( static_cast<const paperback::u32>( i )
									   , device::type::id::GAMEPAD
									   , action::BroadcastStatus::PRESSED
									   , DeltaTime );
					}

					// Gamepad Button Held Down
					else if ( GamepadState.m_Current[i] == GLFW_PRESS && GamepadState.m_Previous[i] == GLFW_PRESS )
					{
						// Action Binding Callback
						BroadcastAction( static_cast<const paperback::u32>( i )
									   , device::type::id::GAMEPAD
									   , action::BroadcastStatus::CONTINUOUS
									   , DeltaTime );
					}

					// Gamepad Buttoon Released
					else if ( GamepadState.m_Current[i] == GLFW_RELEASE && GamepadState.m_Previous[i] == GLFW_PRESS )
					{
						// Action Binding Callback
						BroadcastAction( static_cast<const paperback::u32>( i )
									   , device::type::id::GAMEPAD
									   , action::BroadcastStatus::RELEASED
									   , DeltaTime );
					}
				
					GamepadState.m_Previous[i] = GamepadState.m_Current[i];
				}
			}
		}
	}
}