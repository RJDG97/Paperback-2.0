/**********************************************************************************
*\file         TreeSystem.h
*\brief        TreeSystem.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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