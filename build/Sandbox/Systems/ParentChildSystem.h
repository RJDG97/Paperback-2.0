#pragma once

struct parentchild_system : paperback::system::instance
{
	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "parentchild_system"
	};

	void operator()( parent& Parent, transform& Transform, rotation& Rotation, scale& Scale ) noexcept
	{
		if (Parent.m_ChildrenGlobalIndexes.size() != 0)
		{
			for (const auto& ChildGlobalIndex : Parent.m_ChildrenGlobalIndexes)
			{
				auto& ChildInfo = GetEntityInfo(ChildGlobalIndex);
				auto [CTransform, CRotation, CScale, COffset] = ChildInfo.m_pArchetype->FindComponents<transform, rotation, scale, offset>(ChildInfo.m_PoolDetails);

				if (COffset)
				{
					if (CTransform) { CTransform->m_Position = COffset->m_PosOffset + Transform.m_Position; }
					if (CRotation) { CRotation->m_Value = COffset->m_RotOffset + Rotation.m_Value; }
					if (CScale) { CScale->m_Value = {COffset->m_ScaleOffset.x * Scale.m_Value.x,
													 COffset->m_ScaleOffset.y * Scale.m_Value.y,
													 COffset->m_ScaleOffset.x * Scale.m_Value.z }; }
				}

				else
				{
					if (CTransform) { CTransform->m_Position = Transform.m_Position; }
					if (CRotation) { CRotation->m_Value = Rotation.m_Value; }
					if (CScale) { CScale->m_Value = Scale.m_Value; }
				}
			}
		}
	}
};