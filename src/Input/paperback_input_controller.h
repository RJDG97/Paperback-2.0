#pragma once

#ifndef PPB_INPUT_CONTROLLER
#define PPB_INPUT_CONTROLLER


namespace paperback::input::device
{
    //-----------------------------------
    //          Device Types
    //-----------------------------------

    namespace type
    {
        using guid = xcore::guid::unit<64, struct device_tag>;


        //-----------------------------------
        //            ID Types
        //-----------------------------------

        enum class id : paperback::u8
        {
            KEYBOARD = 0
        ,   MOUSE
        ,   GAMEPAD
        ,   MAX_DEVICES
        };


        //-----------------------------------
        //          Typedef Types
        //-----------------------------------

        struct keyboard
		{
			static constexpr auto        id_v = id::KEYBOARD;
            device::type::guid           m_Guid{ };
            const char*                  m_pName{ };
		};

        struct mouse
		{
			static constexpr auto        id_v = id::MOUSE;
            device::type::guid           m_Guid{ };
            const char*                  m_pName{ };
		};

        struct gamepad
		{
			static constexpr auto        id_v = id::GAMEPAD;
            device::type::guid           m_Guid{ };
            const char*                  m_pName{ };
		};


        //-----------------------------------
        //              Info
        //-----------------------------------

        struct info
		{
			const type::guid             m_Guid;
            const type::id               m_TypeID;
			const char*					 m_pName{ };
		};
    }


    //-----------------------------------
    //      Create Controller Info
    //-----------------------------------

    namespace details
    {
        template< typename T_DEVICE >
        consteval device::type::info CreateInfo( void ) noexcept
        {
            return device::type::info
            {
                .m_Guid          = T_DEVICE::typedef_v.m_Guid.isValid()
                                   ? T_DEVICE::typedef_v.m_Guid
                                   : type::guid{ __FUNCSIG__ }
            ,   .m_TypeID        = T_DEVICE::typedef_v.id_v
			,	.m_pName		 = T_DEVICE::typedef_v.m_pName
            };
        }

        template< typename T_DEVICE >
        static constexpr auto info_v = device::details::CreateInfo< T_DEVICE >();
    }

    template< typename T_DEVICE >
    constexpr auto& info_v = device::details::info_v< std::decay_t< T_DEVICE > >;


    //-----------------------------------
    //         Device Interface
    //-----------------------------------

    struct instance
    {
        enum class DeviceState : paperback::u8
        {
            CONNECTED = 0
        ,   DISCONNECTED
        };

        PPB_INLINE
        instance( DeviceState State = DeviceState::CONNECTED ) noexcept :
            m_State{ State }
        {
        }

        PPB_INLINE
        virtual void Update( const float Dt ) noexcept
        { }

        PPB_INLINE
        virtual ~instance( void ) = default;


        DeviceState        m_State   = DeviceState::CONNECTED;
    };


    /****************************************************************************************
                            ** PLAYER CONTROLLER CREATION MACRO **

      player_device_name   |   E.g. Keyboard_Controls
                           |        OR
                           |        Mouse_Controls
                           |        OR
                           |        Gamepad_Controls
                           |   
      control_type         |   E.g. device::type::keyboard
                           |        OR
                           |        device::type::mouse
                           |        OR
                           |        device::type::gamepad

    ****************************************************************************************/

    #define BEGIN_CONTROLLER_CONSTRUCT( player_device_name, control_type )                                              \
        struct player_device_name : device::instance                                                                    \
        {                                                                                                               \
            using ActionBindings = action::instance::BindingGuids;                                                      \
                                                                                                                        \
            static constexpr auto typedef_v = control_type                                                              \
                                              {                                                                         \
                                                  .m_pName  = #player_device_name                                       \
                                              };                                                                        \
                                                                                                                        \
                                                                                                                        \
            PPB_INLINE                                                                                                  \
            player_device_name( instance::DeviceState State = instance::DeviceState::CONNECTED ) :                      \
                device::instance{ State }                                                                               \
            { }                                                                                                         \
                                                                                                                        \
            PPB_INLINE                                                                                                  \
            void AddBinding( const int                     Key                                                          \
                           , const paperback::u64          Binding                                                      \
                           , const action::BroadcastStatus Status                                                       \
                           , const action::KeyPairing      Pairing = action::KeyPairing::PPB_DEFAULT_KEY ) noexcept     \
            {                                                                                                           \
                if ( Key > m_State.m_Actions.size() || Key < 0 )                                                        \
                    WARN_PRINT( "Control Device: AddBinding invalid Index" );                                           \
                else                                                                                                    \
                    m_State.m_Actions[ Key ].SetBinding( Binding, Status, Pairing );                                    \
            }                                                                                                           \
                                                                                                                        \
            PPB_INLINE                                                                                                  \
            void AddBinding( const int                  Key                                                             \
                           , const binding::type::guid& Guid                                                            \
                           , const action::BroadcastStatus Status                                                       \
                           , const action::KeyPairing   Pairing = action::KeyPairing::PPB_DEFAULT_KEY ) noexcept        \
            {                                                                                                           \
                AddBinding( Key, Guid.m_Value, Status, Pairing );                                                       \
            }                                                                                                           \
                                                                                                                        \
            PPB_INLINE                                                                                                  \
            void ResetBinding( const paperback::u32     Key                                                             \
                             , const action::BroadcastStatus Status                                                     \
                             , const action::KeyPairing Pairing = action::KeyPairing::PPB_DEFAULT_KEY ) noexcept        \
            {                                                                                                           \
                if ( Key > m_State.m_Actions.size() || Key < 0 )                                                        \
                    WARN_PRINT( "Control Device: ResetBinding invalid Index" );                                         \
                else                                                                                                    \
                    m_State.m_Actions[ Key ].ResetBinding( Status, Pairing );                                           \
            }                                                                                                           \
                                                                                                                        \
            PPB_INLINE                                                                                                  \
            ActionBindings FindBindings( const paperback::u32 Key                                                       \
                                       , const action::BroadcastStatus Status ) noexcept                                \
            {                                                                                                           \
                return m_State.m_Actions[ Key ].FindBindings( Status );                                                 \
            }                                                                                                           \

    #define END_CONTROLLER_CONSTRUCT                                                                                    \
            bool     m_Active = true;                                                                                   \
        };
}

#endif