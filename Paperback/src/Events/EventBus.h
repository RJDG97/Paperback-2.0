#pragma once

#include "PaperbackPCH.h"

#include "Paperback/Core.h"

namespace Paperback
{
	class ISystem;


	/****************************************************/
	/*!					 Event Handlers
	/****************************************************/
	class IEventHandler
	{
	public:

		void Exec(Ref<IEvent> event)
		{
			Function(event);
		}

	private:

		virtual void Function(Ref<IEvent> event) = 0;
	};

	template <class EventType>
	class EventHandler final : public IEventHandler
	{
	public:

		EventHandler(Action<Ref<EventType>> _function) :
			function{ _function }
		{ }

		void Function(Ref<IEvent> event) override
		{
			function(std::static_pointer_cast<EventType>(event));
		}

	private:

		Action<Ref<EventType>> function;
	};


	/****************************************************/
	/*!					   Event Bus
	/****************************************************/
	class EventBus final
	{
		using EventList = std::vector<Ref<IEventHandler>>;
		using SubscriberMap = std::unordered_map<std::type_index, Ref<EventList>>;

	public:

		template <typename EventType, typename ...Args>
		void PublishEvent(Args&&... args)
		{
			auto it = subscribers.find(typeid(EventType));

			PPB_ASSERT_MSG(it == subscribers.end(),
				"EventBus<EventType>::PublishEvent() -> EventType does not exist");

			Ref<EventList> events = it->second;
			Ref<EventType> event = CreateRef<EventType>(args...);

			for (auto& _event : *events)
			{
				_event->Exec(event);
			}
		}

		// Subscribed Events are "Sorted" based on their "IEvent" parameters defined within the systems
		template <class EventType>
		void SubscribeEvent(Action<Ref<EventType>> event)
		{
			auto it = subscribers.find(typeid(EventType));

			if (it == subscribers.end())
			{
				auto list = CreateRef<EventList>();
				subscribers[typeid(EventType)] = list;
			}

			subscribers[typeid(EventType)]->push_back(CreateRef<EventHandler<EventType>>(event));
		}

	private:

		SubscriberMap subscribers;
	};
}