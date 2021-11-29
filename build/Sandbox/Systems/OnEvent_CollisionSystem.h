#pragma once

#include "Systems/DebugSystem.h"

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

    void OnEvent(entity& obj, entity& obj2, rigidforce& rf, rigidforce& rf2) noexcept
    {
        //// Play Animation
        //auto m_obj = GetEntityInfo(obj.m_GlobalIndex);
        //auto m_obj2 = GetEntityInfo(obj2.m_GlobalIndex);
        //// check obj is friendly
        //auto Unit_1_Friendly = m_obj.m_pArchetype->FindComponent<friendly>(m_obj.m_PoolDetails);
        //auto Unit_2_Friendly = m_obj2.m_pArchetype->FindComponent<friendly>(m_obj2.m_PoolDetails);
        //// check obj is enemy
        //auto Unit_1_Enemy = m_obj.m_pArchetype->FindComponent<enemy>(m_obj.m_PoolDetails);
        //auto Unit_2_Enemy = m_obj2.m_pArchetype->FindComponent<enemy>(m_obj2.m_PoolDetails);

        //auto Unit_State = m_obj.m_pArchetype->FindComponent<unitstate>(m_obj.m_PoolDetails);

        //// if friendly units
        //if (Unit_State && !Unit_State->isAttacking && (Unit_1_Friendly && Unit_2_Friendly) || (Unit_1_Enemy && Unit_2_Enemy)) {

        //    // Pause Movement
        //    rf.m_Momentum = {};
        //    rf.m_Forces = {};

        //    animator* anim = &m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);
        //    // change animation
        //    if (anim)
        //        anim->m_CurrentAnimationName = "Armature|Idle";
        //}

        //// if oposing units
        //if ((Unit_1_Friendly && Unit_2_Enemy) || (Unit_1_Enemy && Unit_2_Friendly)) {

        //    // Pause Movement
        //    rf.m_Momentum = {};
        //    rf.m_Forces = {};
        //    
        //    animator* anim = &m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);

        //    if (anim && anim->m_FinishedAnimating)
        //        anim->m_CurrentAnimationName = "Armature|Attack";

        //    Unit_State->isAttacking = true;
        //}
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

    void OnEvent(entity& obj, entity& obj2, rigidforce& rf, rigidforce& rf2) noexcept
    {
        ////Check if animation is attack and if finished
        //auto m_obj = GetEntityInfo(obj.m_GlobalIndex);
        //auto m_obj2 = GetEntityInfo(obj2.m_GlobalIndex);
        //// check obj is friendly
        //auto Unit_1_Friendly = m_obj.m_pArchetype->FindComponent<friendly>(m_obj.m_PoolDetails);
        //auto Unit_2_Friendly = m_obj2.m_pArchetype->FindComponent<friendly>(m_obj2.m_PoolDetails);
        //// check obj is enemy
        //auto Unit_1_Enemy = m_obj.m_pArchetype->FindComponent<enemy>(m_obj.m_PoolDetails);
        //auto Unit_2_Enemy = m_obj2.m_pArchetype->FindComponent<enemy>(m_obj2.m_PoolDetails);

        //auto Unit_State = m_obj.m_pArchetype->FindComponent<unitstate>(m_obj.m_PoolDetails);
        //auto Unit_State2 = m_obj2.m_pArchetype->FindComponent<unitstate>(m_obj2.m_PoolDetails);

        //// if oposing units
        //if (Unit_1_Friendly && Unit_2_Enemy || Unit_1_Enemy && Unit_2_Friendly) {
        //    // Pause Movement
        //    rf.m_Momentum = {};
        //    rf.m_Forces = {};

        //    // get animator component
        //    animator* anim = &m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);
        //    anim->m_CurrentAnimationName = "Armature|Attack";
        //    if (anim && anim->m_FinishedAnimating) {
        //        // Check player attack against obj defense
        //        damage* unitdamage = &m_obj.m_pArchetype->GetComponent<damage>(m_obj.m_PoolDetails);
        //        timer* unittimer = &m_obj.m_pArchetype->GetComponent<timer>(m_obj.m_PoolDetails);
        //        health* enemyhealth = &m_obj2.m_pArchetype->GetComponent<health>(m_obj2.m_PoolDetails);

        //        if (unitdamage != nullptr && enemyhealth != nullptr) {
        //            // Apply Modifier
        //            // Deal Damage to obj
        //            if ( unittimer->m_Value <= 0.0f )
        //            {
        //                enemyhealth->m_CurrentHealth -= unitdamage->m_Value;
        //                if (enemyhealth->m_CurrentHealth <= 0) {
        //                    Unit_State->isAttacking = false;
        //                    BroadcastGlobalEvent<collision_system::UnitTriggerExitEvent>(obj, rf);
        //                }
        //                unittimer->m_Value = unittimer->m_Cooldown;
        //            }
        //        }
        //    }
        //}
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

    void OnEvent(entity& obj, rigidforce& rf) noexcept
    {
        //auto m_obj = GetEntityInfo(obj.m_GlobalIndex);
        //// check obj is friendly
        //auto Unit_Friendly = m_obj.m_pArchetype->FindComponent<friendly>(m_obj.m_PoolDetails);
        //// check obj is enemy
        //auto Unit_Enemy = m_obj.m_pArchetype->FindComponent<enemy>(m_obj.m_PoolDetails);

        //auto Unit_State = m_obj.m_pArchetype->FindComponent<unitstate>(m_obj.m_PoolDetails);

        //if (Unit_State && !Unit_State->isAttacking && (Unit_Friendly || Unit_Enemy)) {
        //    animator* anim = &m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);
        //    if (anim)
        //        anim->m_CurrentAnimationName = "Armature|Walk";

        //    // Update Momentum again
        //    auto Transform = m_obj.m_pArchetype->FindComponent<transform>(m_obj.m_PoolDetails);
        //    auto Waypoint = m_obj.m_pArchetype->FindComponent<waypoint>(m_obj.m_PoolDetails);
        //    if ( Transform && Waypoint )
        //    {
        //        auto Direction = Waypoint->m_Value - Transform->m_Position;
        //        Direction /= Direction.Magnitude();
        //        rf.m_Momentum = Direction * 4.0f;
        //    }
        //}
    }
};