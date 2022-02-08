#pragma once

struct onevent_ResetAnimation : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "onevent_ResetAnimation"
    };

    PPB_FORCEINLINE
    void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<physics_system::Event_OnStatic>(this);
    }

    void OnEvent( entity& Entity ) noexcept
    {
        auto Info = GetEntityInfo( Entity.m_GlobalIndex );
        
        if ( Info.m_pArchetype )
        {
            auto [ Anim, Controller, Interaction ] = Info.m_pArchetype->FindComponents<animator, player_controller, player_interaction>( Info.m_PoolDetails );

            if ( Controller && Anim )
            {
                // Strong Unit
                if ( Interaction )
                {
                    Anim->m_CurrentAnimationName = "StrongToy_Armature|Idle";
                    auto UI_Sys = FindSystem<ui_system>();
                    if (UI_Sys)
                    {
                        UI_Sys->TriggerStopSoundEntity("SFX_RedWalk");
                        //UI_Sys->TriggerSoundEntity("SFX_RedStop");
                    }
                }
                else
                    Anim->m_CurrentAnimationName = "Armature|Idle";

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
            auto [ Anim, Controller, Interaction ] = Info.m_pArchetype->FindComponents<animator, player_controller, player_interaction>( Info.m_PoolDetails );

            if ( Controller && Anim )
            {
                // Strong Unit
                if ( !Interaction )
                    Anim->m_CurrentAnimationName = "Armature|JumpEnd";

                Anim->m_CurrentTime = 0.0f;
                Anim->m_PlayOnce = false;
            }
        }
    }
};