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
		// Populate map to render objects
		std::unordered_map<std::string, std::vector<glm::mat4>> objects;

		glm::mat4 t{ 1.0f };
		t = glm::translate(t, glm::vec3{ 0,0,-10 });
		t = glm::scale(t, glm::vec3{ 1,1,1 });

		objects["Backpack"].push_back(t);

		Renderer::GetInstanced().Render(objects);

		// Add points to render lines
		//std::vector<glm::vec3> points;

		//points.push_back(glm::vec3{ 0,-5,0 });
		//points.push_back(glm::vec3{ 0,5,0 });

		//Renderer::GetInstanced().DebugRender(points);
		//tool::query Query;
		//Query.m_Must.AddFromComponents<transform, mesh>();

		//ForEach( Search( Query ), [&]( transform& xform, mesh& mesh ) noexcept
		//{
		//	Make map of [Object][Vector<Mat4>]
		//}
	 //
	 // Call render with map
		//Renderer::GetInstance().Render(Map);
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