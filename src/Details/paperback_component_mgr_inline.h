#pragma once

namespace paperback::component
{
    template< paperback::component::concepts::ValidComponent T_COMPONENT >
    void manager::RegisterComponent(void) noexcept
    {
        if (component::info_v<T_COMPONENT>.m_UID == component::info::invalid_id_v)
            component::info_v<T_COMPONENT>.m_UID = m_ComponentUID++;

        m_ComponentInfoMap.emplace(std::pair{ component::info_v<T_COMPONENT>.m_Guid, &component::info_v<T_COMPONENT> });
    }

    template< typename... T_COMPONENTS >
    void manager::RegisterComponents(void) noexcept
    {
        ((RegisterComponent<T_COMPONENTS>()), ...);
    }

    const paperback::component::info* manager::FindComponentInfo(const paperback::component::type::guid ComponentGuid) noexcept
    {
        auto CInfo = m_ComponentInfoMap.find(ComponentGuid);
        if (CInfo != m_ComponentInfoMap.end())
            return CInfo->second;
        return nullptr;
    }

    void manager::Terminate(void) noexcept
    {

    }
}