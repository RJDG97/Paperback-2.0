#pragma once

struct onevent_NoHealth_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "onevent_NoHealth_system"
    };

    PPB_FORCEINLINE
        void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<health_system::NoHealthEvent>(this);
    }

    void OnEvent(entity& obj) noexcept
    {
        auto m_obj = GetEntityInfo(obj.m_GlobalIndex);
        
        // if Unit health is 0 despawn
        auto [ Unit, Anim, Unit_State] = m_obj.m_pArchetype->FindComponents<unit, animator, unitstate>(m_obj.m_PoolDetails);
        if (Unit) {
            if (Unit_State->IsState(UnitState::DEAD) && Anim->m_FinishedAnimating) {
                DeleteEntity(obj);
            }
            return;
        }

        // if base state is 0 Win/Lose condition
        auto Base = m_obj.m_pArchetype->FindComponent<base>(m_obj.m_PoolDetails);
        if (Base) {
            auto Enemy = m_obj.m_pArchetype->FindComponent<enemy>(m_obj.m_PoolDetails);
            if (Enemy) {
                // Change state to win
                PPB.QueueScene("GameWin");
                return;
            }

            auto Friend = m_obj.m_pArchetype->FindComponent<friendly>(m_obj.m_PoolDetails);
            if (Friend) {
                // Change state to lose
                PPB.QueueScene("GameLoss");
                return;
            }
        }
    }
};

struct onevent_UpdateHealth_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "onevent_UpdateHealth_system"
    };

    PPB_FORCEINLINE
        void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass<health_system::UpdateHealthEvent>(this);
    }

    void OnEvent(entity& obj, entity& obj2, health& Health, health& Health2, transform& Base, boundingbox& Box) noexcept
    {
        auto m_obj = GetEntityInfo(obj.m_GlobalIndex);

        if (Health.m_MaxHealth != Health2.m_MaxHealth) {
            Health.m_MaxHealth = Health2.m_MaxHealth;
        }

        // set the healthbar size according to the current health/max health
        if (Health.m_CurrentHealth != Health2.m_CurrentHealth) {
            Health.m_CurrentHealth = Health2.m_CurrentHealth;

            scale* Scale = &m_obj.m_pArchetype->GetComponent<scale>(m_obj.m_PoolDetails);
            transform* position = &m_obj.m_pArchetype->GetComponent<transform>(m_obj.m_PoolDetails);
            auto Friendly = m_obj.m_pArchetype->FindComponent<friendly>(m_obj.m_PoolDetails);
            auto Enemy = m_obj.m_pArchetype->FindComponent<enemy>(m_obj.m_PoolDetails);
            float offset = Scale->m_Value.x;
            // Update UI Texture
            Scale->m_Value.x = Scale->m_Value.z * ((Health.m_CurrentHealth * 1.0f)/ (Health.m_MaxHealth * 1.0f));

            // Update position
            offset -= Scale->m_Value.x;

            Base.m_Position.y -= (Box.Max.y - Box.Min.y) * (offset);
        
            if (Friendly) {
                position->m_Position.x += offset / 2;
            }
            else if (Enemy) {
                position->m_Position.x -= offset / 2;
            }
            
        }
    }
};