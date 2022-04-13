/**********************************************************************************
*\file         Scripts_Tag.h
*\brief        Scripts_Tag.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"
#include "paperback_camera.h"

namespace MONO_TAG
{

	MONO_EXPORT bool IsPushable(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_tag = m_obj.m_pArchetype->FindComponent<pushable>(m_obj.m_PoolDetails);

		if (m_tag)
		{
			return true;
		}

		return false;
	}

	MONO_EXPORT bool IsFreezable(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_tag = m_obj.m_pArchetype->FindComponent<freezable>(m_obj.m_PoolDetails);

		if (m_tag)
		{
			return true;
		}

		return false;
	}

	void AddInternals()
	{
		mono_add_internal_call("CSScript.Tools.Tag::IsPushable(uint)", &MONO_TAG::IsPushable);
		mono_add_internal_call("CSScript.Tools.Tag::IsFreezable(uint)", &MONO_TAG::IsFreezable);
	}
}