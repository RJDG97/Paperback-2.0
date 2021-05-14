#include "PaperbackPCH.h"
#include "SandboxApp.h"
#include "Pause.h"
#include "Components/Transform.h"

#include "Events/TestEvents.h"

using namespace Paperback;

namespace Sandbox
{
	Pause::Pause()
	{
		test_remove = true;
	}

	void Pause::Init()
	{
		Signature signature;
		signature.set(Engine->GetComponentType<Paperback::Transform>());

		Engine->SetSignature<Pause>(signature);

		Action<Ref<RandomEvent>> fu = PPB_BIND(Pause::TestRandomEvent);
		Engine->SubscribeEvent(fu);

		Action<Ref<CustomEvent>> ck = PPB_BIND(Pause::TestCustomEvent);
		Engine->SubscribeEvent(ck);
	}

	void Pause::Update(float frametime, bool paused)
	{
		if (paused) return;

		for (auto& id : list)
		{
			if (!test_remove)
				continue;

			if (test_remove)
			{
				test_remove = false;

				// Testing Event Publishing
				Engine->PublishEvent<RandomEvent>(id);
				Engine->PublishEvent<CustomEvent>(id);

				// Testing Print & Log
				DEBUG_PRINT("Removed Component from Entity " + std::to_string(id));
				DEBUG_LOG("Removed Component from Entity " + std::to_string(id));

				Engine->RemoveComponentFromEntity<Paperback::Transform>(id);

				// Test print
				for (auto& entity : list)
				{
					TRACE_PRINT(std::to_string(entity));
					TRACE_LOG(std::to_string(entity));

					auto transform = Engine->GetComponent<Paperback::Transform>(entity);

					if (transform)
					{
						TRACE_PRINT("Valid pointer");
						CRITICAL_LOG("Valid pointer");
					}
					else
					{
						TRACE_PRINT("Invalid pointer");
						ERROR_LOG("Invalid pointer");
					}
				}

				return;
			}
		}
	}

	void Pause::TestRandomEvent(Ref<RandomEvent> event)
	{
		(void)event;
		DEBUG_PRINT("Test Random Event");
	}

	void Pause::TestCustomEvent(Ref<CustomEvent> event)
	{
		(void)event;
		INFO_PRINT("Test Custom Event");
	}
}