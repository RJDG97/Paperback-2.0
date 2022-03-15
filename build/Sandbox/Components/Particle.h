#pragma once

BEGIN_CREATE_DATA_COMPONENT( particle, Particle )
	
	PPB_INLINE
	void UpdateParticle( const float Dt ) noexcept
	{
		m_Lifetime -= Dt;
	}

	PPB_INLINE
	bool IsAlive( void ) const noexcept
	{
		return m_Lifetime > 0.0f;
	}

	float m_Lifetime        = 0.0f;
	float m_Opacity         = 1.0f;
	bool  m_bHasDestination = false;

END_CREATE_DATA_COMPONENT


namespace RR_Particle
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_< particle >( particle::typedef_v.m_pName )
			.constructor()(rttr::policy::ctor::as_object)
			.property( "Particle Lifetime", &particle::m_Lifetime )
			.property( "Particle Opacity",  &particle::m_Opacity  )
			.property( "Particle Has Dest",  &particle::m_bHasDestination );
	}
}