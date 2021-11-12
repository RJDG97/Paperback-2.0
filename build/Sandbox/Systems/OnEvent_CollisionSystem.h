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

    // EventReceiver does NOT call the operator() function
    PPB_FORCEINLINE
        void operator()(paperback::component::entity& Entity) noexcept
    {
        // operator() will not be called for Systems declared as system::type::system_event
    }

    void OnEvent(entity& obj, entity& obj2, rigidforce& rf, rigidforce& rf2) noexcept
    {
        // Play Animation
        auto m_obj = GetEntityInfo(obj.m_GlobalIndex);
        auto m_obj2 = GetEntityInfo(obj2.m_GlobalIndex);
        // check obj is friendly
        auto Unit_1_Friendly = m_obj.m_pArchetype->FindComponent<friendly>(m_obj.m_PoolDetails);
        auto Unit_2_Friendly = m_obj2.m_pArchetype->FindComponent<friendly>(m_obj2.m_PoolDetails);
        // check obj is enemy
        auto Unit_1_Enemy = m_obj.m_pArchetype->FindComponent<enemy>(m_obj.m_PoolDetails);
        auto Unit_2_Enemy = m_obj2.m_pArchetype->FindComponent<enemy>(m_obj2.m_PoolDetails);
        // if oposing units
        if ((Unit_1_Friendly && Unit_2_Enemy) || (Unit_1_Enemy && Unit_2_Friendly)) {

            // Pause Movement
            rf.m_Momentum = {};
            rf.m_Forces = {};

            rf2.m_Momentum = {};
            rf2.m_Forces = {};

            animator* anim = &m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);
            animator* anim2 = &m_obj2.m_pArchetype->GetComponent<animator>(m_obj2.m_PoolDetails);
            // change animation
            if (anim != nullptr && anim2 != nullptr) {
                anim->m_PlayOnce = true;
                anim2->m_PlayOnce = true;
                if (anim->m_FinishedAnimating) {
                    anim->m_CurrentAnimationName = "Armature|Attack";
                    anim->m_PlayOnce = false;
                }
                if (anim2->m_FinishedAnimating) {
                    anim2->m_CurrentAnimationName = "Armature|Attack";
                    anim2->m_PlayOnce = false;
                }
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
 
    // EventReceiver does NOT call the operator() function
    PPB_FORCEINLINE
        void operator()(paperback::component::entity& Entity) noexcept
    {
        // operator() will not be called for Systems declared as system::type::system_event
    }

    void OnEvent(entity& obj, entity& obj2) noexcept
    {
        //Check if animation is attack and if finished
        auto m_obj = GetEntityInfo(obj.m_GlobalIndex);
        auto m_obj2 = GetEntityInfo(obj2.m_GlobalIndex);
        // check obj is friendly
        auto Unit_1_Friendly = m_obj.m_pArchetype->FindComponent<friendly>(m_obj.m_PoolDetails);
        auto Unit_2_Friendly = m_obj2.m_pArchetype->FindComponent<friendly>(m_obj2.m_PoolDetails);
        // check obj is enemy
        auto Unit_1_Enemy = m_obj.m_pArchetype->FindComponent<enemy>(m_obj.m_PoolDetails);
        auto Unit_2_Enemy = m_obj2.m_pArchetype->FindComponent<enemy>(m_obj2.m_PoolDetails);
        // if oposing units
        if (Unit_1_Friendly && Unit_2_Enemy || Unit_1_Enemy && Unit_2_Friendly) {
            // get animator component
            animator* anim = &m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);
            animator* anim2 = &m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);
            // check if animation has finished
            if (anim != nullptr && anim->m_FinishedAnimating) {
                anim->m_PlayOnce = false;
                // Check player attack against obj defense
                damage* unitdamage = &m_obj.m_pArchetype->GetComponent<damage>(m_obj.m_PoolDetails);
                health* enemyhealth = &m_obj2.m_pArchetype->GetComponent<health>(m_obj2.m_PoolDetails);

                if (unitdamage != nullptr && enemyhealth != nullptr) {
                    // Apply Modifier
                    // Deal Damage to obj
                    enemyhealth->m_CurrentHealth -= unitdamage->m_Value;
                }
            }

            if (anim2 != nullptr && anim2->m_FinishedAnimating) {
                anim2->m_PlayOnce = false;
                // Check player attack against obj defense
                damage* unitdamage = &m_obj2.m_pArchetype->GetComponent<damage>(m_obj2.m_PoolDetails);
                health* enemyhealth = &m_obj.m_pArchetype->GetComponent<health>(m_obj.m_PoolDetails);

                if (unitdamage != nullptr && enemyhealth != nullptr) {
                    // Apply Modifier
                    // Deal Damage to obj
                    enemyhealth->m_CurrentHealth -= unitdamage->m_Value;
                }
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

    // EventReceiver does NOT call the operator() function
    PPB_FORCEINLINE
        void operator()(paperback::component::entity& Entity) noexcept
    {
        // operator() will not be called for Systems declared as system::type::system_event
    }

    void OnEvent(entity& obj, rigidforce& rf) noexcept
    {
        auto m_obj = GetEntityInfo(obj.m_GlobalIndex);
        // check obj is friendly
        auto Unit_Friendly = m_obj.m_pArchetype->FindComponent<friendly>(m_obj.m_PoolDetails);
        // check obj is enemy
        auto Unit_Enemy = m_obj.m_pArchetype->FindComponent<enemy>(m_obj.m_PoolDetails);

        if (Unit_Friendly || Unit_Enemy) {
            animator* anim = &m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);
            if (anim != nullptr) {
                anim->m_PlayOnce = false;
                anim->m_CurrentAnimationName = "Armature|Walk";
            }
            // Continue Movement
            rf.m_isStatic = false;
        }
    }
};