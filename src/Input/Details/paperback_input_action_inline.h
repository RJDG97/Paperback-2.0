#pragma once

#ifndef PPB_INPUT_ACTION_INLINE
#define PPB_INPUT_ACTION_INLINE

namespace paperback::input::action
{
    instance::instance( void ) noexcept
    {
        m_BindingGuids.fill( settings::invalid_index_v );
    }


    //-----------------------------------
    //             Getters
    //-----------------------------------

    PPB_INLINE
    instance::BindingGuids instance::FindBindings( void ) const noexcept
    {
        BindingGuids List;

        for ( const auto& Bind : m_BindingGuids )
            if ( Bind != settings::invalid_index_v )
                List.push_back( Bind );

        return List;
    }


    //-----------------------------------
    //             Setters
    //-----------------------------------

    PPB_INLINE
    void instance::SetBinding( const paperback::u64 BindingGuidValue
                             , const KeyPairing     Pairing ) noexcept
    {
        // Binding Already Exists
        if ( m_PairingModifier.Has( static_cast<paperback::u32>(Pairing) ))
            TRACE_PRINT( "InputAction::SetBinding() -> Overwriting Binding" );
        
        // Set Binding Again
        m_BindingGuids[ static_cast<paperback::u32>(Pairing) ] = BindingGuidValue;
        m_PairingModifier.Set( static_cast<paperback::i32>(Pairing) );
    }

    PPB_INLINE
    void instance::SetBinding( const binding::type::guid& BindingGuid
                             , const KeyPairing           Pairing ) noexcept
    {
        SetBinding( BindingGuid.m_Value
                  , Pairing );
    }

    PPB_INLINE
    void instance::ResetBinding( const KeyPairing Pairing ) noexcept
    {
        // Reset Binding
        m_BindingGuids[ static_cast<paperback::u32>(Pairing) ] = settings::invalid_index_v;
        m_PairingModifier.Remove( static_cast<paperback::i32>(Pairing) );
    }

    PPB_INLINE
    void instance::SetActionType( const action::type::id ActionType ) noexcept
    {
        m_Type = ActionType;
    }
}

#endif