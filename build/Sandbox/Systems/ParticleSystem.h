#pragma once
#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM

struct particle_system : paperback::system::pausable_instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "Particle System"
    };

    tools::query QueryPlayer;
    tools::query QueryEmitter;
    tools::query QueryParticle;
    debug_system* Debug = nullptr;

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        QueryPlayer.m_Must.AddFromComponents<player_controller, camera>();
		QueryPlayer.m_NoneOf.AddFromComponents<prefab>();

        QueryEmitter.m_Must.AddFromComponents<entity, particle_emitter>();
		QueryEmitter.m_NoneOf.AddFromComponents<prefab>();

        QueryParticle.m_Must.AddFromComponents<entity, particle, rotation, transform>();
		QueryParticle.m_NoneOf.AddFromComponents<prefab>();

        Debug = m_Coordinator.FindSystem<debug_system>();
    }


    PPB_INLINE
	void OnStateChange( void ) noexcept
	{
        m_Coordinator.ResetParticleMgr( );
	}


	PPB_INLINE
	void OnStateLoad( void ) noexcept
	{
        m_Coordinator.InitializeParticleMgr();
	}


    PPB_INLINE
    void Update( void ) noexcept
    {
        auto Dt = DeltaTime();
        glm::vec3 ActiveCameraPosition;

        // Get Player Camera
        ForEach( Search(QueryPlayer), [&]( player_controller& Controller, camera& Camera ) noexcept
        {
            if ( Controller.m_PlayerStatus && Camera.m_Active )
            {
                ActiveCameraPosition = glm::cross(Camera.m_Up, Camera.m_Right);
            }
        });

        // Update Particles - Remove If Dead
        ForEach( Search(QueryParticle), [&]( entity& Entity, particle& Particle, rotation& Rotation, transform& Transform ) noexcept
        {
            Particle.UpdateParticle( Dt );

            if ( !Particle.IsAlive() )
            {
                m_Coordinator.ReturnDeadParticle( Entity.m_GlobalIndex );
            }
        });

        // Update Emitters - Spawn If Reset
        ForEach( Search(QueryEmitter), [&]( entity& EmitterEntity, particle_emitter& Emitter ) noexcept
        {
            if ( !Emitter.IsAlive() ) return;

            bool  ComputedAngle = false;
            paperback::Vector3f Angle{};
            auto  RequestCount  = Emitter.UpdateEmitter( Dt );

            if ( RequestCount && Emitter.IsAlive() )
            {
                auto List = m_Coordinator.RequestParticles( RequestCount
                                                          , EmitterEntity.m_GlobalIndex );
                m_Coordinator.InitializeParticles( EmitterEntity
                                                 , Emitter
                                                 , List );
                Emitter.UpdateParticleList( List );
            }

            // Set Particle Rotation Angles
            ForEach( Emitter.m_ActiveParticles, [&]( entity& Entity, particle& Particle, rotation& Rotation, transform& Transform ) noexcept
            {
                if ( Particle.IsAlive() )
                {
                    if ( !ComputedAngle && Debug )
                    {
                        Angle = Debug->DirtyRotationAnglesFromDirectionalVec( paperback::Vector3f(ActiveCameraPosition.x, ActiveCameraPosition.y, ActiveCameraPosition.z) );
                        ComputedAngle = true;
                    }
                    
                    if ( ComputedAngle )
                    {
                        Rotation.m_Value = Angle;
                    }
                }
            });
        });
    }
};

#endif