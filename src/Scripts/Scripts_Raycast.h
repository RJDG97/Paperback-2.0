/**********************************************************************************
*\file         Scripts_Raycast.h
*\brief        Scripts_Raycast.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"
#include "../Physics/geometry.h"

namespace MONO_RAYCAST
{
	MONO_EXPORT uint32_t rayaab(uint32_t cam_id)
	{
		auto m_obj = PPB.GetEntityInfo(cam_id);
		camera* m_cam = m_obj.m_pArchetype->FindComponent<camera>(m_obj.m_PoolDetails);

		glm::vec3 CamPos{ m_cam->m_Position };
		glm::vec3 RayDir = PPB.GetViewportMousePosition(m_cam->m_Projection, m_cam->m_View);

		glm::vec3 EndPos{ CamPos + RayDir * 20.0f };

		paperback::Vector3f converted_start{ CamPos.x, CamPos.y, CamPos.z };
		paperback::Vector3f converted_end{ EndPos.x, EndPos.y, EndPos.z };
		std::vector<paperback::u32> ExcludeList;

		auto [Hit_ID, HitDist] = PPB.QueryRaycastClosest(converted_start    // Start Ray
													   , converted_end      // End Ray
													   , ExcludeList		// Excluded Entities
													   , true);	

		return Hit_ID == paperback::settings::invalid_index_v ? 0 : Hit_ID;
	}

	void AddInternals()
	{
		mono_add_internal_call("CSScript.Tools.Raycast::rayaab(uint)", &MONO_RAYCAST::rayaab);
	}
}