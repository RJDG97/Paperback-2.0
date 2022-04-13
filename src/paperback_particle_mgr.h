/**********************************************************************************
*\file         paperback_particle_mgr.h
*\brief        paperback_particle_mgr.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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
        void InitializeParticles( component::entity&    EmitterEntity
                                , particle_emitter&     Emitter
                                , ParticleList          ParticleIDList ) noexcept;

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