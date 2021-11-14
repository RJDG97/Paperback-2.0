#pragma once
#include "Viewport.h"
#include "glm/inc/gtc/type_ptr.hpp"
#include "glm/inc/gtx/matrix_decompose.hpp"
#include "../../Physics/geometry.h"

void EditorViewport::Panel()
{
	ImGui::Begin(EditorViewport::typedef_v.m_pName, &m_bEnabled, ImGuiWindowFlags_MenuBar);

	ViewportMenuBar();

	if (ImGui::BeginTabBar("##Viewports"))
	{
		ViewportOne();
		ViewportTwo();

		Gizmo();
		MouseSelection();

		ImGui::EndTabBar();
	}


	ImGui::End();
}

void EditorViewport::ViewportOne()
{
	//Renders the game

	if (ImGui::BeginTabItem("Viewport #1"))
	{
		ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();

		paperback::u64 TextureID = Renderer::GetInstanced().GetFinalImage();

		ImGui::Image(reinterpret_cast<void*>(TextureID), ViewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::EndTabItem();
	}
}

void EditorViewport::ViewportTwo()
{
	// Renders the UI

	if (ImGui::BeginTabItem("Viewport #2"))
	{
		ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();

		paperback::u64 TextureID = Renderer::GetInstanced().GetUIOverlay();

		ImGui::Image(reinterpret_cast<void*>(TextureID), ViewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::EndTabItem();
	}
}

void EditorViewport::ViewportMenuBar()
{
	ImGui::BeginMenuBar();

	if (ImGui::Button(ICON_FA_ARROWS_ALT))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;	
	}
	if (ImGui::Button(ICON_FA_EXPAND_ALT))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
	}

	if (ImGui::Button(ICON_FA_SYNC_ALT))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
	}

	if (ImGui::Button(ICON_FA_BROOM))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = -1;
	}

	ImGui::EndMenuBar();
}

void EditorViewport::MouseSelection()
{
	ImVec2 Min = ImGui::GetWindowContentRegionMin();
	ImVec2 Max = ImGui::GetWindowContentRegionMax();

	Min.x += ImGui::GetWindowPos().x;
	Min.y += ImGui::GetWindowPos().y;
	Max.x += ImGui::GetWindowPos().x;
	Max.y += ImGui::GetWindowPos().y;

	ImVec2 ScreenPos = ImGui::GetCursorScreenPos();
	ImVec2 CursorPos = ImGui::GetCursorPos(); //base on the API tis one is coords in world coordinates?

	if (PPB.IsMousePress(GLFW_MOUSE_BUTTON_LEFT))
	{
		if (!PPB.GetArchetypeList().empty())
		{
			for (auto& Archetype : PPB.GetArchetypeList())
			{
				for (paperback::u32 i = 0; i < Archetype->GetCurrentEntityCount(); ++i)
				{
					auto EntityBB = Archetype->FindComponent<boundingbox>(paperback::vm::PoolDetails{ 0, i });
					auto EntityPos = Archetype->FindComponent<transform>(paperback::vm::PoolDetails{ 0, i });
					auto Entity = Archetype->FindComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, i });

					if (!Archetype->GetComponentBits().Has(paperback::component::info_v<prefab>.m_UID)) //is not a prefab entity
					{
						if (EntityBB) //if the entity has boundingbox
						{
							glm::vec3 CamPos, RayDir;
							float t = 0.0f;

							CamPos = Camera3D::GetInstanced().GetPosition();
							RayDir = PPB.GetMousePosition();

							if (RayAabb({ CamPos.x, CamPos.y, CamPos.z }, { RayDir.x, RayDir.y, RayDir.z }, EntityPos->m_Position + EntityBB->Min, EntityPos->m_Position + EntityBB->Max, t))
							{
								// if collide 
								std::cout << Entity->m_GlobalIndex << std::endl;
							}
						}

					}
				}
			}
		}
	}
}

void EditorViewport::Gizmo()
{
	if (m_Imgui.m_SelectedEntity.first) //An entity is selected	
	{
		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetDrawlist();

		float WindowWidth = (float)ImGui::GetWindowWidth();
		float WindowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, WindowWidth, WindowHeight); //setting the viewport

		ComposeTransform();
	}
}

void EditorViewport::ComposeTransform()
{
	glm::mat4 model{ 1.0f };
	glm::vec3 Trans, Rot, Scale;

	//Camera
	glm::mat4 CameraView = Camera3D::GetInstanced().GetView();
	glm::mat4 CameraProjection = Camera3D::GetInstanced().GetProjection();

	auto EntityDetails = paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second });

	auto bTrans = m_Imgui.m_SelectedEntity.first->FindComponent<transform>(EntityDetails);
	auto bRot = m_Imgui.m_SelectedEntity.first->FindComponent<rotation>(EntityDetails);
	auto bScale = m_Imgui.m_SelectedEntity.first->FindComponent<scale>(EntityDetails);
	auto bChild = m_Imgui.m_SelectedEntity.first->FindComponent<child>(EntityDetails);


	if (bTrans) //for normal entities
		model = glm::translate(model, glm::vec3(bTrans->m_Position.x, bTrans->m_Position.y, bTrans->m_Position.z));

	//else if (bTrans && bChild)
	//{
	//	auto Combi = bTrans->m_Offset + bTrans->m_Position;
	//	model = glm::translate(model, glm::vec3(Combi.x, Combi.y, Combi.z));
	//}
	//else
	//	model = glm::translate(model, glm::vec3(1, 1, 1));


	if (bRot)
	{
		model = glm::rotate(model, glm::radians(bRot->m_Value.x), glm::vec3(1, 0, 0));//rotation x = 0.0 degrees
		model = glm::rotate(model, glm::radians(bRot->m_Value.y), glm::vec3(0, 1, 0));//rotation y = 0.0 degrees
		model = glm::rotate(model, glm::radians(bRot->m_Value.z), glm::vec3(0, 0, 1));//rotation z = 0.0 degrees
	}
	else
	{
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));//rotation x = 0.0 degrees
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 1, 0));//rotation y = 0.0 degrees
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 0, 1));//rotation z = 0.0 degrees
	}

	if (bScale)
		model = glm::scale(model, glm::vec3(bScale->m_Value.x, bScale->m_Value.y, bScale->m_Value.z));
	else
		model = glm::scale(model, glm::vec3(1, 1, 1));//scale = 1,1,1

	ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(CameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(model));

	if (ImGuizmo::IsUsing())
	{
		Editor::Math::DecomposeTransform(model, Trans, Scale, Rot);

		if (bTrans)
			Editor::Math::GlmtoVec3(bTrans->m_Position, Trans);

		//if (bRot)
		//{
		//	glm::vec3 TempRot = Rot - glm::vec3(glm::radians(bRot->m_Value.x), glm::radians(bRot->m_Value.y), glm::radians(bRot->m_Value.z));

		//	bRot->m_Value.x += TempRot.x;
		//	bRot->m_Value.y += TempRot.y;
		//	bRot->m_Value.z += TempRot.z;
		//}

		if (bScale)
			Editor::Math::GlmtoVec3(bScale->m_Value, Scale);
	}
}
