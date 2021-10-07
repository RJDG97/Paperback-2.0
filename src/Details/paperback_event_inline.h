#pragma once

namespace paperback::event
{
	/****************************************************/
	/*!					 Event Handlers
	/****************************************************/
	void handler_interface::Exec( std::shared_ptr<paperback::event::instance> Event ) noexcept
	{
		Function( Event );
	}

	template < class EventType >
	handler<EventType>::handler( Action< std::shared_ptr<EventType> > Function ) noexcept :
		m_Function{ Function }
	{ }

	template < class EventType >
	void handler<EventType>::Function( std::shared_ptr<paperback::event::instance> event ) noexcept
	{
		m_Function( std::static_pointer_cast< EventType >( event ) );
	}


	/****************************************************/
	/*!					   Event Bus
	/****************************************************/
	template < typename EventType, typename ...Args >
	void dispatcher::PublishEvent( Args&&... args ) noexcept
	{
		auto it = m_Subscribers.find( typeid(EventType) );

		// Replace this assertion with a log instead and return
		if ( it == m_Subscribers.end() )
			ERROR_LOG( "PublishEvent: EventType does not exist" );

		std::shared_ptr<EventList> EventsList = it->second;
		std::shared_ptr<EventType> EventDetails = std::make_shared< EventType >( args... );

		for ( auto& SubscribedEvent : *EventsList )
		{
			SubscribedEvent->Exec( EventDetails );
		}
	}

	template <class EventType>
	void dispatcher::SubscribeEvent( Action< std::shared_ptr< EventType > > event ) noexcept
	{
		auto it = m_Subscribers.find( typeid( EventType ) );

		if ( it == m_Subscribers.end() )
		{
			auto list = CreateRef<EventList>();
			m_Subscribers[ typeid(EventType) ] = list;
		}

		m_Subscribers[ typeid(EventType) ]->push_back( std::make_shared< EventHandler< EventType > >( event ) );
	}
}