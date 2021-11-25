#ifndef UNITSTATE_H
#define UNITSTATE_H

enum class UnitState : int
{
	IDLE = 0
,	WALK
,	ATTACK
,	DEAD
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

	// this is to stop in front of an ally/enemy to attack
	bool isAttacking = false;
	int  m_State     = static_cast<int>( UnitState::IDLE );
	
	// player should be m_player = 0, enemy should be m_player = 1 (set in sandbox.cpp)
	unitstate() :
		isAttacking{false}
	{}
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