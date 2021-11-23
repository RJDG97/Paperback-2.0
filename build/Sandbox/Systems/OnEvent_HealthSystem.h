#pragma once

struct onevent_NoHealth_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "onevent_NoHealth_system"
    };

    PPB_FORCEINLINE
        void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<health_system::NoHealthEvent>(this);
    }

    PPB_FORCEINLINE
        void operator()(paperback::component::entity& Entity) noexcept
    {

    }

    void OnEvent(entity& obj) noexcept
    {
        auto m_obj = GetEntityInfo(obj.m_GlobalIndex);
        
        // if Unit health is 0 despawn
        auto Unit = m_obj.m_pArchetype->FindComponent<unit>(m_obj.m_PoolDetails);
        if (Unit) {
            DeleteEntity(obj);
            return;
        }

        // if base state is 0 Win/Lose condition
        auto Base = m_obj.m_pArchetype->FindComponent<base>(m_obj.m_PoolDetails);
        if (Base) {
            auto Enemy = m_obj.m_pArchetype->FindComponent<enemy>(m_obj.m_PoolDetails);
            if (Enemy) {
                // Change state to win
                return;
            }

            auto Friend = m_obj.m_pArchetype->FindComponent<friendly>(m_obj.m_PoolDetails);
            if (Friend) {
                // Change state to lose
                return;
            }
        }
    }
};

struct onevent_UpdateHealth_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "onevent_UpdateHealth_system"
    };

    PPB_FORCEINLINE
        void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<health_system::UpdateHealthEvent>(this);
    }

    PPB_FORCEINLINE
        void operator()(paperback::component::entity& Entity) noexcept
    {

    }

    void OnEvent(entity& obj, entity& obj2, health& Health) noexcept
    {
        auto m_obj = GetEntityInfo(obj.m_GlobalIndex);

        // set the healthbar size according to the current health/max health
    }
};