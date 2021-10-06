#pragma once

#include <iostream>
#include "Mono.h"
#include "paperback_camera.h"
#include "paperback_input.h"

struct scripting_system : paperback::system::instance
{
	Mono* m_pMono = nullptr;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "scripting_system"
	};

	void OnSystemCreated(void) noexcept
	{
		// Set up Mono
		m_pMono = &Mono::GetInstanced();

		PPB_ASSERT_MSG( m_pMono == nullptr
					  , "Invalid Mono Instance" );
	}

	void Update(void) noexcept
	{
		if ( m_pMono )
			m_pMono->ExternalMain();
	}

	void OnSystemTerminated(void) noexcept 
	{
		delete m_pMono;
	}
};