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
        // Pause Movement
        rf.m_isStatic = true;
        // Play Animation
        //animator anim = GetEntityInfo(obj.m_GlobalIndex).m_pArchetype->GetComponent<animator>(GetEntityInfo(obj.m_GlobalIndex).m_PoolDetails);
        //anim.m_PlayOnce = anim.m_FinishedAnimating;
        //anim.m_CurrentAnimationName = "Armature|Attack";
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

    void OnEvent(entity& obj) noexcept
    {
        //Check if animation is attack and if finished
        //animator anim = GetEntityInfo(obj.m_GlobalIndex).m_pArchetype->GetComponent<animator>(GetEntityInfo(obj.m_GlobalIndex).m_PoolDetails);
        //anim.m_PlayOnce = anim.m_FinishedAnimating;
        //anim.m_CurrentAnimationName = "Armature|Attack";
           // Check player attack against obj defense
               // Apply Modifier
               // Deal Damage ot obj
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
        //animator anim = GetEntityInfo(obj.m_GlobalIndex).m_pArchetype->GetComponent<animator>(GetEntityInfo(obj.m_GlobalIndex).m_PoolDetails);
        //anim.m_PlayOnce = anim.m_FinishedAnimating;
        //anim.m_CurrentAnimationName = "Armature|Walk";
        // Continue Movement
        rf.m_isStatic = false;
    }
};