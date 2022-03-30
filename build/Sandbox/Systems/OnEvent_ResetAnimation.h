#pragma once

struct onevent_ResetAnimation : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "onevent_ResetAnimation"
    };

    bool m_PushMove, m_JumpMove;


    PPB_FORCEINLINE
    void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<physics_system::Event_OnStatic>(this);
    }

    PPB_FORCEINLINE
    void OnStateLoad() noexcept
    {

        m_PushMove = m_JumpMove = false;
    }

    void OnEvent( entity& Entity ) noexcept
    {
        auto Info = GetEntityInfo( Entity.m_GlobalIndex );
        
        if ( Info.m_pArchetype )
        {
            auto [ Anim, Controller, Interaction ] = Info.m_pArchetype->FindComponents<animator, player_controller, player_interaction>( Info.m_PoolDetails );

            if (Controller && Anim)
            {
                // Strong Unit
                if ( Interaction && m_PushMove )
                {
                    Anim->m_CurrentAnimationName = "StrongToy_Armature|Idle";

                    // Stop Walk Sound
                    GetSystem<sound_system>().StopTriggeredSoundEvent( "SFX_RedWalk" );
                    m_PushMove = false;
                }
                else if ( !Interaction && m_JumpMove )
                {
                    Anim->m_CurrentAnimationName = "Armature|Idle";

                    // Stop Walk Sound
                    GetSystem<sound_system>().StopTriggeredSoundEvent( "SFX_BlueWalk" );
                    m_JumpMove = false;
                }

                Anim->m_PlayOnce = false;
            }
        }
    }
};



struct onevent_FallingAnimation : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "onevent_FallingAnimation"
    };

    PPB_FORCEINLINE
    void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<physics_system::Event_OnFalling>(this);
    }

    void OnEvent( entity& Entity ) noexcept
    {
        auto Info = GetEntityInfo( Entity.m_GlobalIndex );
        
        if ( Info.m_pArchetype )
        {
            auto [ Anim, Controller, Interaction, BB, Transform, Entity ] = Info.m_pArchetype->FindComponents<animator, player_controller, player_interaction, boundingbox, transform, entity>( Info.m_PoolDetails );

            if ( Controller && Anim && BB && Transform && Entity )
            {
                std::vector<paperback::u32> List;
                List.push_back( Entity->m_GlobalIndex );
                
                auto RayEnd = Transform->m_Position + ( paperback::Vector3f{ 0.0f, BB->Min.y, 0.0f } * 1.8f );
                auto [ ID, Dist ] = m_Coordinator.QueryRaycastClosest( Transform->m_Position, RayEnd, List );

                // Nothing within Acceptable Bounds
                if ( Dist < Transform->m_Position.y - RayEnd.y )
                {
                    return;
                }

                // Strong Unit
                if ( !Interaction )
                    Anim->m_CurrentAnimationName = "Armature|JumpEnd";

                Anim->m_CurrentTime = 0.0f;
                Anim->m_PlayOnce = false;
                m_Coordinator.GetSystem<onevent_ResetAnimation>().m_JumpMove = true;
            }
        }
    }
};