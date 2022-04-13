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

	MONO_EXPORT MonoArray* rayaab(uint32_t cam_id)
	{
		std::vector<uint32_t> ids{};
		tools::query Query;
		Query.m_Must.AddFromComponents<boundingbox, transform, paperback::component::entity>();
		Query.m_NoneOf.AddFromComponents<prefab>();

		auto m_obj = PPB.GetEntityInfo(cam_id);
		camera* m_cam = m_obj.m_pArchetype->FindComponent<camera>(m_obj.m_PoolDetails);

		glm::vec3 CamPos { m_cam->m_Position };
		glm::vec3 RayDir = PPB.GetViewportMousePosition(m_cam->m_Projection, m_cam->m_View);

		PPB.ForEach(PPB.Search(Query), [&](boundingbox& BoundingBox, transform& Transform, paperback::component::entity& Entity) noexcept
		{
			float t = 0;
			if (RayAabb(paperback::Vector3f{ CamPos.x, CamPos.y, CamPos.z },
						paperback::Vector3f{ RayDir.x, RayDir.y, RayDir.z },
						Transform.m_Position + BoundingBox.Min,
						Transform.m_Position + BoundingBox.Max,
						t))
			{
				ids.push_back(Entity.m_GlobalIndex);
			}
		});

		MonoArray* temp_array = mono_array_new(mono_domain_get(), mono_get_uint32_class(), ids.size());

		for (size_t i = 0 ; i != ids.size() ; ++i)
		{
			mono_array_set(temp_array, uint32_t, i, ids[i]);
		}

		return temp_array;
	}

	void AddInternals()
	{
		mono_add_internal_call("CSScript.Tools.Raycast::rayaab(uint)", &MONO_RAYCAST::rayaab);
	}
}