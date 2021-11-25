#pragma once

#include "Systems/DebugSystem.h"

static constexpr auto ResetForces = [&]( rigidforce& RF1, rigidforce& RF2 )
                                    {
                                        RF1.m_Momentum = {}; RF1.m_Forces = {};
                                        RF2.m_Momentum = {}; RF2.m_Forces = {};
                                    };

struct onevent_UnitTrigger_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "onevent_unittrigger_system"
    };

    PPB_FORCEINLINE
    void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<collision_system::UnitTriggerEvent>(this);
    }

    PPB_FORCEINLINE
    void operator()(paperback::component::entity& Entity) noexcept
    {

    }

    void OnEvent( entity& obj, entity& obj2, rigidforce& rf, rigidforce& rf2 ) noexcept
    {
        // Get Entity Info
        auto m_obj  = GetEntityInfo(obj.m_GlobalIndex);
        auto m_obj2 = GetEntityInfo(obj2.m_GlobalIndex);

        // Get Relevant Components
        auto [ Unit_1_Friendly, Unit_1_Enemy, Unit_State ]  = m_obj.m_pArchetype->FindComponents < friendly, enemy, unitstate >( m_obj.m_PoolDetails );
        auto [ Unit_2_Friendly, Unit_2_Enemy, Unit_State2 ] = m_obj2.m_pArchetype->FindComponents< friendly, enemy, unitstate >( m_obj2.m_PoolDetails );

        // Same Unit Type && Not Currently Fighting - IDLE
        if ( Unit_State && !Unit_State->isAttacking && 
            (Unit_1_Friendly && Unit_2_Friendly) || 
            (Unit_1_Enemy && Unit_2_Enemy) )
        {
            // Disable Movement - Maintain Collision
            ResetForces( rf, rf2 );
            
            // Set Unit's State to Idle
            Unit_State->SetState( UnitState::IDLE );

            auto Unit_1_Anim = m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails);

            // Set Unit to Idle Permanently
            if ( Unit_1_Anim && Unit_1_Anim->m_FinishedAnimating )
            {
                Unit_1_Anim->m_CurrentAnimationName = "Armature|Idle";
                Unit_1_Anim->m_PlayOnce = false;
            }
        }

        // Diff Unit Types - ATTACK
        else if ( (Unit_1_Friendly && Unit_2_Enemy) || 
                  (Unit_1_Enemy && Unit_2_Friendly) )
        {
            // Disable Movement - Maintain Collision
            ResetForces( rf, rf2 );

            // Set Unit's State to Attack
            Unit_State->SetState( UnitState::ATTACK );
            Unit_State->isAttacking = true;

            auto Unit_1_Anim = m_obj.m_pArchetype->FindComponent<animator>( m_obj.m_PoolDetails );

            // Update Animation
            if ( Unit_1_Anim && Unit_1_Anim->m_FinishedAnimating )
            {
                Unit_1_Anim->m_CurrentAnimationName = "Armature|Attack";
                Unit_1_Anim->m_PlayOnce = true;
            }
        }
    }
};



struct onevent_UnitTriggerStay_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "onevent_unittriggerstay_system"
    };

     PPB_FORCEINLINE
     void OnSystemCreated(void) noexcept
     {
         RegisterGlobalEventClass<collision_system::UnitTriggerStayEvent>(this);
     }
 
    PPB_FORCEINLINE
    void operator()(paperback::component::entity& Entity) noexcept
    {
    }

    void OnEvent( entity& obj, entity& obj2, rigidforce& rf, rigidforce& rf2, bool& Skip ) noexcept
    {
        //Check if animation is attack and if finished
        auto m_obj = GetEntityInfo(obj.m_GlobalIndex);
        auto m_obj2 = GetEntityInfo(obj2.m_GlobalIndex);

        auto [ Unit_1_Friendly, Unit_1_Enemy, Unit_State ] = m_obj.m_pArchetype->FindComponents< friendly, enemy, unitstate >( m_obj.m_PoolDetails );
        auto [ Unit_2_Friendly, Unit_2_Enemy, Unit_State2 ] = m_obj2.m_pArchetype->FindComponents< friendly, enemy, unitstate >( m_obj2.m_PoolDetails );

        // Different Unit Types
        if (Unit_1_Friendly && Unit_2_Enemy || Unit_1_Enemy && Unit_2_Friendly)
        {
            // Disable Movement - Maintain Collision
            ResetForces( rf, rf2 );

            auto Unit_1_Anim = m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails);

            // Set Animation to Attack
            if ( Unit_1_Anim )
            {
                Unit_1_Anim->m_CurrentAnimationName = "Armature|Attack";
                Unit_1_Anim->m_PlayOnce = true;
            }

            // Current Animation Is Complete
            if ( Unit_1_Anim && Unit_1_Anim->m_FinishedAnimating )
            {
                auto [ Damage_1, Timer_1 ] = m_obj.m_pArchetype->FindComponents< damage, timer >( m_obj.m_PoolDetails );
                auto [ Health_2 ]          = m_obj2.m_pArchetype->FindComponents< health >( m_obj2.m_PoolDetails );

                // Update Unit Health
                if ( Damage_1 && Health_2 && Timer_1 )
                {
                    if ( Timer_1->m_Value <= 0.0f )
                    {
                        // Update Health
                        Health_2->m_CurrentHealth -= Damage_1->m_Value;

                        // Delete Entity
                        if ( Health_2->m_CurrentHealth <= 0 )
                        {
                            DeleteEntity( obj2 );
                            Unit_State->isAttacking = false;
                            BroadcastGlobalEvent<collision_system::UnitTriggerExitEvent>( obj, rf, Skip );
                        }

                        // Reset Timer
                        Timer_1->m_Value = Timer_1->m_Cooldown;
                    }
                }
            }
        }
        // Same Unit Type
        else if ( (Unit_1_Friendly && Unit_2_Friendly) || 
                  (Unit_1_Enemy && Unit_2_Enemy) )
        {
            // Disable Movement - Maintain Collision
            ResetForces( rf, rf2 );

            // Set to IDLE
            if ( Unit_State->IsNotState( UnitState::ATTACK ) &&
                 Unit_State2->IsNotState( UnitState::ATTACK ) )
            {
                Unit_State->SetState( UnitState::IDLE );

                // Helps to Re-Enable movement when SAME Unit Types collide - This also causes the units to overshot
                BroadcastGlobalEvent<collision_system::UnitTriggerExitEvent>(obj, rf, Skip);
            }
        }
    }
};



struct onevent_UnitTriggerExit_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "onevent_unittriggerexit_system"
    };

    PPB_FORCEINLINE
    void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<collision_system::UnitTriggerExitEvent>(this);
    }

    PPB_FORCEINLINE
    void operator()(paperback::component::entity& Entity) noexcept
    {

    }

    void OnEvent( entity& obj, rigidforce& rf, bool& Skip ) noexcept
    {
        auto m_obj = GetEntityInfo(obj.m_GlobalIndex);

        auto [ Unit_Friendly, Unit_Enemy, Unit_State ] = m_obj.m_pArchetype->FindComponents< friendly, enemy, unitstate >( m_obj.m_PoolDetails );

        // Not Attacking & Is Unit
        if ( Unit_State && !Unit_State->isAttacking && 
             (Unit_Friendly || Unit_Enemy) )
        {
            auto Unit_Anim = m_obj.m_pArchetype->FindComponent<animator>( m_obj.m_PoolDetails );

            // Update Walking Animation
            if ( Unit_Anim )
            {
                Unit_Anim->m_CurrentAnimationName = "Armature|Walk";
                Unit_Anim->m_PlayOnce = false;
                Unit_State->SetState( UnitState::WALK );
            }

            // Update Force
            auto Transform = m_obj.m_pArchetype->FindComponent<transform>(m_obj.m_PoolDetails);
            auto Waypoint = m_obj.m_pArchetype->FindComponent<waypoint>(m_obj.m_PoolDetails);
            if ( Transform && Waypoint )
            {
                auto Direction = Waypoint->m_Value - Transform->m_Position;
                Direction /= Direction.Magnitude();
                rf.m_Momentum = Direction * 4.0f;
                Skip = true;
            }
        }
    }
};