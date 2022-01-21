#pragma once

#ifndef PPB_INPUT_ACTION
#define PPB_INPUT_ACTION

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
        using ActionParings = std::array< paperback::u64, static_cast<size_t>( KeyPairing::PPB_KEY_MAX ) >;

        PPB_INLINE
        instance( void ) noexcept;


        //-----------------------------------
        //             Getters
        //-----------------------------------

        PPB_INLINE
        BindingGuids FindBindings( void ) const noexcept;
        

        //-----------------------------------
        //             Setters
        //-----------------------------------

        PPB_INLINE
        void SetBinding( const paperback::u64 BindingGuidValue
                       , const KeyPairing     Pairing = KeyPairing::PPB_DEFAULT_KEY ) noexcept;

        PPB_INLINE
        void SetBinding( const binding::type::guid& BindingGuid
                       , const KeyPairing           Pairing = KeyPairing::PPB_DEFAULT_KEY ) noexcept;

        PPB_INLINE
        void ResetBinding( const KeyPairing Pairing = KeyPairing::PPB_DEFAULT_KEY ) noexcept;

        PPB_INLINE
        void SetActionType( const action::type::id ActionType = action::type::id::BUTTON ) noexcept;


        //-----------------------------------
        //               Data
        //-----------------------------------

        Interaction                                                          m_ActionInteraction{   };                             // Action Modifiers
        ActionParings                                                        m_BindingGuids     { settings::invalid_index_v };     // Action to Binding Mapping - 0 Reserved For Default Action ( No Pairing ) - paperback::u64 = input::type::guid
        tools::n_bits< static_cast<size_t>(KeyPairing::PPB_KEY_MAX) >        m_PairingModifier  {   };                             // Pairing Key ( Event Triggers on Key && KeyPairing )
        action::type::id                                                     m_Type             = action::type::id::BUTTON;        // Action Type
    };
}

#endif