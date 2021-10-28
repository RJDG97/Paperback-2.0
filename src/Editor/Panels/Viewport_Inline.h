#pragma once
#include "Viewport.h"

void EditorViewport::Panel()
{
	ImGui::Begin(EditorViewport::typedef_v.m_pName);

	ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();

	paperback::u64 TextureID = Renderer::GetInstanced().GetFinalImage();

	ImGui::Image(reinterpret_cast<void*>(TextureID), ViewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();
}