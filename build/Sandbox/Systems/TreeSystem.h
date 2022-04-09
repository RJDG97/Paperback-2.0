#pragma once

struct tree_system : paperback::system::instance
{
	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "tree_system"
	};

	PPB_INLINE
	void OnStateChange( void ) noexcept
	{
        m_Coordinator.ResetTree( );
	}


	PPB_INLINE
	void OnStateLoad( void ) noexcept
	{
        m_Coordinator.InitializeTree();
	}

	PPB_FORCEINLINE
	void Update(void) noexcept
	{ }
};