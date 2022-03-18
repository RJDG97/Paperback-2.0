#pragma once

#include <array>
#include "glm/inc/glm.hpp"
#include "glfw/inc/glfw3.h"


/****************************************************************************************
                               ** BINDING CALLBACK **

  Actions   |   E.g. m_KeyboardActions     
            |        m_MouseActions        
            |        m_GamepadAxisActions  
            |        m_GamepadButtonActions
            |
  Key       |   Key Event
            |   E.g. A Pressed, B Pressed, C Pressed, ...

****************************************************************************************/

#define BEGIN_BINDING_EVENT_CALLBACK( Actions, Key, Status )                                                                           \
    const auto& Action   = Actions[ Key ];                                                                                             \
    auto        Bindings = Action.FindBindings( Status );                                                                              \
                                                                                                                                       \
    for ( const auto& Bind : Bindings )                                                                                                \
    {                                                                                                                                  \
        auto BindIndex = FindBindingIndex( Bind );                                                                                     \
                                                                                                                                       \
        if ( BindIndex != settings::invalid_index_v && BindIndex < m_InputBindingEvents.size() )                                       \
        {                                                                                                                              \
            auto& BindingEvent = m_InputBindingEvents[ BindIndex ];
            // BindingEvent Callback

#define END_BINDING_EVENT_CALLBACK                                                                                                     \
        }                                                                                                                              \
        else                                                                                                                           \
        {                                                                                                                              \
            WARN_PRINT( "Attempting to execute invalid Binding Event" );                                                               \
        }                                                                                                                              \
    }


namespace paperback::input
{
	namespace device
	{
		struct Keyboard_Controls;
		struct Mouse_Controls;
		struct Gamepad_Controls;
	}

    class manager
    {
    public:

	    // Input Bindings
	    using BindingList          = std::vector< binding::InputBinding >;
        using BindingMap           = std::unordered_map< paperback::u64, paperback::u8 >;
        using BindingConstructList = std::vector< std::pair<const binding::type::info*, std::unique_ptr<binding::instance>> >;

	    // Input Bindings
        using ActionBindings         = std::vector< paperback::u64 >;                                 // Device Action Bindings
        using PlayerControlBindings  = std::tuple< device::type::id                                   // Player Device Type
                                                 , ActionBindings >;                                  // Action Bindings
                                                                                                  
        // Controller                                                                                 
        using DeviceMap              = std::unordered_map< paperback::u64, paperback::u8 >;           // Device Guid Mapped To Device List Index
        using DeviceList             = std::vector< std::pair< const device::type::info*              // Device Info
                                                             , std::unique_ptr<device::instance>      // Player Device Class ( Keyboard  |  Mouse  |  Gamepad  |  GamepadAxis )
                                                             >              
                                                  >;


	    //-----------------------------------
	    //           Input Events
	    //-----------------------------------

	    struct KeyPressed     : paperback::event::instance< const size_t&, const bool& > {};		// Held Down - Not Released ( Passes False )
	    struct KeyClicked     : paperback::event::instance< const size_t&, const bool& > {};		// Released                 ( Passes True )
        struct MousePressed   : paperback::event::instance< const size_t&, const bool& > {};		// Held Down - Not Released ( Passes False )
        struct MouseClicked   : paperback::event::instance< const size_t&, const bool& > {};		// Released                 ( Passes True )
		struct GamepadPressed : paperback::event::instance< const size_t&, const bool& > {};		// Held Down - Not Released ( Passes False )
        struct GamepadClicked : paperback::event::instance< const size_t&, const bool& > {};		// Released                 ( Passes True )


	    //-----------------------------------
	    //            Default
	    //-----------------------------------

	    PPB_INLINE
	    manager( paperback::coordinator::instance& Coordinator ) noexcept;

	    PPB_INLINE
	    ~manager( void ) noexcept
		{
			
		}

		PPB_INLINE
		void Terminate( void ) noexcept
		{
		}

	    PPB_INLINE
	    void UpdateInputs( void ) noexcept;


	    //-----------------------------------
        //      Controller Assignment
        //-----------------------------------

        template < typename T_DEVICE >
        T_DEVICE* RegisterDevice( void ) noexcept;

		//// To Be Used In External Intercept System
  //      PPB_INLINE
  //      PlayerControlBindings GetActiveDevice( const paperback::u64 ControllerGuid ) noexcept;

		PPB_INLINE
		device::Keyboard_Controls* FindKeyboard( void ) const noexcept;

		PPB_INLINE
		device::Mouse_Controls* FindMouse( void ) const noexcept;

		PPB_INLINE
		device::Gamepad_Controls* FindGamepad( void ) const noexcept;

		PPB_INLINE
		void ResetDevices( void ) noexcept;


        //-----------------------------------
        //         Input Bindings
        //-----------------------------------

	    template < typename T_BINDING_CONSTRUCT >
        paperback::u64 RegisterBinding( void ) noexcept;


        PPB_INLINE
        void AssignBindingToAction( const binding::type::guid&         BindingGuid
                                  , const paperback::u32               Key
                                  , const device::type::id             Type
                                  , const action::BroadcastStatus      Status
                                  , const action::KeyPairing           Pairing = action::KeyPairing::PPB_DEFAULT_KEY ) noexcept;

		PPB_INLINE
        void AssignBindingToAction( const paperback::u64&              BindingGuidValue
                                  , const paperback::u32               Key
                                  , const device::type::id             Type
                                  , const action::BroadcastStatus      Status
                                  , const action::KeyPairing           Pairing = action::KeyPairing::PPB_DEFAULT_KEY ) noexcept;


        template < typename... T_ARGS >
        void BroadcastAction( const paperback::u32           Key
                            , const device::type::id         Type
                            , const action::BroadcastStatus  Status
                            , T_ARGS&&...                    Args ) noexcept;


	    //-----------------------------------
	    //            Keyboard
	    //-----------------------------------
	
	    PPB_INLINE
	    bool IsKeyPress( int Key ) const noexcept;

	    PPB_INLINE
	    bool IsKeyPressDown( int Key ) const noexcept;

	    PPB_INLINE
	    bool IsKeyPressUp( int Key ) const noexcept;


		//-----------------------------------
	    //            Gamepad
	    //-----------------------------------
	
	    PPB_INLINE
	    bool IsGamepadButtonPress( int Key ) const noexcept;

	    PPB_INLINE
	    bool IsGamepadButtonPressDown( int Key ) const noexcept;

	    PPB_INLINE
	    bool IsGamepadButtonPressUp( int Key ) const noexcept;


	    //-----------------------------------
	    //              Mouse
	    //-----------------------------------
	
	    PPB_INLINE
	    bool IsMousePress( int Key ) const noexcept;

	    PPB_INLINE
	    bool IsMouseDown( int Key ) const noexcept;

	    PPB_INLINE
	    bool IsMouseUp( int Key ) const noexcept;

	    PPB_INLINE
	    bool IsScrollUp() noexcept;

	    PPB_INLINE
	    bool IsScrollDown() noexcept;

	
	    //-----------------------------------
	    //        Mouse Coordinates
	    //-----------------------------------

	    PPB_INLINE
	    glm::vec2 GetMouseDirection() const noexcept;

	    PPB_INLINE // Screen Position
	    glm::vec2 Get2DMousePosition() const noexcept;

	    PPB_INLINE
	    glm::vec3 GetMousePosition() const noexcept;

		PPB_INLINE
		glm::vec3 GetViewportMousePosition(glm::mat4 projection, glm::mat4 view) const noexcept;

	    PPB_INLINE
	    glm::vec3 GetViewportMousePosition( glm::vec2 viewport_min
		                                  , glm::vec2 viewport_max ) const noexcept;

	    //-----------------------------------
	    //             Setters
	    //-----------------------------------

	    PPB_INLINE
	    void SetKey( int Key
		           , int Action ) noexcept;

	    PPB_INLINE
	    void SetMouse( int Key
		             , int Action ) noexcept;

	    PPB_INLINE
	    void SetScroll( double Y ) noexcept;


    private:


        //-----------------------------------
        //        Helper Functions
        //-----------------------------------

        PPB_INLINE
        paperback::u32 FindBindingIndex( const paperback::u64& BindingGuidValue ) const noexcept
        {
            auto it = m_BindingMap.find( BindingGuidValue );

            if ( it != m_BindingMap.end() )
                return it->second;
            return settings::invalid_index_v;
        }

        PPB_INLINE
        paperback::u32 FindBindingIndex( const binding::type::guid& BindingGuid ) const noexcept
        {
            return FindBindingIndex( BindingGuid.m_Value );
        }


		// Devices
	    DeviceMap                            m_PlayerDeviceMap;                      // Device Guid Mapped To Controller List Index  |  key = 0  |  mouse = 1
        DeviceList                           m_PlayerDeviceList;                     // List Of Devices

		// Input Bindings
        BindingMap                           m_BindingMap;                           // Maps Binding Script Guid to Binding List Index
        BindingList                          m_InputBindingEvents;                   // Stores Binding Action Callbacks - Actual Execution
        BindingConstructList                 m_BindingConstructs;                    // Stores Actual Binding Action

		// Device Ptrs
		device::Keyboard_Controls*           m_Keyboard = nullptr;
		device::Mouse_Controls*              m_Mouse = nullptr;
		device::Gamepad_Controls*            m_Gamepad = nullptr;

	    // Reference to Coordinator
	    paperback::coordinator::instance& m_Coordinator;
    };
}