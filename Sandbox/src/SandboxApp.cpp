#include <Paperback.h>

#ifdef PAPERBACK_PLATFORM_WINDOWS

#include "Systems/Pause.h"

using namespace Paperback;

Ref<Paperback::Application> Engine;

int main(int args, char** argv)
{
	try
	{
		Engine = CreateApplication();
		DEBUG_PRINT("created app");

		Engine->RegisterSystem<Sandbox::Pause>();

		Engine->Init();


		// Temporary method to create entities
		for (size_t i = 0; i < 20; ++i)
		{
			TRACE_PRINT("Creating entity!");
			EntityID id = Engine->CreateEntity();

			Engine->AddComponentToEntity(id, CreateRef<Transform>());
		}


		Engine->GameLoop();
	}
	catch (const std::exception& e)
	{
		std::string msg = e.what();
		ERROR_LOG(msg);
	}
	catch (...)
	{
		
	}
}

#endif