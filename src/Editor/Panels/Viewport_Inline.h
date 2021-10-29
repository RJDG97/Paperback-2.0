#pragma once
#include "Viewport.h"
#include "glm/inc/gtc/type_ptr.hpp"
#include "glm/inc/gtc/quaternion.hpp"

void EditorViewport::Panel()
{
	ImGui::Begin(EditorViewport::typedef_v.m_pName, &m_bEnabled);

	ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();

	paperback::u64 TextureID = Renderer::GetInstanced().GetFinalImage();

	ImGui::Image(reinterpret_cast<void*>(TextureID), ViewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	Gizmo();

	ImGui::End();
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

		//Camera
		glm::mat4 CameraView = Camera::GetInstanced().GetView();
		glm::mat4 CameraProjection = Camera::GetInstanced().GetProjection();

		//Entity
		//Get Transform component

		//transform EntityTransform = m_Imgui.m_SelectedEntity.first->GetComponent<transform>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));
		//scale& EntityScale = m_Imgui.m_SelectedEntity.first->GetComponent<scale>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));
		//rotation& EntityRotation = m_Imgui.m_SelectedEntity.first->GetComponent<rotation>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));


		auto EntityDetails = paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second });
		glm::mat4 model{ 1.0f };

		//if (m_Imgui.m_SelectedEntity.first->FindComponent<transform>(EntityDetails) != nullptr)
		//{
		//	transform EntityTransform = m_Imgui.m_SelectedEntity.first->GetComponent<transform>(EntityDetails);
		//	model = glm::translate(model, glm::vec3( EntityTransform.m_Position.x, EntityTransform.m_Position.y, EntityTransform.m_Position.z ));
		//}
		//else
		//	model = glm::translate(model, glm::vec3( 0, 0, 0 )); //position = 0,0,0

		//if (m_Imgui.m_SelectedEntity.first->FindComponent<rotation>(EntityDetails) != nullptr)
		//{
		//	rotation EntityRotate = m_Imgui.m_SelectedEntity.first->GetComponent<rotation>(EntityDetails);
		//	model = glm::rotate(model, glm::radians(EntityRotate.m_Value.x), glm::vec3(1, 0, 0));//rotation x = 0.0 degrees
		//	model = glm::rotate(model, glm::radians(EntityRotate.m_Value.y), glm::vec3(0, 1, 0));//rotation y = 0.0 degrees
		//	model = glm::rotate(model, glm::radians(EntityRotate.m_Value.z), glm::vec3(0, 0, 1));//rotation z = 0.0 degrees
		//}
		//else
		//{
		//	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));//rotation x = 0.0 degrees
		//	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 1, 0));//rotation y = 0.0 degrees
		//	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 0, 1));//rotation z = 0.0 degrees
		//}

		//if (m_Imgui.m_SelectedEntity.first->FindComponent<scale>(EntityDetails) != nullptr)
		//{
		//	scale EntityScale = m_Imgui.m_SelectedEntity.first->GetComponent<scale>(EntityDetails);
		//	model = glm::scale(model, glm::vec3(EntityScale.m_Value.x, EntityScale.m_Value.y, EntityScale.m_Value.z));
		//}
		//else 
		//	model = glm::scale(model, glm::vec3(1, 1, 1));//scale = 1,1,1

		bool bTrans = false, bScale = false, bRot = false;

		if (m_Imgui.m_SelectedEntity.first->FindComponent<transform>(EntityDetails) != nullptr)
			bTrans = true;
		if (m_Imgui.m_SelectedEntity.first->FindComponent<rotation>(EntityDetails) != nullptr)
			bRot = true;
		if (m_Imgui.m_SelectedEntity.first->FindComponent<scale>(EntityDetails) != nullptr)
			bScale = true;
		
		if (bTrans && bScale && bRot)
		{
			transform& EntityTransform = m_Imgui.m_SelectedEntity.first->GetComponent<transform>(EntityDetails);
			model = glm::translate(model, glm::vec3(EntityTransform.m_Position.x, EntityTransform.m_Position.y, EntityTransform.m_Position.z));

			rotation& EntityRotate = m_Imgui.m_SelectedEntity.first->GetComponent<rotation>(EntityDetails);
			model = glm::rotate(model, glm::radians(EntityRotate.m_Value.x), glm::vec3(1, 0, 0));//rotation x = 0.0 degrees
			model = glm::rotate(model, glm::radians(EntityRotate.m_Value.y), glm::vec3(0, 1, 0));//rotation y = 0.0 degrees
			model = glm::rotate(model, glm::radians(EntityRotate.m_Value.z), glm::vec3(0, 0, 1));//rotation z = 0.0 degrees

			scale& EntityScale = m_Imgui.m_SelectedEntity.first->GetComponent<scale>(EntityDetails);
			model = glm::scale(model, glm::vec3(EntityScale.m_Value.x, EntityScale.m_Value.y, EntityScale.m_Value.z));


			ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(CameraProjection), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL, glm::value_ptr(model));

			if (ImGuizmo::IsUsing())
			{
				EntityTransform.m_Position.x = model[3].x;
				EntityTransform.m_Position.y = model[3].y;
				EntityTransform.m_Position.z = model[3].z;
			}

		}


	}
}

void EditorViewport::ComposeTransform()
{
	bool bTrans = false, bScale = false, bRot = false;
	glm::mat4 model{ 1.0f };
	auto EntityDetails = paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second });

	if (m_Imgui.m_SelectedEntity.first->FindComponent<transform>(EntityDetails) != nullptr)
		bTrans = false;
	if (m_Imgui.m_SelectedEntity.first->FindComponent<rotation>(EntityDetails) != nullptr)
		bRot = false;
	if (m_Imgui.m_SelectedEntity.first->FindComponent<scale>(EntityDetails) != nullptr)
		bScale = false;

	if (bTrans && bRot && bScale)
	{

	}

	if (bTrans && !bRot && !bScale)
	{

	}


}

glm::mat4 EditorViewport::DecomposeTransform(const glm::vec4& Transform, glm::vec3& Translate, glm::vec3& Scale, glm::vec3& Rotation)
{
//	using namespace glm;
//	using T = float;
//
//	mat4 LocalMatrix(transform);
//
//	// Normalize the matrix.
//	if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
//		return false;
//
//	// First, isolate perspective.  This is the messiest.
//	if (
//		epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
//		epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
//		epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
//	{
//		// Clear the perspective partition
//		LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
//		LocalMatrix[3][3] = static_cast<T>(1);
//	}
//
//	// Next take care of translation (easy).
//	Translate = vec3(LocalMatrix[3]);
//	LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);
//
//	vec3 Row[3], Pdum3;
//
//	// Now get scale and shear.
//	for (length_t i = 0; i < 3; ++i)
//		for (length_t j = 0; j < 3; ++j)
//			Row[i][j] = LocalMatrix[i][j];
//
//	// Compute X scale factor and normalize first row.
//	Scale.x = length(Row[0]);
//	Row[0] = detail::scale(Row[0], static_cast<T>(1));
//	Scale.y = length(Row[1]);
//	Row[1] = detail::scale(Row[1], static_cast<T>(1));
//	Scale.z = length(Row[2]);
//	Row[2] = detail::scale(Row[2], static_cast<T>(1));
//
//	// At this point, the matrix (in rows[]) is orthonormal.
//	// Check for a coordinate system flip.  If the determinant
//	// is -1, then negate the matrix and the scaling factors.
//#if 0
//	Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
//	if (dot(Row[0], Pdum3) < 0)
//	{
//		for (length_t i = 0; i < 3; i++)
//		{
//			scale[i] *= static_cast<T>(-1);
//			Row[i] *= static_cast<T>(-1);
//		}
//	}
//#endif
//
//	Rotation.y = asin(-Row[0][2]);
//	if (cos(Rotation.y) != 0) {
//		Rotation.x = atan2(Row[1][2], Row[2][2]);
//		Rotation.z = atan2(Row[0][1], Row[0][0]);
//	}
//	else {
//		Rotation.x = atan2(-Row[2][0], Row[1][1]);
//		Rotation.z = 0;
//	}
//
//
//	return true;
	return glm::mat4{ 1.0f };
}



