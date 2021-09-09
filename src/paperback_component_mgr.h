#pragma once

namespace paperback::component
{
	namespace concepts
    {
        template < typename T_COMPONENT >
        concept ValidComponent = component::type::is_valid_v<T_COMPONENT>;
    }

	struct manager
    {
    private:

        inline static int m_ComponentUID = 0;
        
    public:

        template< paperback::component::concepts::ValidComponent T_COMPONENT >
        void RegisterComponent( void ) noexcept;

        template< typename... T_COMPONENTS >
        void RegisterComponents( void ) noexcept;
    };
}