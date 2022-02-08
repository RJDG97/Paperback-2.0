#pragma once

#include "Mono.h"
#include "../Physics/geometry.h"

namespace MONO_RAYCAST
{

	MONO_EXPORT MonoArray* rayaab()
	{
		std::vector<uint32_t> ids{};
		tools::query Query;
		Query.m_Must.AddFromComponents<boundingbox, paperback::component::entity>();
		Query.m_NoneOf.AddFromComponents<prefab>();

		glm::vec3 CamPos = cam::GetInstanced().GetPosition();
		glm::vec3 RayDir = PPB.GetViewportMousePosition();

		PPB.ForEach(PPB.Search(Query), [&](boundingbox& BoundingBox, paperback::component::entity& Entity) noexcept
		{
			float temp = 0; //probs remove and replace the temp below with t, this temp thing makes it so that everything is returned.

			if (RayAabb(paperback::Vector3f{ CamPos.x, CamPos.y, CamPos.z },
						paperback::Vector3f{ RayDir.x, RayDir.y, RayDir.z },
						BoundingBox.Min,
						BoundingBox.Max,
						temp))
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
		mono_add_internal_call("CSScript.Raycast::rayaab()", &MONO_RAYCAST::rayaab);
	}
}