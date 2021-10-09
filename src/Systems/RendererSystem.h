#pragma once
#ifndef _RENDERERSYSTEM_H_
#define _RENDERERSYSTEM_H_

#include "../Functionality/Renderer/Renderer.h"
#include "glm/inc/gtx/transform.hpp"
#include "../Systems/WindowSystem.h"

struct render_system : paperback::system::instance
{
	GLFWwindow* m_pWindow;
	RenderResourceManager* m_Resources;

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

	}

	PPB_FORCEINLINE
	void PreUpdate( void ) noexcept
	{
	}

	PPB_FORCEINLINE
	void Update( void ) noexcept
	{
		//// Populate map to render objects
		std::unordered_map<std::string, std::vector<glm::mat4>> objects;

		//just testing stuff
		tools::query Query;
		Query.m_Must.AddFromComponents<transform, mesh, scale, rotation>();
		
		glm::mat4 t{ 1.0f };
		t = glm::mat4{ 1.0f };
		t = glm::translate(t, glm::vec3{ 0, 0, 0 });
		t = glm::rotate(t, glm::radians(-90.f), glm::vec3{ 1.f, 0.f, 0.f });
		t = glm::scale(t, glm::vec3{ 40,40,40 });

		objects["Quad"].push_back(t);

		ForEach( Search( Query ), [&]( transform& Xform, mesh& Mesh, scale& Scale, rotation& Rotation) noexcept
		{
			glm::mat4 t{ 1.0f };
			t = glm::translate(t, glm::vec3{Xform.m_Position.x, Xform.m_Position.y, Xform.m_Position.z});
			t = glm::rotate(t, glm::radians(Rotation.m_Value.x), glm::vec3{ 1.f, 0.f, 0.f });
			t = glm::rotate(t, glm::radians(Rotation.m_Value.y), glm::vec3{ 0.f, 1.f, 0.f });
			t = glm::rotate(t, glm::radians(Rotation.m_Value.z), glm::vec3{ 0.f, 0.f, 1.f });
			t = glm::scale(t, glm::vec3{ Scale.m_Value.x, Scale.m_Value.y, Scale.m_Value.z });
			objects[Mesh.m_Model].push_back(t);
		});

		auto points = GetSystem<debug_system>().GetPoints();

		Renderer::GetInstanced().Render(objects, &points);
	}

	PPB_FORCEINLINE
	void PostUpdate ( void ) noexcept
	{
		// Swap buffer
		glfwSwapBuffers(m_pWindow);
	}
};

#endif