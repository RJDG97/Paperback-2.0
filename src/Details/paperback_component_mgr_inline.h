#pragma once

namespace paperback::component
{
	template< paperback::component::concepts::ValidComponent T_COMPONENT >
    void manager::RegisterComponent( void ) noexcept
    {
        if ( component::info_v<T_COMPONENT>.m_UID == component::info::invalid_id_v )
            component::info_v<T_COMPONENT>.m_UID = m_ComponentUID++;
        else if ( component::info_v<T_COMPONENT>.m_UID == 0 )
            ++m_ComponentUID;

        m_ComponentGuidArray[ m_ComponentUID - 1 ] = component::info_v<T_COMPONENT>.m_Guid;
        m_ComponentInfoMap.emplace( std::pair{ m_ComponentGuidArray[ m_ComponentUID - 1 ]
                                             , &component::info_v<T_COMPONENT> } );
    }

    template< typename... T_COMPONENTS >
    void manager::RegisterComponents( void ) noexcept
    {
        ( ( RegisterComponent<T_COMPONENTS>() ), ... );
    }

    const paperback::component::info* manager::FindComponentInfo( const paperback::component::type::guid ComponentGuid ) noexcept
    {
        auto CInfo = m_ComponentInfoMap.find( ComponentGuid );
        if ( CInfo != m_ComponentInfoMap.end() )
            return CInfo->second;
        return nullptr;
    }

    const paperback::component::info* manager::FindComponentInfoFromUID( const u32 ComponentUID ) noexcept
    {
        if ( ComponentUID >= static_cast<u32>( m_ComponentUID ) ) return nullptr;
        else                                                      return FindComponentInfo( m_ComponentGuidArray[ ComponentUID ] );
    }

    component::manager::ComponentInfoMap& manager::GetComponentInfoMap() noexcept
    {
        return m_ComponentInfoMap;
    }

    void manager::Terminate( void ) noexcept
    {
        
    }
}