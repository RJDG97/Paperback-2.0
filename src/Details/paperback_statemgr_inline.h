#pragma once
#include "../Functionality/RenderResource/RenderResourceManager.h"

namespace paperback::coordinator
{

	//-----------------------------------
	//            Scene
	//-----------------------------------
	scene::scene(const std::string& Name, const std::string& Path, const std::string& Info, const std::string& Tex) :
		m_Name{ Name },
		m_ScenePath{ Path },
		m_InfoPath{ Info },
		m_TexPath{ Tex }
	{}

	void scene::Load()
	{

		if (m_ScenePath == "" || m_InfoPath == "" || m_TexPath == "")
			return;

		JsonFile Jfile;

		// On Initial State Change - Pre-Load
		PPB.ResetSystems();

		PPB.Initialize();
		Jfile.StartReader(m_ScenePath);
		Jfile.LoadEntities("Entities");
		Jfile.EndReader();

		PPB.ReloadSystems();

		PPB.LoadEntityInfo(m_InfoPath);

		if (!PPB.VerifyState("Editor"))
			PPB.LoadTextures(m_TexPath);
	}

	void scene::Unload()
	{
		if (!PPB.GetArchetypeList().empty())
			PPB.ResetAllArchetypes();
	}

	void scene::UpdateName(const std::string& Name)
	{

		m_Name = Name;
	}

	const std::string& scene::GetName()
	{

		return m_Name;
	}

	void scene::UpdatePath(const std::string& Path, const std::string& Info, const std::string& Tex)
	{

		m_ScenePath = Path;

		if (Info != "")
			m_InfoPath = Info;

		if (Tex != "")
			m_TexPath = Tex;
	}

	//-----------------------------------
	//         Scene Manager
	//-----------------------------------
	scene_mgr::scene_mgr() :
		m_Scenes{},
		m_CurrentSceneIndex{},
		m_NextSceneIndex{},
		m_SceneChange{ false }
	{

		//load scenes here

		JsonFile Jfile;
		std::stringstream buffer{};
		Jfile.StartReader("../../resources/stateloading/StateList.json").LoadStringPairs(buffer).EndReader();

		//process buffer

		std::string name{}, path{}, info{}, tex{};
		while (buffer >> name >> path >> name >> info >> name >> tex)
		{
			AddScene(name, path, info, tex);
			name = path = info = tex = "";
		}
	}

	scene_mgr::~scene_mgr()
	{

		m_Scenes[m_CurrentSceneIndex].Unload();
	}

	void scene_mgr::AddScene(const std::string& Name, const std::string& Path, const std::string& Info, const std::string& Tex)
	{

		m_Scenes.push_back({ Name, Path, Info, Tex });
	}

	void scene_mgr::RemoveScene(const std::string& Name)
	{
	}

	void scene_mgr::UpdateScene(const std::string& Path, const std::string& Info)
	{

		m_Scenes[m_CurrentSceneIndex].UpdatePath(Path, Info);
	}

	void scene_mgr::ReloadScene()
	{

		m_Scenes[m_CurrentSceneIndex].Unload();
		m_Scenes[m_CurrentSceneIndex].Load();
	}

	void scene_mgr::SaveScenes()
	{
	}

	bool scene_mgr::TriggerChangeScene(const std::string& Name)
	{

		size_t index = 0;
		//check if scene exists
		for (; index < m_Scenes.size(); ++index)
		{

			if (m_Scenes[index].GetName() == Name)
			{

				break;
			}
		}

		//if yes, set bool to true and update next scene index
		if (index < m_Scenes.size())
		{

			m_SceneChange = true;
			m_NextSceneIndex = index;
		}
		else
		{

			m_SceneChange = false;
		}

		//else, set false
		return m_SceneChange;
	}

	void scene_mgr::ChangeScene()
	{

		m_Scenes[m_CurrentSceneIndex].Unload();
		m_CurrentSceneIndex = m_NextSceneIndex;
		m_Scenes[m_CurrentSceneIndex].Load();
	}

	bool scene_mgr::VerifyScene(const std::string& Name)
	{
		return (Name == m_Scenes[m_CurrentSceneIndex].GetName());
	}
}