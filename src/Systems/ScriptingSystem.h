#pragma once

#include <iostream>
#include "Mono.h"
#include "paperback_camera.h"
#include "paperback_input.h"
#include "Scripts/IScripts.h"

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
	}

	void Update(void) noexcept 
	{
		m_pMono->RunImportFn(m_pMono->m_pMainObj, m_pMono->m_pMainFn);
	}

	void OnSystemTerminated(void) noexcept 
	{
		delete m_pMono;
	}
};