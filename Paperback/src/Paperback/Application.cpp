#include "PaperbackPCH.h"

#include "Application.h"

namespace Paperback
{
	Ref<Application> app;

	Application::Application() :
		paused{ false }, game_active{ true }
	{
		Log::Init();

		// Constructing Managers
		mSystemManager = CreateScope<SystemManager>();
		mEntityManager = CreateScope<EntityManager>();
		mResourceManager = CreateScope<ResourceManager>();
		mComponentManager = CreateScope<ComponentManager>();

		mEventBus = CreateScope<EventBus>();
		//mTimer = CreateScope<Timer>();

		//// Register Systems
		//RegisterSystem<Pause>();
	}

	Application::~Application()
	{

	}

	void Application::Init()
	{
		mComponentManager->Init();
		mEntityManager->Init();
		mResourceManager->Init();
		mSystemManager->Init();
	}

	void Application::GameLoop()
	{
		while (game_active)
		{
			//mTimer->Tick();
			//float dt = mTimer->DeltaTime();

			for (auto& [id, system] : mSystemManager->systems)
			{
				// Figure a way out to tidy this up
			//#ifdef PAPERBACK_DEBUG
			//	auto starting_time = mTimer->Now();
			//#endif

				system->Update(0.01667f, paused);

			// Figure a way out to tidy this up
			//#ifdef PAPERBACK_DEBUG
			//	system_timer[id] = mTimer->ComputeTime(starting_time);
			//#endif
			}

			++counter;
			if (counter == 10)
				PPB_ASSERT_MSG(counter != 11, "Not equal to 11");
		}
	}

	void Application::Pause(bool status)
	{
		paused = status;
	}

	bool Application::Pause() const
	{
		return paused;
	}

	EntityID Application::CreateEntity()
	{
		return mEntityManager->CreateEntity();
	}

	void Application::DeleteEntity(EntityID id)
	{
		mEntityManager->DestroyEntity(id);
	}

	void Application::DeleteAllEntities()
	{
		mEntityManager->DestroyAllEntities();
	}

	void Application::DeleteArchetype(EntityID id)
	{
		mEntityManager->DestroyArchetype(id);
	}

	void Application::DeleteAllArchetypes()
	{
		mEntityManager->DestroyAllArchetypes();
	}
}