#pragma once

struct Generate_Lifetime
{
	PPB_INLINE
	float Rand( void ) const noexcept
	{
		return {}; // Update This
	}

	float m_Min = 0.0f;
	float m_Max = 1.0f;
};

struct Generate_Position
{
	PPB_INLINE
	paperback::Vector3f Rand( const transform&   EmitterTransform
						    , const boundingbox& EmitterBB ) const noexcept
	{
		return {}; // Update This
	}
};

struct Generate_Velocity
{
	PPB_INLINE
	paperback::Vector3f Rand( void ) const noexcept
	{
		return {}; // Update This
	}

	paperback::Vector3f m_Min{ 1.0f, 1.0f, 1.0f };
	paperback::Vector3f m_Max{ 2.0f, 2.0f, 2.0f };
};

struct Generate_Rotation
{
	PPB_INLINE
	paperback::Vector3f Rand( void ) const noexcept
	{
		return {}; // Update This
	}

	paperback::Vector3f m_Min{ -2.0f, -2.0f, -2.0f };
	paperback::Vector3f m_Max{  2.0f,  2.0f,  2.0f };
};

struct Generate_Opacity
{
	PPB_INLINE
	int Rand( void ) const noexcept
	{
		return {}; // Update This
	}

	float m_Min = 0.0f;
	float m_Max = 1.0f;
};

struct Update_Velocity
{
	PPB_INLINE
	paperback::Vector3f Rand( const transform&  ParticleTransform
		                    , const rigidforce& ParticleRF ) const noexcept
	{
		return {}; // Update This
	}

	paperback::Vector3f m_Destination{ 2.0f, 2.0f, 2.0f };
};

struct Generate_Scale
{
	PPB_INLINE
	paperback::Vector3f Rand( void ) const noexcept
	{
		return {}; // Update This
	}

	paperback::Vector3f m_Min{ 0.05f, 0.05f, 0.05f };
	paperback::Vector3f m_Max{ 0.10f, 0.10f, 0.10f };
};




//------------------------------------------------------------
// 
// 
//                     Emitter Component
// 
// 
//------------------------------------------------------------
BEGIN_CREATE_DATA_COMPONENT( particle_emitter, Particle Emitter )
	
	using ParticleList = std::vector<paperback::u32>;

	PPB_INLINE
	void InitializeParticles( std::span<paperback::u32> List ) noexcept
	{
		// Check m_bPrewarm & m_bHasDestination

		//PPB.ForEach( List, [&]( /* Relevant Components */ )
		//{
		//	// Rand Everything & Initialize
		//});

		// Merge List w/ m_ActiveParticles List
	}

	PPB_INLINE
	int UpdateEmitter( float Dt ) noexcept
	{
		m_Lifetime -= Dt;
		m_CurrentTime -= Dt;

		if ( m_CurrentTime <= 0.0f && m_Lifetime > 0.0f )
		{
			int QuantityAvailable = m_EmissionCap - static_cast<int>( m_ActiveParticles.size() );

			PPB_ASSERT( QuantityAvailable < 0 );

			return ( QuantityAvailable > m_EmissionRate ) ? m_EmissionRate
													      : QuantityAvailable;
		}

		return 0;
	}

	PPB_INLINE
	void ReleaseParticle( paperback::u32 ParticleGID ) noexcept
	{
		m_ActiveParticles.erase( std::remove(m_ActiveParticles.begin(), m_ActiveParticles.end(), ParticleGID), m_ActiveParticles.end() );
	}

	PPB_INLINE
	bool IsAlive( void ) const noexcept
	{
		return m_Lifetime > 0.0f;
	}

	//-----------------------------------
	//          Emitter Data
	//-----------------------------------
	float              m_Lifetime         = 0.0f;				// Lifetime of Spawner
	float              m_CurrentTime      = 0.0f;				// Current Timer
	float              m_EmissionInterval = 1.0f;				// Rate Of Particle Emission
	int                m_EmissionRate     = 1;					// Particles Spawned Per Emission Interval
	int                m_EmissionCap      = 100;				// Max Particles Owned By Emitter
	ParticleList       m_ActiveParticles;						// List Of Active Particles
	std::string        m_TextureName;							// Texture Name

	//-----------------------------------
	//       Particle Generation
	//-----------------------------------
	Generate_Lifetime  m_GenerateLifetime;
	Generate_Position  m_GeneratePosition;
	Generate_Velocity  m_GenerateVelocity;
	Generate_Rotation  m_GenerateRotation;
	Generate_Opacity   m_GenerateOpacity;
	Update_Velocity    m_UpdateVelocity;
	Generate_Scale     m_GenerateScale;

	//-----------------------------------
	//          Emitter Status
	//-----------------------------------
	bool               m_bHasDestination = false;               // If True - Compute Normalized Direction & Multiply By Randomized Velocity Magnitude
	bool               m_bPrewarm = false;						// If True - Simulate Particles Being Already Emitted for ___ Emission Interval Loops

END_CREATE_DATA_COMPONENT


// Need to register all "Rands" too


namespace RR_ParticleEmitter
{
	//RTTR_REGISTRATION
	//{
	//	rttr::registration::class_< particle_emitter >( particle_emitter::typedef_v.m_pName )
	//		.constructor()(rttr::policy::ctor::as_object)
	//		.property( "Emitter Lifetime",           &particle_emitter::m_Lifetime         )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Emitter Timer",              &particle_emitter::m_CurrentTime      )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Emission Interval",          &particle_emitter::m_EmissionInterval )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Emission Rate",              &particle_emitter::m_EmissionRate     )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Emission Cap",               &particle_emitter::m_EmissionCap      )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Active Particle List",       &particle_emitter::m_ActiveParticles  )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Particle Texture Name",      &particle_emitter::m_TextureName      )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Generate Particle Lifetime", &particle_emitter::m_GenerateLifetime )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Generate Particle Position", &particle_emitter::m_GeneratePosition )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Generate Particle Velocity", &particle_emitter::m_GenerateVelocity )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Generate Particle Rotation", &particle_emitter::m_GenerateRotation )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Generate Particle Opacity",  &particle_emitter::m_GenerateOpacity  )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Update Particle Velocity",   &particle_emitter::m_UpdateVelocity   )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Generate Particle Scale",    &particle_emitter::m_GenerateScale    )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Particle Has Destination?",  &particle_emitter::m_bHasDestination  )( rttr::policy::prop::as_reference_wrapper )
	//		.property( "Enable Emitter Prewarm?",    &particle_emitter::m_bPrewarm         )( rttr::policy::prop::as_reference_wrapper );
	//}
}