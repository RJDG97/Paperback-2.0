#pragma once
#include "../Functionality/Renderer/Renderer.h"
#include "glm/inc/gtx/transform.hpp"
#include "WindowSystem.h"

struct render_system : paperback::system::instance
{
	GLFWwindow* m_pWindow;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "render_system"
	};

	PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
		m_pWindow = GetSystem<window_system>().m_pWindow;
	}

	PPB_FORCEINLINE
	void PreUpdate( void ) noexcept
	{
		Renderer::GetInstanced().StartFrame();
	}

	PPB_FORCEINLINE
	void Update( void ) noexcept
	{
		std::unordered_map<std::string, std::vector<glm::mat4>> objects;

		glm::mat4 t{ 1.0f };
		t = glm::translate(t, glm::vec3{ 0,0,-10 });
		t = glm::scale(t, glm::vec3{ 1,1,1 });

		objects["Backpack"].push_back(t);

		Renderer::GetInstanced().Render(objects);
		
		//just testing stuff
		tools::query Query;
		Query.m_Must.AddFromComponents<transform, mesh>();

		ForEach( Search( Query ), [&]( transform& xform, mesh& mesh) noexcept
		{
			objects.clear();
			glm::mat4 t{ 1.0f };
			t = glm::translate(t, glm::vec3{xform.m_Position.m_X, xform.m_Position.m_Y, xform.m_Position.m_Z});
			t = glm::scale(t, glm::vec3{ 1,1,1 });
			objects[mesh.m_Model].push_back(t);
			Renderer::GetInstanced().Render(objects);
		});
	 
	 // Call render with map
		//Renderer::GetInstanced().Render(objects);
	}

	PPB_FORCEINLINE
	void PostUpdate ( void ) noexcept
	{
		// Composite
		// Post processes

		// Swap buffer
		glfwSwapBuffers(m_pWindow);
	}
};