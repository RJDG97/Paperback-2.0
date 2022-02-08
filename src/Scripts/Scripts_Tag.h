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

	void AddInternals()
	{
		mono_add_internal_call("CSScript.Tools.Tag::IsPushable(uint)", &MONO_TAG::IsPushable);
	}
}