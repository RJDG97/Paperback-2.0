#pragma once

namespace paperback::component
{
	template< paperback::component::concepts::ValidComponent T_COMPONENT >
    void manager::RegisterComponent( void ) noexcept
    {
        if ( component::info_v<T_COMPONENT>.m_UID == component::info::invalid_id_v )
            component::info_v<T_COMPONENT>.m_UID = m_ComponentUID++;
    }

    template< typename... T_COMPONENTS >
    void manager::RegisterComponents( void ) noexcept
    {
        ( ( RegisterComponent<T_COMPONENTS>() ), ... );
    }
}