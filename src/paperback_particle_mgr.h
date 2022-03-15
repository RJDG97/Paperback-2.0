#pragma once
#ifndef PPB_PARTICLE_MANAGER
#define PPB_PARTICLE_MANAGER

namespace paperback::particles
{
    class manager final
    {
    public:

        static inline paperback::u32 max_particles_v = 1000;

        using ParticleGID = paperback::u32;
        using EmitterGID  = paperback::u32;

        using FreeList     = std::vector< ParticleGID >;
        using EmitterMap   = std::unordered_map< ParticleGID, EmitterGID >;
        using ParticleList = std::vector< ParticleGID >;

        //-----------------------------------
        //             Defaults
        //-----------------------------------

        PPB_INLINE
        manager( coordinator::instance& Instance ) noexcept;

        PPB_INLINE
        void Initialize( void ) noexcept;

        PPB_INLINE
        void Reset( void ) noexcept;

        PPB_INLINE
        ParticleList RequestParticles( const int            Quantity
                                     , const paperback::u32 EmitterGID ) noexcept;

        PPB_INLINE
        void ReturnDeadParticle( const paperback::u32 ParticleGID ) noexcept;


    private:
        
        paperback::u32                      m_ParticleCount = 0;
        paperback::archetype::instance*     m_ParticleArchetype = nullptr;
        FreeList                            m_FreeList;
        EmitterMap                          m_EmitterMap;
        coordinator::instance&              m_Coordinator;
    };
}

#endif