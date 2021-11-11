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

    void OnEvent(entity& obj, rigidforce& rf) noexcept
    {
        //// Pause Movement
        //rf.m_isStatic = true;
        //// Play Animation
        //auto m_obj = GetEntityInfo(obj.m_GlobalIndex);
        //animator* anim = & m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);
        //if(anim != nullptr){
        //    anim->m_PlayOnce = true;
        //    if (anim->m_FinishedAnimating)
        //        anim->m_CurrentAnimationName = "Armature|Attack";
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

        animator* anim = &m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);

        if (anim != nullptr && anim->m_FinishedAnimating) {
            // Check player attack against obj defense
            damage* unitdamage = &m_obj.m_pArchetype->GetComponent<damage>(m_obj.m_PoolDetails);
            health* enemyhealth = &m_obj.m_pArchetype->GetComponent<health>(m_obj.m_PoolDetails);

            if (unitdamage != nullptr && enemyhealth != nullptr) {
                // Apply Modifier
                // Deal Damage to obj
                enemyhealth->m_CurrentHealth -= unitdamage->m_Value;
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
        //auto m_obj = GetEntityInfo(obj.m_GlobalIndex);
        //animator* anim = &m_obj.m_pArchetype->GetComponent<animator>(m_obj.m_PoolDetails);
        //if (anim != nullptr) {
        //    anim->m_PlayOnce = true;
        //    if (anim->m_FinishedAnimating)
        //        anim->m_CurrentAnimationName = "Armature|Walk";
        //}
        //// Continue Movement
        //rf.m_isStatic = false;
    }
};