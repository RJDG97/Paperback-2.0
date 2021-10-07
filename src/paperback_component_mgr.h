#pragma once

namespace paperback::component
{
	namespace concepts
    {
        template < typename T_COMPONENT >
        concept ValidComponent = component::type::is_valid_v<T_COMPONENT>;
    }

	class manager final
    {    
    public:

        using ComponentInfoMap      = std::unordered_map< paperback::component::type::guid, const paperback::component::info* >;
        using ComponentGuidArray    = std::array< paperback::component::type::guid, settings::max_component_types_v >;

        template< paperback::component::concepts::ValidComponent T_COMPONENT >
        void RegisterComponent( void ) noexcept;

        template< typename... T_COMPONENTS >
        void RegisterComponents( void ) noexcept;

        PPB_INLINE
        const paperback::component::info* FindComponentInfo( const paperback::component::type::guid ComponentGuid ) noexcept;

        PPB_INLINE
        const paperback::component::info* FindComponentInfoFromUID( const u32 ComponentUID ) noexcept;

        PPB_INLINE
        void Terminate( void ) noexcept;

    private:

        inline static int     m_ComponentUID = 0;
        ComponentInfoMap      m_ComponentInfoMap;
        ComponentGuidArray    m_ComponentGuidArray;
    };
}