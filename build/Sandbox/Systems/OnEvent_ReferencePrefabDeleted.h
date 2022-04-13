#pragma once
struct onevent_ReferencePrefabDeleted_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "OnEvent_ReferencePrefabDeleted"
    };

    PPB_FORCEINLINE
        void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass< paperback::vm::instance::OnEvent_ReferencePrefabDeleted >(this);
    }

    void OnEvent(const reference_prefab& RefPrefab, const paperback::u32& InstanceGID) noexcept
    {
        auto& PrefabInfo = GetEntityInfo(RefPrefab.m_PrefabGID);

        if ( PrefabInfo.m_pArchetype )
        {
            auto Prefab = PrefabInfo.m_pArchetype->FindComponent<prefab>( PrefabInfo.m_PoolDetails );
            if ( Prefab ) Prefab->RemovePrefabInstance( InstanceGID );
        }
    }
};