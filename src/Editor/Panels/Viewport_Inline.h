#pragma once
#include "Viewport.h"
#include "glm/inc/gtc/type_ptr.hpp"
#include "glm/inc/gtx/matrix_decompose.hpp"

void EditorViewport::Panel()
{
	ImGui::Begin(EditorViewport::typedef_v.m_pName, &m_bEnabled, ImGuiWindowFlags_MenuBar);

	ViewportMenuBar();

	ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();

	paperback::u64 TextureID = Renderer::GetInstanced().GetFinalImage();

	ImGui::Image(reinterpret_cast<void*>(TextureID), ViewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	Gizmo();

	ImGui::End();
}

void EditorViewport::ViewportMenuBar()
{
	ImGui::BeginMenuBar();

	if (ImGui::MenuItem(ICON_FA_ARROWS_ALT))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;	
	}
	if (ImGui::MenuItem(ICON_FA_EXPAND_ALT))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
	}

	if (ImGui::MenuItem(ICON_FA_SYNC_ALT))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
	}

	if (ImGui::MenuItem(ICON_FA_BROOM))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = -1;
	}

	ImGui::EndMenuBar();
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
	glm::mat4 CameraView = Camera::GetInstanced().GetView();
	glm::mat4 CameraProjection = Camera::GetInstanced().GetProjection();

	auto EntityDetails = paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second });

	auto bTrans = m_Imgui.m_SelectedEntity.first->FindComponent<transform>(EntityDetails);
	auto bRot = m_Imgui.m_SelectedEntity.first->FindComponent<rotation>(EntityDetails);
	auto bScale = m_Imgui.m_SelectedEntity.first->FindComponent<scale>(EntityDetails);
	auto bChild = m_Imgui.m_SelectedEntity.first->FindComponent<child>(EntityDetails);


	if (bTrans && !bChild) //for normal entities
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

		if (bRot)
		{
			glm::vec3 TempRot = Rot - glm::vec3(glm::radians(bRot->m_Value.x), glm::radians(bRot->m_Value.y), glm::radians(bRot->m_Value.z));

			bRot->m_Value.x += TempRot.x;
			bRot->m_Value.y += TempRot.y;
			bRot->m_Value.z += TempRot.z;
		}

		if (bTrans && !bChild)
			Editor::Math::GlmtoVec3(bTrans->m_Position, Trans);

		//if (bTrans && bChild)
		//	Editor::Math::GlmtoVec3(bTrans->m_Offset, Trans);

		if (bScale)
			Editor::Math::GlmtoVec3(bScale->m_Value, Scale);
	}
}
