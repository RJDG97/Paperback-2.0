/**********************************************************************************
*\file         UnitState.h
*\brief        UnitState.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef UNITSTATE_H
#define UNITSTATE_H

enum class UnitState : int
{
    IDLE = 0
,   WALK
,   ATTACK
,   DEAD
};

struct unitstate
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Unit State"
    };

    void SetState( const UnitState State ) noexcept
    {
        m_State = static_cast<int>( State );
    }

    UnitState GetState( void ) noexcept
    {
        return static_cast< UnitState >( m_State );
    }

    bool IsState( const UnitState State ) const noexcept
    {
        return State == static_cast< UnitState >( m_State );
    }

    bool IsNotState( const UnitState State ) const noexcept
    {
        return State != static_cast< UnitState >( m_State );
    }

    bool isAttacking = false;
    int  m_State     = static_cast<int>( UnitState::IDLE );
};

namespace RR_UnitState
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<unitstate>( unitstate::typedef_v.m_pName )
           .constructor()( rttr::policy::ctor::as_object )
           .property( "Current State", &unitstate::m_State )( rttr::policy::prop::as_reference_wrapper );
    }
}

#endif