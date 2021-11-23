#pragma once
#ifndef _RENDERERSYSTEM_H_
#define _RENDERERSYSTEM_H_

#include "../Functionality/Renderer/Renderer.h"
#include "glm/inc/gtx/transform.hpp"
#include "../Systems/WindowSystem.h"
#include "Math/Math_includes.h"

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
		const auto& window = GetSystem<window_system>();
		Renderer::GetInstanced().SetUpFramebuffer(window.E.m_Width, window.E.m_Height);
		m_pWindow = window.m_pWindow;
		m_Resources = &RenderResourceManager::GetInstanced();
	}

	PPB_FORCEINLINE
	void PreUpdate( void ) noexcept
	{
		//auto& window = GetSystem<window_system>();

		//if (window.m_UpdateResolution)
		//{
		//	Renderer::GetInstanced().UpdateFramebufferSize(window.m_Width, window.m_Height);
		//	window.m_UpdateResolution = false;
		//}
	}

	PPB_FORCEINLINE
	void Update( void ) noexcept
	{
		// Populate map to render objects
		std::unordered_map<std::string_view, std::vector<Renderer::TransformInfo>> objects;
		std::unordered_map<std::string_view, std::vector<glm::mat4>> uis;

		// Reference quad
		//glm::mat4 t{ 1.0f };
		//t = glm::mat4{ 1.0f };
		//t = glm::translate(t, glm::vec3{ 0, 0, 0 });
		//t = glm::rotate(t, glm::radians(-90.f), glm::vec3{ 1.f, 0.f, 0.f });
		//t = glm::scale(t, glm::vec3{ 40,40,40 });

		//objects["Quad"].push_back({ t });

		// Populate map
		tools::query Query;
		Query.m_Must.AddFromComponents<transform, mesh, scale, rotation>();
		Query.m_OneOf.AddFromComponents<animator, socketed, mesh>();
		Query.m_NoneOf.AddFromComponents<prefab>();

		ForEach( Search( Query ), [&]( transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation, animator* Animator, socketed* Socketed) noexcept
		{
			glm::mat4 t{ 1.0f };
			t = glm::translate(t, glm::vec3{Transform.m_Position.x, Transform.m_Position.y, Transform.m_Position.z});
			t = glm::rotate(t, glm::radians(Rotation.m_Value.x), glm::vec3{ 1.f, 0.f, 0.f });
			t = glm::rotate(t, glm::radians(Rotation.m_Value.y), glm::vec3{ 0.f, 1.f, 0.f });
			t = glm::rotate(t, glm::radians(Rotation.m_Value.z), glm::vec3{ 0.f, 0.f, 1.f });
			t = glm::scale(t, glm::vec3{ Scale.m_Value.x, Scale.m_Value.y, Scale.m_Value.z });

			Renderer::TransformInfo transform_info{ t };

			if (Socketed)
				transform_info.m_ParentSocketTransform = &Socketed->m_BoneTransform;

			if (Animator)
				transform_info.m_BoneTransforms = &Animator->m_FinalBoneMatrices;

			if(Mesh.m_Model != "Quad")
				objects[Mesh.m_Model].push_back(transform_info);
			else
				uis[Mesh.m_Texture].push_back(t);
		});

		std::vector<paperback::Spline::SplinePoint> spline_points;
		spline_points.push_back({ { -5.0f, 2.0f, -5.0f } });
		spline_points.push_back({{ 4.0f, 2.0f, -6.0f }});
		spline_points.push_back({{ 7.0f, 5.0f, -5.0f }});
		spline_points.push_back({{ 7.5f, 5.5f, -4.0f }});
		spline_points.push_back({{ 8.0f, 3.0f, -5.0f }});
		
		std::vector<paperback::Vector3f> control_points;
		control_points.push_back({ -5.0f, 2.1f, -5.0f }); control_points.push_back({ -5.0f, 1.9f, -5.0f });
		control_points.push_back({ 4.0f, 2.1f, -5.0f }); control_points.push_back({ 4.0f, 1.9f, -5.0f });
		control_points.push_back({ 7.0f, 5.1f, -5.0f }); control_points.push_back({ 7.0f, 4.9f, -5.0f });
		control_points.push_back({ 7.5f, 5.4f, -4.0f }); control_points.push_back({ 7.5f, 5.6f, -4.0f });
		control_points.push_back({ 8.0f, 3.1f, -5.0f }); control_points.push_back({ 8.0f, 2.9f, -5.0f });
		GetSystem<debug_system>().DrawDebugLines(control_points, true);

		paperback::Spline spline{ spline_points, true };

		for (float i = 0.0f ; i < static_cast<float>(spline_points.size()) - 0.05f ; i += 0.05f)
		{
			std::vector<paperback::Vector3f> render_points;
			render_points.push_back(spline.GetSplinePoint(i).m_Point);
			render_points.push_back(spline.GetSplinePoint(i + 0.05f).m_Point);
			GetSystem<debug_system>().DrawDebugLines(render_points, false);
		}

		for (float i = 0.0f; i < static_cast<float>(spline_points.size()); i += 0.3f)
		{
			std::vector<paperback::Vector3f> render_points;
			render_points.push_back(spline.GetSplinePoint(i).m_Point);
			render_points.push_back(spline.GetSplinePoint(i).m_Point + spline.GetSplineGradient(i).m_Point.Normalized());
			GetSystem<debug_system>().DrawDebugLines(render_points, true);
		}

		auto points = GetSystem<debug_system>().GetPoints();

		Renderer::GetInstanced().Render(objects, uis, &points);
	}

	PPB_FORCEINLINE
	void PostUpdate ( void ) noexcept
	{
		// Swap buffer
		glfwSwapBuffers(m_pWindow);
	}
};

#endif