#pragma once
#ifndef _RENDERERSYSTEM_H_
#define _RENDERERSYSTEM_H_

#include "../Functionality/Renderer/Renderer.h"
#include "../Functionality/Camera/Camera.h"
#include "glm/inc/gtx/transform.hpp"
#include "../Systems/WindowSystem.h"
#include "Math/Math_includes.h"

#include <iostream>

struct render_system : paperback::system::instance
{
	GLFWwindow* m_pWindow;
	RenderResourceManager* m_Resources;
	Camera2D m_Camera2D;
	Camera3D m_Camera3D;

	tools::query Query;
	tools::query QueryText;
	tools::query QueryCamera;

	bool m_bGamma = true;


	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "render_system"
	};

	PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
		RegisterGlobalEventClass<paperback::input::manager::KeyPressed>(this);      // Held Down - Not Released ( Passes False )
		RegisterGlobalEventClass<paperback::input::manager::KeyClicked>(this);      // Released                 ( Passes True )
		RegisterGlobalEventClass<paperback::input::manager::MousePressed>(this);    // Held Down - Not Released ( Passes False )
		RegisterGlobalEventClass<paperback::input::manager::MouseClicked>(this);

		const auto& window = GetSystem<window_system>();
		Renderer::GetInstanced().SetUpFramebuffer(window.E.m_Width, window.E.m_Height);
		m_pWindow = window.m_pWindow;
		m_Resources = &RenderResourceManager::GetInstanced();

		Query.m_Must.AddFromComponents<transform, mesh, scale, rotation>();
		Query.m_OneOf.AddFromComponents<animator, socketed, mesh>();
		Query.m_NoneOf.AddFromComponents<prefab>();

		QueryText.m_Must.AddFromComponents<transform, text, scale, rotation>();
		QueryText.m_NoneOf.AddFromComponents<prefab>();

		QueryCamera.m_Must.AddFromComponents<transform, camera>();
		QueryCamera.m_NoneOf.AddFromComponents<prefab>();
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
		std::map<float, std::vector<Renderer::UIInfo>> uis;
		std::unordered_map<std::string_view, std::vector<Renderer::TextInfo>> texts;

		// Populate map
		ForEach( Search( Query ), [&]( transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation, animator* Animator, socketed* Socketed) noexcept
		{

			if (!Mesh.m_Active)
				return;

			if (Mesh.m_Model != "Quad")
			{
				glm::mat4 t{ 1.0f };
				t = glm::translate(t, glm::vec3{ Transform.m_Position.x, Transform.m_Position.y, Transform.m_Position.z });
				t = glm::rotate(t, glm::radians(Rotation.m_Value.x), glm::vec3{ 1.f, 0.f, 0.f });
				t = glm::rotate(t, glm::radians(Rotation.m_Value.y), glm::vec3{ 0.f, 1.f, 0.f });
				t = glm::rotate(t, glm::radians(Rotation.m_Value.z), glm::vec3{ 0.f, 0.f, 1.f });
				t = glm::scale(t, glm::vec3{ Scale.m_Value.x, Scale.m_Value.y, Scale.m_Value.z });

				Renderer::TransformInfo transform_info{ t };

				transform_info.m_ShadowBias = Mesh.m_Bias;

				if (Socketed)
					transform_info.m_ParentSocketTransform = &Socketed->m_BoneTransform;

				if (Animator)
					transform_info.m_BoneTransforms = &Animator->m_FinalBoneMatrices;

				objects[Mesh.m_Model].push_back(transform_info);
			}
			else
			{
				glm::mat4 t{ 1.0f };
				t = glm::translate(t, glm::vec3{ Transform.m_Position.x, Transform.m_Position.y, Transform.m_Position.z });
				t = glm::rotate(t, glm::radians(Rotation.m_Value.x), glm::vec3{ 1.f, 0.f, 0.f });
				t = glm::rotate(t, glm::radians(Rotation.m_Value.y), glm::vec3{ 0.f, 1.f, 0.f });
				t = glm::rotate(t, glm::radians(Rotation.m_Value.z), glm::vec3{ 0.f, 0.f, 1.f });
				t = glm::scale(t, glm::vec3{ Scale.m_Value.x, Scale.m_Value.y, Scale.m_Value.z });

				Renderer::UIInfo ui_info{ t, Mesh.m_Texture };

				uis[Transform.m_Position.z].push_back(ui_info);
			}
		});

		ForEach(Search(QueryText), [&](transform& Transform, text& Text, scale& Scale, rotation& Rotation) noexcept
		{
			glm::mat4 transform{ 1.0f };
			transform = glm::translate(transform, glm::vec3{ Transform.m_Position.x, Transform.m_Position.y, Transform.m_Position.z });
			transform = glm::rotate(transform, glm::radians(Rotation.m_Value.x), glm::vec3{ 1.f, 0.f, 0.f });
			transform = glm::rotate(transform, glm::radians(Rotation.m_Value.y), glm::vec3{ 0.f, 1.f, 0.f });
			transform = glm::rotate(transform, glm::radians(Rotation.m_Value.z), glm::vec3{ 0.f, 0.f, 1.f });
			transform = glm::scale(transform, glm::vec3{ Scale.m_Value.x * Text.m_FontSize, Scale.m_Value.y * Text.m_FontSize, Scale.m_Value.z });

			glm::vec3 color{ Text.m_Color.x, Text.m_Color.y, Text.m_Color.z };
			color /= 255.f;

			Renderer::TextInfo text_info{ transform, color, Text.m_Text };

			texts[Text.m_Font].push_back(text_info);
		});

		auto points = GetSystem<debug_system>().GetPoints();

		Camera3D cam = m_Camera3D;

		if (!PPB.GetPauseBool())
		{
			bool found = false;

			// Populate map
			ForEach(Search(QueryCamera), [&](transform& Transform, camera& Cam) noexcept
			{
				if (found || !Cam.m_Active)
					return;

				glm::vec3 position = Cam.m_Position;
				glm::mat4 view = Cam.m_View;
				glm::mat4 projection = Cam.m_Projection;

				cam = Camera3D{ position, view, projection };

				found = true;
			});
		}


		Renderer::GetInstanced().Render(objects, cam, m_bGamma, uis, texts, m_Camera2D, &points);
	}

	// On Event Key / Mouse Pressed
	void OnEvent(const size_t& Key, const bool& Clicked) noexcept
	{
		if ( !IsGameActive() )
		{
			if (Key == GLFW_MOUSE_BUTTON_RIGHT && !Clicked && !IsGameActive() )
			{
				auto direction = GetMouseDirection();

				m_Camera3D.RotateWithMouse(direction);
			}

			if (Key == GLFW_KEY_W)
			{
				m_Camera3D.MoveForward();
			}
			else if (Key == GLFW_KEY_S)
			{
				m_Camera3D.MoveBackward();
			}

			if (Key == GLFW_KEY_A)
			{
				m_Camera3D.MoveLeft();
			}
			else if (Key == GLFW_KEY_D)
			{
				m_Camera3D.MoveRight();
			}
		}
	}

	PPB_FORCEINLINE
	void PostUpdate ( void ) noexcept
	{
		// Swap buffer
		glfwSwapBuffers(m_pWindow);
	}
};

#endif