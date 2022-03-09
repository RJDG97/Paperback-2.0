#pragma once
#ifndef PPB_PARTICLE_MANAGER_INLINE
#define PPB_PARTICLE_MANAGER_INLINE

namespace paperback::particles
{
    PPB_INLINE
    manager::manager( coordinator::instance& Instance ) noexcept :
        m_Coordinator{ Instance }
    {
        Reset();
    }

    PPB_INLINE
    void manager::Initialize( void ) noexcept
    {
        // Create Particle Archetype
        auto ParticleArchetype = &m_Coordinator.GetOrCreateArchetype< component::entity, transform, scale, rotation
                                                                    , offset, mesh, rigidforce, rigidbody, particle>();

        // Clone If Valid
        if ( ParticleArchetype )
        {
            for ( paperback::u32 i = 0; i < manager::max_particles_v; ++i )
            {
                m_FreeList.push_back( ParticleArchetype->CreatePrefab().m_GlobalIndex );
            }
        }

        // Reverse IDs - Because we use pop_back
        for ( size_t i = 0, max = m_FreeList.size(), mid = max / 2; i < mid; ++i )
        {
            std::swap( m_FreeList[ i ], m_FreeList[ max - i - 1 ] );
        }
    }

    PPB_INLINE
    void manager::Reset( void ) noexcept
    {
        // Free IDs
        m_FreeList.clear();
        m_EmitterMap.clear();
    }

    PPB_INLINE
    manager::ParticleList manager::RequestParticles( const int            Quantity
                                                   , const paperback::u32 EmitterGID ) noexcept
    {
        ParticleList List{ };

        if ( m_FreeList.size() == 0 ) return List;

        for ( int i = 0; i < Quantity; ++i )
        {
            if ( m_FreeList.size() )
            {
                auto ID = m_FreeList.back();
                m_FreeList.pop_back();

                List.push_back( ID );
                m_EmitterMap.emplace( ID, EmitterGID );
            }
        }
    }

    PPB_INLINE
    void manager::ReturnDeadParticle( const paperback::u32 ParticleGID ) noexcept
    {
        auto it = m_EmitterMap.find( ParticleGID );

        PPB_ASSERT( it == m_EmitterMap.end() );

        auto EmitterGID = it->second;
        auto EmitterInfo = m_Coordinator.GetEntityInfo( EmitterGID );

        if ( EmitterInfo.m_pArchetype )
        {
            auto Emitter = EmitterInfo.m_pArchetype->FindComponent<particle_emitter>( EmitterInfo.m_PoolDetails );
            if ( Emitter ) Emitter->ReleaseParticle( ParticleGID );
        }
    }
}

#endif