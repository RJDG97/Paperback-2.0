#pragma once

#include "Systems/DebugSystem.h"

static constexpr auto ResetForces = [&]( rigidforce& RF1, rigidforce& RF2 )
                                    {
                                        RF1.m_Momentum = {}; RF1.m_Forces = {};
                                        RF2.m_Momentum = {}; RF2.m_Forces = {};
                                    };

struct onevent_UnitTrigger_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "onevent_unittrigger_system"
    };

    PPB_FORCEINLINE
    void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<collision_system::OnCollisionEnter>(this);
    }

    void OnEvent( entity& obj, entity& obj2, rigidforce& rf, rigidforce& rf2, bool& Skip) noexcept
    {
        // Get Entity Info
        auto m_obj  = GetEntityInfo(obj.m_GlobalIndex);
        auto m_obj2 = GetEntityInfo(obj2.m_GlobalIndex);

        // Get Relevant Components
        auto [ Unit_1_Friendly, Unit_1_Enemy, Unit_State, Base_1, Sound_1 ]  = m_obj.m_pArchetype->FindComponents < friendly, enemy, unitstate, base, sound >( m_obj.m_PoolDetails );
        auto [ Unit_2_Friendly, Unit_2_Enemy, Unit_State2, CapturePt_2 ] = m_obj2.m_pArchetype->FindComponents< friendly, enemy, unitstate, capture_point >( m_obj2.m_PoolDetails );

        // Same Unit Type && Not Currently Fighting - WALK
        if ( Unit_State && Unit_State->IsNotState( UnitState::ATTACK ) &&
            ((Unit_1_Friendly && Unit_2_Friendly) || (Unit_1_Enemy && Unit_2_Enemy)) )
        {
            if (Unit_State && Unit_State->IsState(UnitState::DEAD))
                return;
            
            // Set Unit's State to Walk
            Unit_State->SetState( UnitState::WALK );

            auto Unit_1_Anim = m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails);

            // Update Animation
            if ( Unit_1_Anim )
            {
                Unit_1_Anim->m_CurrentAnimationName = "Armature|Walk";
                Unit_1_Anim->m_PlayOnce = false;
            }
        }

        // Diff Unit Types - ATTACK
        else if ( (Unit_1_Friendly && Unit_2_Enemy) || 
                  (Unit_1_Enemy && Unit_2_Friendly) || 
                  ((Unit_1_Friendly || Unit_1_Enemy) && CapturePt_2 && !CapturePt_2->m_Captured ))
        {
            if (Base_1)
            {
                if (Sound_1)
                    Sound_1->m_Trigger = true;
                return;
            }

            // Disable Movement - Maintain Collision
            ResetForces(rf, rf2);

            // Set Unit's State to Attack
            Unit_State->SetState( UnitState::ATTACK );

            auto Unit_1_Anim = m_obj.m_pArchetype->FindComponent<animator>( m_obj.m_PoolDetails );

            auto Unit_1_Parent = m_obj.m_pArchetype->GetComponent<parent>(m_obj.m_PoolDetails);

            for (auto child : Unit_1_Parent.m_ChildrenGlobalIndexes) {
                auto m_childobj = GetEntityInfo(child);
                auto m_ChildAnim = m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);
                if (&m_ChildAnim) {
                    m_ChildAnim.m_PlayOnce = false;
                }
            }

            // Update Animation
            if ( Unit_1_Anim && Unit_1_Anim->m_FinishedAnimating )
            {
                Unit_1_Anim->m_CurrentAnimationName = "Armature|Attack";
                Unit_1_Anim->m_PlayOnce = false;
            }
        }

        Skip = true;
    }
};



struct onevent_UnitTriggerStay_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "onevent_unittriggerstay_system"
    };

    struct PointCaptured : paperback::event::instance< const bool& > {};
    struct CapturePointDamaged : paperback::event::instance< entity&, health&, const bool& > {};

     PPB_FORCEINLINE
     void OnSystemCreated(void) noexcept
     {
         RegisterGlobalEventClass<collision_system::OnCollisionStay>(this);
     }

    void OnEvent( entity& obj, entity& obj2, rigidforce& rf, rigidforce& rf2, bool& Skip ) noexcept
    {
        // Get Entity Info
        auto m_obj = GetEntityInfo(obj.m_GlobalIndex);
        auto m_obj2 = GetEntityInfo(obj2.m_GlobalIndex);

        // Get Relevant Components
        auto [ Unit_1_Friendly, Unit_1_Enemy, Unit_State, Base_1, CapturePt, Sound_1 ] = m_obj.m_pArchetype->FindComponents< friendly, enemy, unitstate, base, capture_point, sound >( m_obj.m_PoolDetails );
        auto [ Unit_2_Friendly, Unit_2_Enemy, Unit_State2, Base_2, CapturePt_2 ] = m_obj2.m_pArchetype->FindComponents< friendly, enemy, unitstate, base, capture_point >( m_obj2.m_PoolDetails );

        // Skip if it's Base or Capture Point
        if (Base_1 || CapturePt)
        {
            // Disable Movement - Maintain Collision
            ResetForces(rf, rf2);
            return;
        }

        // Different Unit Types - ATTACK
        if ( Unit_State && ((Unit_1_Friendly && Unit_2_Enemy) ||                                                        // Friendly vs Enemy
                            (Unit_1_Enemy && Unit_2_Friendly) ||                                                        // Enemy vs Friendly
                           ((Unit_1_Friendly || Unit_1_Enemy) && CapturePt_2 && !CapturePt_2->m_Captured ) ))           // Friendly or Enemy vs Capture Point
        {
            // Disable Movement - Maintain Collision
            ResetForces( rf, rf2 );

            if (Unit_State->IsState(UnitState::DEAD))
                return;

            // Set Unit's State to Attack
            Unit_State->SetState( UnitState::ATTACK );

            auto Unit_1_Anim = m_obj.m_pArchetype->FindComponent<animator>(m_obj.m_PoolDetails);

            // Set Animation to Attack
            if ( Unit_1_Anim )
            {
                Unit_1_Anim->m_CurrentAnimationName = "Armature|Attack";
                Unit_1_Anim->m_PlayOnce = false;
            }

            // Current Animation Is Complete
            if ( Unit_1_Anim )
            {
                auto [ Damage_1, Timer_1 ]  = m_obj.m_pArchetype->FindComponents< damage, timer >( m_obj.m_PoolDetails );
                auto [ Damage_2, Health_2, Anim_2 ] = m_obj2.m_pArchetype->FindComponents< damage, health, animator >( m_obj2.m_PoolDetails );

                // Update Unit Health
                if ( Damage_1 && Health_2 && (Timer_1 || Base_2) )
                {
                    if ( Timer_1->m_Value <= 0.0f )
                    {
                        if (Sound_1)
                            Sound_1->m_Trigger = true;
                        // Unit vs Unit or Base
                        if ( !CapturePt_2 )
                        {
                            if (Base_2 || Damage_1->m_Type == Damage_2->m_Type) {
                                // Update Health
                                Health_2->m_CurrentHealth -= Damage_1->m_Value;
                            }
                            else if (Damage_1->m_Type == 0 && Damage_2->m_Type == 1) {
                                // Paper & Scissor
                                Health_2->m_CurrentHealth -= Damage_1->m_Value/2;
                            }
                            else if (Damage_1->m_Type == 0 && Damage_2->m_Type == 2) {
                                // Paper & Rock
                                Health_2->m_CurrentHealth -= Damage_1->m_Value * 2;
                            }
                            else if (Damage_1->m_Type == 1 && Damage_2->m_Type == 0) {
                                // Scissor & Paper
                                Health_2->m_CurrentHealth -= Damage_1->m_Value * 2;
                            }
                            else if (Damage_1->m_Type == 1 && Damage_2->m_Type == 2) {
                                // Scissor & Rock
                                Health_2->m_CurrentHealth -= Damage_1->m_Value / 2;
                            }
                            else if (Damage_1->m_Type == 2 && Damage_2->m_Type == 0) {
                                // Rock & Paper
                                Health_2->m_CurrentHealth -= Damage_1->m_Value / 2;
                            }
                            else if (Damage_1->m_Type == 2 && Damage_2->m_Type == 1) {
                                // Rock & Scissor
                                Health_2->m_CurrentHealth -= Damage_1->m_Value * 2;
                            }

                            // Delete Entity
                            if (!Base_2 && Health_2->m_CurrentHealth <= 0 )
                            {
                                Anim_2->m_CurrentAnimationName = "Armature|Death";
                                Anim_2->m_PlayOnce = true;
                                Anim_2->m_FinishedAnimating = false;
                                Unit_State2->SetState(UnitState::DEAD);
                                //DeleteEntity( obj2 );
                                BroadcastGlobalEvent<collision_system::OnCollisionExit>( obj, rf, Skip );
                            }
                        }
                        // Capture Point
                        else
                        {
                            if ( Unit_1_Friendly )    Health_2->m_CurrentHealth -= Damage_1->m_Value;    // Health <= 0 ? Friendlies Win
                            else if ( Unit_1_Enemy )  Health_2->m_CurrentHealth += Damage_1->m_Value;    // Health >= MaxHealth ? Enemies Win

                            // Health <= 0   - Friendlies Captured
                            // Health >= Max - Enemies Captured
                            if ( Health_2->m_CurrentHealth <= 0 || Health_2->m_CurrentHealth >= Health_2->m_MaxHealth )
                            {
                                CapturePt_2->m_Captured = true;
                                // Reset Movement
                                BroadcastGlobalEvent<collision_system::OnCollisionExit>( obj, rf, Skip );
                                // Update Card Decks
                                BroadcastGlobalEvent<onevent_UnitTriggerStay_system::PointCaptured>( Unit_1_Friendly ? true : false );
                            }

                            // Update Position of Flags
                            BroadcastGlobalEvent<onevent_UnitTriggerStay_system::CapturePointDamaged>( obj2, *Health_2, Unit_1_Friendly ? true : false );
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

            if (Unit_State->IsState(UnitState::DEAD))
                return;

            // Set to IDLE
            if (Unit_State && Unit_State2 && Unit_State->IsNotState( UnitState::ATTACK ) &&
                 Unit_State2->IsState( UnitState::ATTACK ) )
            {
                // Helps to Re-Enable movement when SAME Unit Types collide - This also causes the units to overshot
                BroadcastGlobalEvent<collision_system::OnCollisionExit>(obj, rf, Skip);
            }
        }
        // Skip Capture Points that have been Captured
        else if ( CapturePt_2 && CapturePt_2->m_Captured || CapturePt && CapturePt->m_Captured )
            Skip = true;
    }
};



struct onevent_UnitTriggerExit_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "onevent_unittriggerexit_system"
    };

    PPB_FORCEINLINE
    void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<collision_system::OnCollisionExit>(this);
    }

    void OnEvent( entity& obj, rigidforce& rf, bool& Skip ) noexcept
    {
        // Get Entity Info
        auto m_obj = GetEntityInfo(obj.m_GlobalIndex);

        // Get Relevant Components
        auto [ Unit_Friendly, Unit_Enemy, Unit_State, Unit_Anim, Unit_1_Parent] = m_obj.m_pArchetype->FindComponents< friendly, enemy, unitstate, animator, parent >( m_obj.m_PoolDetails );

        // Not Attacking & Is Unit
        if ( Unit_State && (Unit_Friendly || Unit_Enemy) )
        {
            if (Unit_State->IsState(UnitState::DEAD))
                return;

            // Update Walking Animation
            if ( Unit_Anim )
            {
                Unit_Anim->m_CurrentAnimationName = "Armature|Walk";
                Unit_Anim->m_PlayOnce = false;
                Unit_State->SetState( UnitState::WALK );
                Skip = true;
            }

            for (auto child : Unit_1_Parent->m_ChildrenGlobalIndexes) {
                auto m_childobj = GetEntityInfo(child);
                auto m_ChildAnim = m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);
                if (&m_ChildAnim) {
                    m_ChildAnim.m_PlayOnce = true;
                }
            }
        }
    }
};