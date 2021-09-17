#pragma once

#include <iostream>
#include "Mono.h"

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
		m_pMono = new Mono;
	}

	void Update(void) noexcept 
	{
		//m_pMono->externaltest();
	}

	void OnSystemTerminated(void) noexcept 
	{
		// you might have to delete m_pMono too
		delete m_pMono;
	}
};