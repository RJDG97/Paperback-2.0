#pragma once
#ifndef PPB_PARTICLE_MANAGER_INLINE
#define PPB_PARTICLE_MANAGER_INLINE

namespace paperback::particles
{
    PPB_INLINE
    manager::manager( coordinator::instance& Instance ) noexcept :
        m_ParticleCount{ 0 }
    ,   m_Coordinator{ Instance }
    {
        Reset();
    }

    PPB_INLINE
    void manager::Initialize( void ) noexcept
    {
        // Gotta Call This After RegisterComponents
        m_ParticleArchetype = &m_Coordinator.GetOrCreateArchetype< component::entity, transform, scale, rotation
                                                                 , rigidforce, rigidbody, particle, mass>();
    }

    PPB_INLINE
    void manager::Reset( void ) noexcept
    {
        // Free IDs
        m_ParticleCount = 0;
        m_FreeList.clear();
        m_EmitterMap.clear();
    }

    PPB_INLINE
    manager::ParticleList manager::RequestParticles( const int            Quantity
                                                   , const paperback::u32 EmitterGID ) noexcept
    {
        if ( !m_ParticleArchetype )
        {
            ERROR_PRINT( "Particle Archetype Does Not Exist - Cannot Generate Particles" );
            return {};
        }

        ParticleList List{ };

        // Insufficient Particles To Allocate
        if ( m_FreeList.size() < Quantity )
        {
            int Count = m_FreeList.size() == 0 ? Quantity
                                               : static_cast<int>( Quantity - m_FreeList.size() );

            while ( m_ParticleCount < manager::max_particles_v &&
                    --Count >= 0 )
            {
                auto ID = m_ParticleArchetype->CreatePrefab().m_GlobalIndex;
                List.push_back( ID );
                m_EmitterMap.insert_or_assign( ID, EmitterGID );
                ++m_ParticleCount;
            }
        }
        else
        {
            if ( m_FreeList.size() < Quantity ) 
            {
                ERROR_PRINT( "Invalid Particle Quantity" );
                return {};
            }

            for ( int i = 0; i < Quantity; ++i )
            {
                List.push_back( m_FreeList.back() );
                m_FreeList.pop_back();
                m_EmitterMap.insert_or_assign( List.back(), EmitterGID );
                ++m_ParticleCount;
            }
        }

        return List;
    }


    PPB_INLINE
    void manager::InitializeParticles( component::entity&    EmitterEntity
                                     , particle_emitter&     Emitter
                                     , ParticleList          ParticleIDList ) noexcept
    {
        // Check m_bPrewarm & m_bHasDestination - TODO

		auto& EmitterInfo = m_Coordinator.GetEntityInfo( EmitterEntity.m_GlobalIndex );

        if ( !EmitterInfo.m_pArchetype ) return;

        auto [ EmitterTransform, EmitterBB ] = EmitterInfo.m_pArchetype->FindComponents<transform, boundingbox>( EmitterInfo.m_PoolDetails );

		if ( !EmitterTransform )
		{
			ERROR_PRINT( "Invalid Emitter Components During Particle Initialization" );
			return;
		}

		m_Coordinator.ForEach( ParticleIDList, [&]( particle& Particle, transform& Transform, rigidforce& RF, scale& Scale, mass& Mass, rotation& Rotation ) noexcept
		{
			// Update Particle Details
			Particle.m_ConstantRotation = Emitter.m_GenerateRotation.Rand( );
			Particle.m_Lifetime         = Emitter.m_GenerateLifetime.Rand( );
			Particle.m_Opacity          = Emitter.m_GenerateOpacity.Rand( );
			Particle.m_bHasDestination  = Emitter.m_bHasDestination;

			// Update Particle Spawn Position - Based On Emitter Position & Bounding Box Area
			Transform.m_Position        = EmitterBB ? Emitter.m_GeneratePosition.Rand( *EmitterTransform, *EmitterBB, static_cast<ParticleSpawnPosition>(Emitter.m_SpawnLocation) )
                                                    : EmitterTransform->m_Position + EmitterTransform->m_Offset;
			
			// Update Particle Velocity
			RF.m_Momentum               = Emitter.m_GenerateVelocity.Rand( );
            RF.m_dynamicFriction        = 0.001f;
            RF.m_GravityAffected        = false;
			
			// Update Particle Scale
			Scale.m_Value               = Emitter.m_GenerateScale.Rand( );

            // Need to add local & constant rotation rand - Temporary
            Rotation.m_Value            = Emitter.m_GenerateRotation.Rand( );

            // For Phy Sys - Convert this to data member to modify particle speed
            Mass.m_Mass                 = 5.0f;
		});
    }


    PPB_INLINE
    void manager::ReturnDeadParticle( const paperback::u32 ParticleGID ) noexcept
    {
        auto it = m_EmitterMap.find( ParticleGID );

        PPB_ASSERT( it == m_EmitterMap.end() );

        auto EmitterGID = it->second;
        if ( EmitterGID == settings::invalid_index_v ) return;
        it->second = settings::invalid_index_v;
        auto EmitterInfo = m_Coordinator.GetEntityInfo( EmitterGID );

        if ( EmitterInfo.m_pArchetype )
        {
            auto Emitter = EmitterInfo.m_pArchetype->FindComponent<particle_emitter>( EmitterInfo.m_PoolDetails );
            if ( Emitter ) Emitter->ReleaseParticle( ParticleGID );
        }

        --m_ParticleCount;
        m_FreeList.push_back( ParticleGID );
    }
}

#endif