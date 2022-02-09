#pragma once

#ifndef PPB_INPUT_ACTION
#define PPB_INPUT_ACTION


#define BEGIN_QUERY_VALID_INPUT_BINDINGS( Input_Binding_List, Return_List )                      \
    for ( size_t Pairing = 0, max = Input_Binding_List.size(); Pairing < max; ++Pairing )        \
    {                                                                                            \
        switch ( static_cast<paperback::input::action::KeyPairing>( Pairing ) )                  \
        {                                                                                        \
            case paperback::input::action::KeyPairing::PPB_DEFAULT_KEY:                          \
            {                                                                                    \
                for ( const auto& Bind : Input_Binding_List[Pairing] )                           \
                    if ( Bind != settings::invalid_index_v )                                     \
                        Return_List.push_back( Bind );                                           \
                break;                                                                           \
            }                                                                                    \
            case paperback::input::action::KeyPairing::PPB_SHIFT_KEY:                            \
            {                                                                                    \
                if ( PPB.IsKeyPressDown( GLFW_KEY_LEFT_SHIFT ) )                                 \
                {                                                                                \
                    for ( const auto& Bind : Input_Binding_List[Pairing] )                       \
                        if ( Bind != settings::invalid_index_v )                                 \
                            Return_List.push_back( Bind );                                       \
                }                                                                                \
                break;                                                                           \
            }                                                                                    \
            case paperback::input::action::KeyPairing::PPB_CTRL_KEY:                             \
            {                                                                                    \
                if ( PPB.IsKeyPressDown( GLFW_KEY_LEFT_CONTROL ) )                               \
                {                                                                                \
                    for ( const auto& Bind : Input_Binding_List[Pairing] )                       \
                        if ( Bind != settings::invalid_index_v )                                 \
                            Return_List.push_back( Bind );                                       \
                }                                                                                \
                break;                                                                           \
            }                                                                                    \
            case paperback::input::action::KeyPairing::PPB_ALT_KEY:                              \
            {                                                                                    \
                if ( PPB.IsKeyPressDown( GLFW_KEY_LEFT_ALT ) )                                   \
                {                                                                                \
                    for ( const auto& Bind : Input_Binding_List[Pairing] )                       \
                        if ( Bind != settings::invalid_index_v )                                 \
                            Return_List.push_back( Bind );                                       \
                }                                                                                \
                break;                                                                           \
            }                                                                                    \
            case paperback::input::action::KeyPairing::PPB_CMD_KEY:                              \
            {                                                                                    \
                break;                                                                           \
            }

#define END_QUERY_VALID_INPUT_BINDINGS                                                           \
        }                                                                                        \
    }


namespace paperback::input::action
{
    namespace type
    {
        enum class id : paperback::u8
        {
            // Value Action Type - Performs on every value change that is not the default value  |  e.g. paperback::Vector3f{ 0.0f, 0.0f, 0.0f }
            // Action begins on InputActionPhase.Started then performs InputActionPhase.Performed
            // When Value returns to the default value, InputActionPhase.Cancelled is triggered
            VALUE = 0

            // Button Action Type - Performs on EITHER Button Input OR when Interaction.Modifiers are cleared
        ,   BUTTON

            // Input Sink
        ,   INPUT_SINK
        };
    }


    //-----------------------------------
    //         Action Pairings
    //-----------------------------------

    enum class KeyPairing : paperback::u8
    {
        PPB_DEFAULT_KEY = 0
    ,   PPB_SHIFT_KEY
    ,   PPB_CTRL_KEY
    ,   PPB_ALT_KEY
    ,   PPB_CMD_KEY

    ,   PPB_KEY_MAX
    };

    enum class BroadcastStatus : paperback::u8
    {
        PRESSED = 0
    ,   CONTINUOUS
    ,   RELEASED
    };


    //-----------------------------------
    //           Input Action
    //-----------------------------------

    struct instance
    {
        struct Interaction
        {
            float  m_CurrentHoldTime  = 0.0f;
            float  m_HoldTime         = 0.0f;                  // Modifier
            bool   m_Hold             = false;                 // Non-Default Action
            bool   m_Default          = true;                  // Default = type::Action::BUTTON
        };


        using BindingGuids  = std::vector< paperback::u64 >;
        using ActionParings = std::array< /*paperback::u64*/ BindingGuids, static_cast<size_t>( KeyPairing::PPB_KEY_MAX ) >;

        PPB_INLINE
        instance( void ) noexcept;


        //-----------------------------------
        //             Getters
        //-----------------------------------

        PPB_INLINE
        BindingGuids FindBindings( const action::BroadcastStatus Status ) const noexcept;
        

        //-----------------------------------
        //             Setters
        //-----------------------------------

        PPB_INLINE
        void SetBinding( const paperback::u64          BindingGuidValue
                       , const action::BroadcastStatus Status
                       , const KeyPairing              Pairing = KeyPairing::PPB_DEFAULT_KEY ) noexcept;

        PPB_INLINE
        void SetBinding( const binding::type::guid&    BindingGuid
                       , const action::BroadcastStatus Status
                       , const KeyPairing              Pairing = KeyPairing::PPB_DEFAULT_KEY ) noexcept;


        //-----------------------------------
        //             Extras
        //-----------------------------------

        PPB_INLINE
        void ResetBinding( const action::BroadcastStatus Status
                         , const KeyPairing Pairing = KeyPairing::PPB_DEFAULT_KEY ) noexcept;

        PPB_INLINE
        void SetActionType( const action::type::id ActionType = action::type::id::BUTTON ) noexcept;


        //-----------------------------------
        //               Data
        //-----------------------------------

        Interaction                                                          m_ActionInteraction     {  };                             // Action Modifiers
        ActionParings                                                        m_PressedBindingGuids   {  };                             // Action to Binding Mapping - 0 Reserved For Default Action ( No Pairing ) - Initial Button Press
        ActionParings                                                        m_ContinuousBindingGuids{  };                             // Action to Binding Mapping - 0 Reserved For Default Action ( No Pairing ) - Held Down
        ActionParings                                                        m_ReleasedBindingGuids  {  };                             // Action to Binding Mapping - 0 Reserved For Default Action ( No Pairing ) - On Release
        tools::n_bits< static_cast<size_t>(KeyPairing::PPB_KEY_MAX) >        m_PairingModifier       {  };                             // Pairing Key ( Event Triggers on Key && KeyPairing )
        action::type::id                                                     m_Type                  = action::type::id::BUTTON;       // Action Type
    };
}

#endif