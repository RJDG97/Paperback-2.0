#pragma once

#ifndef PPB_INPUT_ACTION_INLINE
#define PPB_INPUT_ACTION_INLINE

namespace paperback::input::action
{
    instance::instance( void ) noexcept
    {
        m_PressedBindingGuids.fill( settings::invalid_index_v );
        m_ContinuousBindingGuids.fill( settings::invalid_index_v );
        m_ReleasedBindingGuids.fill( settings::invalid_index_v );
    }


    //-----------------------------------
    //             Getters
    //-----------------------------------

    PPB_INLINE
    instance::BindingGuids instance::FindBindings( const action::BroadcastStatus Status ) const noexcept
    {
        BindingGuids List;

        switch( Status )
        {
            case action::BroadcastStatus::PRESSED:
            {
                for ( const auto& Bind : m_PressedBindingGuids )
                    if ( Bind != settings::invalid_index_v )
                        List.push_back( Bind );
                break;
            }
            case action::BroadcastStatus::CONTINUOUS:
            {
                for ( const auto& Bind : m_ContinuousBindingGuids )
                    if ( Bind != settings::invalid_index_v )
                        List.push_back( Bind );
                break;
            }
            case action::BroadcastStatus::RELEASED:
            {
                for ( const auto& Bind : m_ReleasedBindingGuids )
                    if ( Bind != settings::invalid_index_v )
                        List.push_back( Bind );
                break;
            }
        }

        return List;
    }


    //-----------------------------------
    //             Setters
    //-----------------------------------

    PPB_INLINE
    void instance::SetBinding( const paperback::u64           BindingGuidValue
                             , const action::BroadcastStatus  Status
                             , const KeyPairing               Pairing ) noexcept
    {
        //// Binding Already Exists
        //if ( m_PairingModifier.Has( static_cast<paperback::u32>(Pairing) ))
        //    TRACE_PRINT( "InputAction::SetBinding() -> Overwriting Binding" );
        
        // Set Binding Again
        //m_PressedBindingGuids[ static_cast<paperback::u32>(Pairing) ] = BindingGuidValue;
        //m_PairingModifier.Set( static_cast<paperback::i32>(Pairing) );

        switch( Status )
        {
            case action::BroadcastStatus::PRESSED:
            {
                m_PressedBindingGuids[ static_cast<paperback::u32>(Pairing) ] = BindingGuidValue;
                break;
            }
            case action::BroadcastStatus::CONTINUOUS:
            {
                m_ContinuousBindingGuids[ static_cast<paperback::u32>(Pairing) ] = BindingGuidValue;
                break;
            }
            case action::BroadcastStatus::RELEASED:
            {
                m_ReleasedBindingGuids[ static_cast<paperback::u32>(Pairing) ] = BindingGuidValue;
                break;
            }
        }
    }

    PPB_INLINE
    void instance::SetBinding( const binding::type::guid&     BindingGuid
                             , const action::BroadcastStatus  Status
                             , const KeyPairing               Pairing ) noexcept
    {
        SetBinding( BindingGuid.m_Value
                  , Status
                  , Pairing );
    }


    //-----------------------------------
    //             Extras
    //-----------------------------------

    PPB_INLINE
    void instance::ResetBinding( const action::BroadcastStatus Status
                               , const KeyPairing Pairing ) noexcept
    {
        switch( Status )
        {
            case action::BroadcastStatus::PRESSED:
            {
                m_PressedBindingGuids[ static_cast<paperback::u32>(Pairing) ] = settings::invalid_index_v;
                break;
            }
            case action::BroadcastStatus::CONTINUOUS:
            {
                m_ContinuousBindingGuids[ static_cast<paperback::u32>(Pairing) ] = settings::invalid_index_v;
                break;
            }
            case action::BroadcastStatus::RELEASED:
            {
                m_ReleasedBindingGuids[ static_cast<paperback::u32>(Pairing) ] = settings::invalid_index_v;
                break;
            }
        }


        // Reset Binding
        //m_PressedBindingGuids[ static_cast<paperback::u32>(Pairing) ] = settings::invalid_index_v;
        //m_PairingModifier.Remove( static_cast<paperback::i32>(Pairing) );
    }

    PPB_INLINE
    void instance::SetActionType( const action::type::id ActionType ) noexcept
    {
        m_Type = ActionType;
    }
}

#endif