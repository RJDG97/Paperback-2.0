/**********************************************************************************
*\file         ParenChildSystem.h
*\brief        ParenChildSystem.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

struct parentchild_system : paperback::system::instance
{
	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "parentchild_system"
	};

	PPB_FORCEINLINE
	void Update(void) noexcept
	{
		tools::query Query;

		Query.m_Must.AddFromComponents<parent, transform, rotation, scale>();
		Query.m_NoneOf.AddFromComponents<prefab>();

		ForEach(Search(Query), [&](parent& Parent, transform& Transform, rotation& Rotation, scale& Scale) noexcept
		{
			if (Parent.m_ChildrenGlobalIndexes.size() != 0)
			{
				for (const auto& ChildGlobalIndex : Parent.m_ChildrenGlobalIndexes)
				{
					auto& ChildInfo = GetEntityInfo(ChildGlobalIndex);
					auto [CEntity, CTransform, CRotation, CScale, COffset, CBox] = ChildInfo.m_pArchetype->FindComponents<entity, transform, rotation, scale, offset, boundingbox>(ChildInfo.m_PoolDetails);

					if (COffset)
					{
						if (CTransform) { CTransform->m_Position = COffset->m_PosOffset + Transform.m_Position; }
						if (CRotation) { CRotation->m_Value = COffset->m_RotOffset + Rotation.m_Value; }
						if (CScale) {
							CScale->m_Value = { COffset->m_ScaleOffset.x * Scale.m_Value.x,
												COffset->m_ScaleOffset.y * Scale.m_Value.y,
												COffset->m_ScaleOffset.z * Scale.m_Value.z };
						}
					}

					else
					{
						if (CTransform) { CTransform->m_Position = Transform.m_Position; }
						if (CRotation) { CRotation->m_Value = Rotation.m_Value; }
						if (CScale) { CScale->m_Value = Scale.m_Value; }
					}

					if ( CBox && CTransform && CEntity )
					{
						m_Coordinator.UpdateNode( *CBox, *CTransform, *CEntity );
					}
				}
			}
		});
	}
};