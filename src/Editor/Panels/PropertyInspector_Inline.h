#pragma once
#include "Editor/Panels/PropertyInspector.h"
#include "../../Functionality/RenderResource/RenderResourceManager.h"

void DetailsWindow::Panel()
{
	ImGui::Begin(DetailsWindow::typedef_v.m_pName, &m_bEnabled);

    DisplayProperties();

	ImGui::End();
}

void DetailsWindow::DisplayProperties()
{
    static ImGuiTextFilter Filter;
    Filter.Draw(ICON_FA_FILTER, 150.0f);

    if (!m_Imgui.m_Components.empty())
    {
        ImGui::PushItemWidth(120.0f);

        if (!m_Imgui.m_SelectedEntity.first->GetComponentBits().Has(paperback::component::info_v<reference_prefab>.m_UID)) //Cannot amend components if 
        {
            AddComponent();

            ImGui::SameLine();

            RemoveComponent();
        }
        else
            ImGui::NewLine();

        ImGui::PopItemWidth();

        ImGui::Separator();

        for (auto& ComponentInstance : m_Imgui.m_Components)
        {
            if (Filter.PassFilter(ComponentInstance.first.get_type().get_name().to_string().c_str()))
            {
                if (ImGui::CollapsingHeader(ComponentInstance.first.get_type().get_name().to_string().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                {
                    for (auto& property : ComponentInstance.first.get_type().get_properties())
                    {
                        rttr::variant PropertyValue = property.get_value(ComponentInstance.first);

                        if (!PropertyValue) continue;

                        auto PropertyType = property.get_type(); //etc vector 3, std::string etc
                        auto PropertyName = property.get_name().to_string();

                        if (PropertyType.get_wrapped_type().is_arithmetic() || PropertyType.is_arithmetic())
                            m_Imgui.DisplayBaseTypes(PropertyName, PropertyType, PropertyValue);

                        if ((PropertyType.get_wrapped_type() == rttr::type::get< std::string >() ||
                            PropertyType == rttr::type::get< std::string>()) &&
                            ComponentInstance.first.get_type().get_name().to_string() != "Mesh" &&
                            ComponentInstance.first.get_type().get_name().to_string() != "Socketed" &&
                            ComponentInstance.first.get_type().get_name().to_string() != "Animator")

                            m_Imgui.DisplayStringType(PropertyName, PropertyType, PropertyValue);

                        if (PropertyType.is_class())
                            m_Imgui.DisplayClassType(PropertyName, PropertyType, PropertyValue);

                        if (ComponentInstance.first.get_type().get_name().to_string() == "Parent")
                            ParentComponent();

                        auto ReferencePrefab = m_Imgui.m_SelectedEntity.first->FindComponent<reference_prefab>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });
                        auto Prefab = m_Imgui.m_SelectedEntity.first->FindComponent<prefab>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });

                        if (Prefab)
                        {
                            if (ImGui::IsItemEdited())
                            {
                            }
                        }

                        if (ReferencePrefab)
                        {
                            if (ImGui::IsItemEdited())
                            {
                                ReferencePrefab->AddModifiedComponentGuid(ComponentInstance.second.m_Value);
                            }

                            //if have reset button -> Call ResetModifiedComponentGuid
                        }
                    }

                    if (ComponentInstance.first.get_type().get_name().to_string() == "Mesh")
                        MeshCombo();
                    if (ComponentInstance.first.get_type().get_name().to_string() == "Animator")
                        AnimatorComponent();
                    if (ComponentInstance.first.get_type().get_name().to_string() == "Socketed")
                        SocketedComponent();
                }
            }
        }
    }
}

void DetailsWindow::AddComponent()
{
    std::array<const paperback::component::info*, 1 > ComponentAdd;

    if (m_Imgui.m_SelectedEntity.first)
    {
        if (ImGui::BeginCombo("##AddComponents", ICON_FA_PLUS " Add"))
        {
            for (auto& [CompKey, CompPInfo] : PPB.GetComponentInfoMap())
            {
                if (!m_Imgui.m_SelectedEntity.first->CheckComponentExistence(CompPInfo))
                {
                    if (CompPInfo->m_pName != "Child" && CompPInfo->m_pName != "ReferencePrefab" && CompPInfo->m_pName != "Entity") // dont include child & ReferencePrefab component
                    {
                        if (ImGui::Selectable(CompPInfo->m_pName))
                        {
                            auto& Entity = m_Imgui.m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second }); //first instance

                            ComponentAdd[0] = CompPInfo;

                            //Get global index of the current selected entity
                            auto GlobalIndex = Entity.m_GlobalIndex;

                            PPB.AddOrRemoveComponents(Entity, ComponentAdd, {});

                            EDITOR_INFO_PRINT("Added: " + std::string(CompPInfo->m_pName) + " Component");

                            if (!m_Imgui.m_Components.empty())
                            {
                                m_Imgui.m_Components.clear();
                                ImGui::EndCombo();
                                UpdateComponents(GlobalIndex);

                                return;
                            }
                            else
                            {
                                ImGui::EndCombo();
                                return;
                            }
                        }
                    }
                }
            }

            ImGui::EndCombo();
        }
    }
}

void DetailsWindow::RemoveComponent()
{
    std::array<const paperback::component::info*, 1 > ComponentRemove;
    bool a = false;
    if (m_Imgui.m_SelectedEntity.first)
    {
        if (ImGui::BeginCombo("##RemoveComponent", ICON_FA_MINUS " Remove"))
        {
            for (paperback::u32 i = 0; i < m_Imgui.m_SelectedEntity.first->GetComponentCount(); ++i)
            {
                if (m_Imgui.m_SelectedEntity.first->GetComponentInfos()[i]->m_pName != "Entity")
                {
                    if (ImGui::Selectable(m_Imgui.m_SelectedEntity.first->GetComponentInfos()[i]->m_pName))
                    {
                        auto& Entity = m_Imgui.m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second }); //first instance
                        ComponentRemove[0] = (m_Imgui.m_SelectedEntity.first->GetComponentInfos()[i]);

                        //Get global index of the current selected entity
                        auto GlobalIndex = Entity.m_GlobalIndex;
                        std::string Temp = m_Imgui.m_SelectedEntity.first->GetName();

                        PPB.AddOrRemoveComponents(Entity, {}, ComponentRemove);

                        EDITOR_INFO_PRINT("Removed: " + Temp + " Component");
                        
                        if (!m_Imgui.m_Components.empty())
                        {
                            m_Imgui.m_Components.clear();
                            ImGui::EndCombo();
                            UpdateComponents(GlobalIndex);
                            return;
                        }
                        else
                        {
                            ImGui::EndCombo();
                            return;
                        }
                    }
                }
            }
            ImGui::EndCombo();
        }
    }
}

void DetailsWindow::UpdateComponents( paperback::u32 EntityGlobalIndex )
{
    //Get Entity Info

    auto& EntityInfo = PPB.GetEntityInfo(EntityGlobalIndex);

    m_Imgui.m_SelectedEntity.first = EntityInfo.m_pArchetype;
    m_Imgui.m_SelectedEntity.second = EntityInfo.m_PoolDetails.m_PoolIndex;
    m_Imgui.m_Components = m_Imgui.m_SelectedEntity.first->GetEntityComponents(m_Imgui.m_SelectedEntity.second);

}

void DetailsWindow::ParentComponent()
{
    if (m_Imgui.m_SelectedEntity.first->FindComponent<parent>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second })))
    {
        auto& Parent = m_Imgui.m_SelectedEntity.first->GetComponent<parent>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));

        if (Parent.m_ChildrenGlobalIndexes.size() != 0)
        {
            for (auto& Child : Parent.m_ChildrenGlobalIndexes)
                ImGui::Text("Child GID: %d", Child);
        }
        else
            ImGui::Text("No Child is attached to this parent");

        ChildCombo();
    }
}

void DetailsWindow::MeshCombo()
{
    RenderResourceManager& Manager = RenderResourceManager::GetInstanced();

    //Get Mesh Component
    auto& EntityMesh = m_Imgui.m_SelectedEntity.first->GetComponent<mesh>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });

    if (ImGui::BeginCombo("##ModelMeshes", EntityMesh.m_Model.empty() ? "Choose Model" : EntityMesh.m_Model.c_str()))
    {
        if (!Manager.m_Models.empty())
        {
            for ( auto& [MeshName, Mesh] : Manager.m_Models )
            {
                if (MeshName != EntityMesh.m_Model) //Dont include the current texture if there is any
                {
                    if (ImGui::Selectable(MeshName.c_str()))
                    {
                        EntityMesh.m_Model = MeshName;
                    }
                }
            }
        }

        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("##ModelTexture", EntityMesh.m_Texture.empty() ? "Choose Texture" : EntityMesh.m_Texture.c_str()))
    {
        if (!Manager.m_Textures.empty())
        {
            for (auto& [TextureName, Texture] : Manager.m_Textures)
            {
                if (TextureName != EntityMesh.m_Model) //Dont include the current texture if there is any
                {
                    if (ImGui::Selectable(TextureName.c_str()))
                    {
                        EntityMesh.m_Texture = TextureName;
                    }
                }
            }
        }

        ImGui::EndCombo();
    }
}

void DetailsWindow::AnimatorComponent()
{
    RenderResourceManager& RRM = RenderResourceManager::GetInstanced();
    //get Mesh component -> m_Model
    auto EntityMesh = m_Imgui.m_SelectedEntity.first->FindComponent<mesh>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });
    auto EntityAnimator = m_Imgui.m_SelectedEntity.first->FindComponent<animator>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });

    if (EntityMesh && EntityAnimator)
    {
        auto& AnimationMap = RRM.m_Models[EntityMesh->m_Model].GetAnimations(); //Get the animations that is avaliable for the model

        if (ImGui::BeginCombo("##ModelAnimation", EntityAnimator->m_CurrentAnimationName.empty() ? "Choose an animation" : EntityAnimator->m_CurrentAnimationName.c_str()))
        {
            if (!AnimationMap.empty())
            {
                for (auto& [AnimString, Anim] : AnimationMap)
                {
                    if (ImGui::Selectable(AnimString.c_str()))
                    {
                        EntityAnimator->m_CurrentAnimationName = AnimString;
                    }
                }
            }
            ImGui::EndCombo();
        }
    }
}

void DetailsWindow::SocketedComponent()
{
    RenderResourceManager& RRM = RenderResourceManager::GetInstanced();

    auto EntityChild = m_Imgui.m_SelectedEntity.first->FindComponent<child>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });
    auto EntitySocketed = m_Imgui.m_SelectedEntity.first->FindComponent<socketed>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });
    if (EntitySocketed)
    {
        if (EntityChild)
        {
            //Get Entity Parent
            auto& ParentEntityInfo = PPB.GetEntityInfo(EntityChild->m_ParentGlobalIndex);

            //Get the Parent Mesh + Animation
            auto ParentMesh = ParentEntityInfo.m_pArchetype->FindComponent<mesh>(ParentEntityInfo.m_PoolDetails);
            auto ParentAnimator = ParentEntityInfo.m_pArchetype->FindComponent<animator>(ParentEntityInfo.m_PoolDetails);

            if (ParentMesh && ParentAnimator)
            {
                auto& AnimationMap = RRM.m_Models[ParentMesh->m_Model].GetAnimations(); //Get the animations that is avaliable for the model
                ImGui::Text("Parent Socket: ");

                if (!AnimationMap.empty())
                {
                    auto& BoneMap = AnimationMap[ParentAnimator->m_CurrentAnimationName].GetBoneIDMap(); //Get the bones based on the animation

                    if (!BoneMap.empty())
                    {
                        if (ImGui::BeginCombo("##ModelBones", EntitySocketed->m_ParentSocket.empty() ? "Select Parent Socket" : EntitySocketed->m_ParentSocket.c_str()))
                        {
                            for (auto& [BoneString, Bone] : BoneMap)
                            {
                                if (ImGui::Selectable(BoneString.c_str()))
                                {
                                    EntitySocketed->m_ParentSocket = BoneString;
                                }
                            }

                            ImGui::EndCombo();
                        }
                    }
                }
            }
        }
        else
        {
            ImGui::Text("Parent Socket: %s", EntitySocketed->m_ParentSocket.c_str());
        }
    }
}

void DetailsWindow::ChildCombo()
{
    if (ImGui::BeginCombo("##Potential Children", "Potential Children")) //TBD IDK WHAT TO NAME TIS
    {
        if (!PPB.GetArchetypeList().empty())
        {
            for (auto& Archetype : PPB.GetArchetypeList())
            {
                for (paperback::u32 i = 0; i < Archetype->GetCurrentEntityCount(); ++i)
                {
                    // Selected Entity -> New Parent
                    auto& NewParent = m_Imgui.m_SelectedEntity.first->GetComponent<parent>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));

                    // Check if the new parent is a prefab
                    auto ParentPrefab = m_Imgui.m_SelectedEntity.first->FindComponent<prefab>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));

                    // New Parent Entity Component
                    auto NewParentEntity = m_Imgui.m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));


                    if (!Archetype->GetComponentBits().Has(paperback::component::info_v<reference_prefab>.m_UID)) // Prefab instances cant be added as children
                    {
                        if (ParentPrefab)
                        {
                            //if new parent is a prefab -> Potential children have to be prefabs as well

                            if (Archetype->GetComponentBits().Has(paperback::component::info_v<prefab>.m_UID))
                            {
                                auto& Entity = Archetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails({ 0, i }));
                                DisplayAvailableChildren(Entity, i, NewParent, NewParentEntity, Archetype);
                            }
                        }
                        else
                        {
                            //normal entities
                            if (!Archetype->GetComponentBits().Has(paperback::component::info_v<prefab>.m_UID))
                            {
                                // Non Parent Entities Entity Component
                                auto& Entity = Archetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails({ 0, i }));
                                DisplayAvailableChildren(Entity, i, NewParent, NewParentEntity, Archetype);
                            }
                        }
                    }
                }
            }
        }
        ImGui::EndCombo();
    }
}

void DetailsWindow::DisplayAvailableChildren(paperback::component::entity& Entity, paperback::u32 i, parent& NewParent, paperback::component::entity NewParentEntity, paperback::archetype::instance* Archetype)
{
    paperback::u32 OldParentGID;
    std::array<const paperback::component::info*, 1 > ComponentAdd;

    if (NewParent.m_ChildrenGlobalIndexes.find(Entity.m_GlobalIndex) == NewParent.m_ChildrenGlobalIndexes.end()) //not already a child of the selected entity
    {
        std::string EntityName = Archetype->GetName() + " [" + std::to_string(i) + "]";

        if (EntityName != (m_Imgui.m_SelectedEntity.first->GetName() + " [" + std::to_string(m_Imgui.m_SelectedEntity.second) + "]"))
        {
            if (ImGui::Selectable(EntityName.c_str()))
            {
                if (Archetype->FindComponent<child>(paperback::vm::PoolDetails({ 0, i })) != nullptr) //Check for Child Component
                {
                    //Get the Child Component
                    auto& Child = Archetype->GetComponent<child>(paperback::vm::PoolDetails({ 0, i }));
                    OldParentGID = Child.m_ParentGlobalIndex; //Get Old Parent GID

                    //Check if already attached to a parent
                    if (OldParentGID != NewParentEntity.m_GlobalIndex)
                    {
                        auto& OldParent = PPB.GetEntityInfo(OldParentGID);

                        //Old Parent remove child
                        OldParent.m_pArchetype->GetComponent<parent>(OldParent.m_PoolDetails).RemoveChild(Entity.m_GlobalIndex);
                    }

                    if (OldParentGID == Entity.m_GlobalIndex) //check if parent entity matches the current selected child
                    {
                        EDITOR_WARN_PRINT("Unable to add entity as a child");
                        return;
                    }

                    //Link Child to Existing Parent
                    NewParent.AddChild(Entity.m_GlobalIndex);
                    //Update Child's Parent GID
                    Child.AddParent(NewParentEntity.m_GlobalIndex);
                }
                else
                {
                    //save new child GID
                    auto EntityIndex = Entity.m_GlobalIndex;
                    //Link Child to Existing Parent
                    NewParent.AddChild(EntityIndex);

                    //Add in child component
                    ComponentAdd[0] = &paperback::component::info_v<child>;
                    PPB.AddOrRemoveComponents(Entity, ComponentAdd, {});

                    //Add Parent GID to Child
                    auto& Info = PPB.GetEntityInfo(EntityIndex);
                    Info.m_pArchetype->GetComponent<child>(Info.m_PoolDetails).AddParent(NewParentEntity.m_GlobalIndex);
                }
            }
        }
    }

}
