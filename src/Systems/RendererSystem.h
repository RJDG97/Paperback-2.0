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
	}

	PPB_FORCEINLINE
	void Update( void ) noexcept
	{
		// Populate map to render objects
		std::unordered_map<std::string, std::vector<glm::mat4>> objects;

		//glm::mat4 t{ 1.0f };
		//t = glm::mat4{ 1.0f };
		//t = glm::translate(t, glm::vec3{ 0,-10,-10 });
		//t = glm::scale(t, glm::vec3{ 5,5,5 });

		//objects["Box"].push_back(t);

		//t = glm::mat4{ 1.0f };
		//t = glm::translate(t, glm::vec3{ 0,0,-10 });
		//t = glm::scale(t, glm::vec3{ 1,1,1 });

		//objects["Backpack"].push_back(t);
		//Renderer::GetInstanced().Render(objects);

		//just testing stuff
		tools::query Query;
		Query.m_Must.AddFromComponents<transform, mesh>();

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

		Renderer::GetInstanced().Render(objects);
	}

	PPB_FORCEINLINE
	void PostUpdate ( void ) noexcept
	{
		// Swap buffer
		glfwSwapBuffers(m_pWindow);
	}
};