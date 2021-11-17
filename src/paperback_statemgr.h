#pragma once
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;
namespace paperback::coordinator
{

	class scene
	{
		std::string m_Name, m_ScenePath, m_InfoPath;

	public:
		PPB_INLINE
		scene(const std::string& Name, const std::string& Path, const std::string& Info); // ctor to set up a scene

		PPB_INLINE
		void Load(); //to be called on loading scene, calls m_EntityManager Load()
		
		PPB_INLINE
		void Unload(); //to be called on unloading scene, calls m_EntityManager Unload()
		
		PPB_INLINE
		void UpdateName(const std::string& Name); // changes name of current scene

		PPB_INLINE
		const std::string& GetName();

		PPB_INLINE
		void UpdatePath(const std::string& Path, const std::string& Info = ""); // changes the path of the current scene
	};

	class scene_mgr
	{
	  std::vector<scene> m_Scenes; // contains all scenes that can be loaded 
	  size_t m_CurrentSceneIndex; // contains pointer to current scene
	  size_t m_NextSceneIndex; // remains nullptr until scene is to change
	  bool m_SceneChange; // bool to check when scene is to be changed

	public:

		PPB_INLINE
		scene_mgr(); // ctor that loads all scenes needed

		PPB_INLINE
		~scene_mgr(); // dtor to ensure current scene is unloaded

		PPB_INLINE
		void AddScene(const std::string& Name, const std::string& Path, const std::string& Info); //manually adds scene to the current list

		PPB_INLINE
		void RemoveScene(const std::string& Name); //manually removes scene

		PPB_INLINE
		void UpdateScene(const std::string& Path, const std::string& Info); //updates path for the current scene

		PPB_INLINE
		void ReloadScene(); //unloads current scene and reloads current scene

		PPB_INLINE
		void SaveScenes(); //deserializes scenes

		PPB_INLINE
		bool TriggerChangeScene(const std::string& Name); // prepares for change of scene, returns false if scene does not exist

		PPB_INLINE
		void ChangeScene(); //begins the scene change process, where current scene is unloaded and new scene is loaded

		PPB_INLINE
		bool VerifyScene(const std::string& Name); // returns a bool confirming if the state is as specified
	};
}