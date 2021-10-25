#pragma once

struct parentchild_system : paperback::system::instance
{
	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "parentchild_system"
	};

	void operator()( parent& Parent, transform& Transform ) noexcept
	{
		const auto& Children = Parent.m_Infos;

		for ( const auto& ChildrenInfo : Children )
		{
			auto& ChildInfo = GetEntityInfo( ChildrenInfo.m_ChildGID );	
			auto [ CTransform, COffset ] = ChildInfo.m_pArchetype->FindComponents<transform, offset>( ChildInfo.m_PoolDetails );

			if ( CTransform && COffset )
			{
				CTransform->m_Position = COffset->m_Value + Transform.m_Position;
			}
		}
	}
};