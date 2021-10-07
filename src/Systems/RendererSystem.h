#pragma once
#ifndef _RENDERERSYSTEM_H_
#define _RENDERERSYSTEM_H_

#include "../Functionality/Renderer/Renderer.h"
#include "glm/inc/gtx/transform.hpp"
#include "../Systems/WindowSystem.h"
#include "../Functionality/Animation/Animator.h"

struct render_system : paperback::system::instance
{
	GLFWwindow* m_pWindow;
	RenderResourceManager* m_Resources;

	Animation animation_test;
	Animator animator_test;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "render_system"
	};

	PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
		m_pWindow = GetSystem<window_system>().m_pWindow;
		m_Resources = &RenderResourceManager::GetInstanced();
		Renderer::GetInstanced().StartFrame();
		animation_test = Animation{ "../../resources/models/mutant.fbx", &m_Resources->m_Models["Character"] };
		animator_test = Animator{ &animation_test };

	}

	PPB_FORCEINLINE
	void PreUpdate( void ) noexcept
	{
	}

	PPB_FORCEINLINE
	void Update( void ) noexcept
	{
		animator_test.UpdateAnimation(DeltaTime());

		//// Populate map to render objects
		std::unordered_map<std::string, std::vector<glm::mat4>> objects;

		glm::mat4 t{ 1.0f };
		t = glm::translate(t, glm::vec3{ -3,2,-3 });
		t = glm::scale(t, glm::vec3{ 0.03,0.03,0.03 });

		objects["Character"].push_back(t);
		
		auto transforms{ animator_test.GetFinalBoneMatrices() };
		
		//just testing stuff
		tools::query Query;
		Query.m_Must.AddFromComponents<transform, mesh>();

		t = glm::mat4{ 1.0f };
		t = glm::translate(t, glm::vec3{ 0, 0, 0 });
		t = glm::rotate(t, glm::radians(-90.f), glm::vec3{ 1.f, 0.f, 0.f });
		t = glm::scale(t, glm::vec3{ 40,40,40 });

		objects["Quad"].push_back(t);

		ForEach( Search( Query ), [&]( transform& xform, mesh& mesh) noexcept
		{
			glm::mat4 t{ 1.0f };
			t = glm::translate(t, glm::vec3{xform.m_Position.m_X, xform.m_Position.m_Y, xform.m_Position.m_Z});
			t = glm::scale(t, glm::vec3{ 1,1,1 });
			objects[mesh.m_Model].push_back(t);
		});
	 
		// Add points to render lines
		//std::vector<glm::vec3> points;

		//points.push_back(glm::vec3{ 0,-5,0 });
		//points.push_back(glm::vec3{ 0,5,0 });

		Renderer::GetInstanced().Render(objects, nullptr, &transforms);
	}

	PPB_FORCEINLINE
	void PostUpdate ( void ) noexcept
	{
		// Swap buffer
		glfwSwapBuffers(m_pWindow);
	}
};

#endif