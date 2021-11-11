#pragma once

//struct onevent_update_health : paperback::system::instance
//{
//	constexpr static auto typedef_v = paperback::system::type::system_event<collision_system, collision_system::DamageEvent>
//	{
//		.m_pName = "OnEvent Update Health"
//	};
//
//	void operator()( entity& Entity ) noexcept
//	{
//	}
//
//	void OnEvent( entity& Receiver, entity& Dealer ) noexcept
//    {
//		const auto ReceiverInfo = GetEntityInfo( Receiver.m_GlobalIndex );
//		auto ReceiverHealth     = ReceiverInfo.m_pArchetype->FindComponent<health>( ReceiverInfo.m_PoolDetails );
//
//		const auto DealerInfo = GetEntityInfo( Dealer.m_GlobalIndex );
//		auto DealerDamage     = DealerInfo.m_pArchetype->FindComponent<damage>( DealerInfo.m_PoolDetails );
//
//		if ( ReceiverHealth && DealerDamage )
//		{
//			ReceiverHealth->m_CurrentHealth -= DealerDamage->m_Value;
//			if ( ReceiverHealth->m_CurrentHealth <= 0 )
//				DeleteEntity( Receiver );
//		}
//    }
//};