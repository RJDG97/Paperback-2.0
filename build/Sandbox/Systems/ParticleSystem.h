#pragma once
#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM

struct particle_system : paperback::system::pausable_instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "Particle System"
    };

    tools::query QueryEmitter;
    tools::query QueryParticle;

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        QueryEmitter.m_Must.AddFromComponents<entity, particle_emitter>();
		QueryEmitter.m_NoneOf.AddFromComponents<prefab>();

        QueryParticle.m_Must.AddFromComponents<entity, particle>();
		QueryParticle.m_NoneOf.AddFromComponents<prefab>();
    }

    PPB_INLINE
    void Update( void ) noexcept
    {
        auto Dt = DeltaTime();

        // Update Particles - Remove If Dead
        ForEach( Search(QueryParticle), [&]( entity& Entity, particle& Particle ) noexcept
        {
            Particle.UpdateParticle( Dt );

            if ( !Particle.IsAlive() )
            {
                m_Coordinator.ReturnDeadParticle( Entity.m_GlobalIndex );
            }
        });

        // Update Emitters - Spawn If Reset
        ForEach( Search(QueryEmitter), [&]( entity& Entity, particle_emitter& Emitter ) noexcept
        {
            if ( !Emitter.IsAlive() ) return;

            auto RequestCount = Emitter.UpdateEmitter( Dt );

            if ( RequestCount && Emitter.IsAlive() )
            {
                auto List = m_Coordinator.RequestParticles( RequestCount
                                                          , Entity.m_GlobalIndex );
                Emitter.InitializeParticles( List );
            }
        });
    }
};

#endif