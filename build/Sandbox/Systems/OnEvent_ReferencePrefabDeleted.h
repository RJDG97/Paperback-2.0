#pragma once
struct onevent_ReferencePrefabDeleted_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "OnEvent_ReferencePrefabDeleted"
    };

    PPB_FORCEINLINE
        void OnSystemCreated(void) noexcept
    {
        RegisterGlobalEventClass< paperback::vm::instance::OnEvent_ReferencePrefabDeleted >(this);
    }

    PPB_FORCEINLINE
        void operator()(paperback::component::entity& Entity) noexcept
    {

    }

    void OnEvent(const reference_prefab& RefPrefab, const paperback::u32& InstanceGID) noexcept
    {
        auto& PrefabInfo = GetEntityInfo(RefPrefab.m_PrefabGID);
        auto& Prefab = PrefabInfo.m_pArchetype->GetComponent<prefab>(PrefabInfo.m_PoolDetails);
        Prefab.RemovePrefabInstance(InstanceGID);
    }
};