#pragma once
#include "Editor/Panels/PropertyInspector.h"
#include "../../Functionality/RenderResource/RenderResourceManager.h"

void DetailsWindow::Panel()
{
    DisplayProperties();
}

void DetailsWindow::DisplayProperties()
{
    ImGui::Begin(DetailsWindow::typedef_v.m_pName, &m_bEnabled);

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
    }
    ImGui::End();


    ImGui::Begin("##EntitiesProperties", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    if (!m_Imgui.m_Components.empty())
    {
        for (auto& ComponentInstance : m_Imgui.m_Components)
        {
            if (Filter.PassFilter(ComponentInstance.first.get_type().get_name().to_string().c_str()))
            {
                if (ImGui::CollapsingHeader(ComponentInstance.first.get_type().get_name().to_string().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                {
                    auto ReferencePrefab = m_Imgui.m_SelectedEntity.first->FindComponent<reference_prefab>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });
                    auto Prefab = m_Imgui.m_SelectedEntity.first->FindComponent<prefab>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });
                    auto& Entity = m_Imgui.m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });
                    auto& EntityInfo = PPB.GetEntityInfo(Entity.m_GlobalIndex);

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
                            ParentComponent(Prefab, ReferencePrefab, ComponentInstance.second.m_Value);

                        if (ComponentInstance.first.get_type().get_name().to_string() == "Prefab")
                            PrefabComponent();


                        if (Prefab)
                        {
                            if (ImGui::IsItemEdited())
                            {
                            }
                        }

                        if (ReferencePrefab)
                        {
                            if (ImGui::IsItemEdited())
                                ReferencePrefab->AddModifiedComponentGuid(ComponentInstance.second.m_Value);

                            //if have reset button -> Call ResetModifiedComponentGuid
                        }
                    }

                    if (ComponentInstance.first.get_type().get_name().to_string() == "Mesh")
                        MeshCombo(EntityInfo, Prefab, ReferencePrefab, ComponentInstance.second.m_Value);
                    if (ComponentInstance.first.get_type().get_name().to_string() == "Animator")
                        AnimatorComponent(EntityInfo, Prefab, ReferencePrefab, ComponentInstance.second.m_Value);
                    if (ComponentInstance.first.get_type().get_name().to_string() == "Socketed")
                        SocketedComponent(EntityInfo, Prefab, ReferencePrefab, ComponentInstance.second.m_Value);
                }
            }
        }
    }

    ImGui::End();

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
                                m_Imgui.UpdateComponents(GlobalIndex);

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
                            m_Imgui.UpdateComponents(GlobalIndex);
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

void DetailsWindow::ParentComponent(prefab* Prefab, reference_prefab* ReferencePrefab, paperback::u64 CompGuid)
{
    auto Parent = m_Imgui.m_SelectedEntity.first->FindComponent<parent>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));

    if (Parent)
    {
        if (Parent->m_ChildrenGlobalIndexes.size() != 0)
        {
            for (auto& Child : Parent->m_ChildrenGlobalIndexes)
                ImGui::Text("Child GID: %lu", Child);
        }
        else
            ImGui::Text("No Child is attached to this parent");

        ChildCombo(Prefab, ReferencePrefab, CompGuid);
    }
}

void DetailsWindow::PrefabComponent()
{
    auto Prefab = m_Imgui.m_SelectedEntity.first->FindComponent<prefab>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));

    if (Prefab)
    {
        if (Prefab->m_ReferencePrefabGIDs.size() != 0)
        {
            for (auto& Instance : Prefab->m_ReferencePrefabGIDs)
                ImGui::Text("Prefab Instance GID: %lu", Instance);
        }
    }
}

void DetailsWindow::MeshCombo(paperback::entity::info& EntityInfo, prefab* Prefab, reference_prefab* ReferencePrefab, paperback::u64 CompGuid)
{
    RenderResourceManager& Manager = RenderResourceManager::GetInstanced();

    //Get Mesh Component
    auto& EntityMesh = m_Imgui.m_SelectedEntity.first->GetComponent<mesh>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });

    std::string PrevMesh = EntityMesh.m_Model.c_str(), PrevTexture = EntityMesh.m_Texture.c_str();

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

                        if (!PrefabRefComboCheck(PrevMesh, EntityMesh.m_Model))
                        {
                            if (Prefab)
                            {
                                EntityInfo.m_pArchetype->UpdatePrefabInstanceComponent(EntityInfo.m_PoolDetails, EntityMesh);
                            }

                            if (ReferencePrefab)
                                ReferencePrefab->AddModifiedComponentGuid(CompGuid);
                        }
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

                        if (!PrefabRefComboCheck(PrevTexture, EntityMesh.m_Texture))
                        {
                            if (Prefab)
                            {
                                EntityInfo.m_pArchetype->UpdatePrefabInstanceComponent(EntityInfo.m_PoolDetails, EntityMesh);
                            }

                            if (ReferencePrefab)
                                ReferencePrefab->AddModifiedComponentGuid(CompGuid);
                        }
                    }
                }
            }
        }

        ImGui::EndCombo();
    }
}

void DetailsWindow::AnimatorComponent(paperback::entity::info& EntityInfo, prefab* Prefab, reference_prefab* ReferencePrefab, paperback::u64 CompGuid)
{
    RenderResourceManager& RRM = RenderResourceManager::GetInstanced();
    //get Mesh component -> m_Model
    auto EntityMesh = m_Imgui.m_SelectedEntity.first->FindComponent<mesh>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });
    auto EntityAnimator = m_Imgui.m_SelectedEntity.first->FindComponent<animator>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second });

    if (EntityMesh && EntityAnimator)
    {
        auto& AnimationMap = RRM.m_Models[EntityMesh->m_Model].GetAnimations(); //Get the animations that is avaliable for the model
        std::string PrevAnim = EntityAnimator->m_CurrentAnimationName.c_str();

        if (ImGui::BeginCombo("##ModelAnimation", EntityAnimator->m_CurrentAnimationName.empty() ? "Choose an animation" : EntityAnimator->m_CurrentAnimationName.c_str()))
        {
            if (!AnimationMap.empty())
            {
                for (auto& [AnimString, Anim] : AnimationMap)
                {
                    if (ImGui::Selectable(AnimString.c_str()))
                    {
                        EntityAnimator->m_CurrentAnimationName = AnimString;

                        if (!PrefabRefComboCheck(PrevAnim, EntityAnimator->m_CurrentAnimationName))
                        {
                            if (Prefab)
                            {
                                EntityInfo.m_pArchetype->UpdatePrefabInstanceComponent(EntityInfo.m_PoolDetails, *EntityAnimator);
                            }

                            if (ReferencePrefab)
                                ReferencePrefab->AddModifiedComponentGuid(CompGuid);
                        }
                    }
                }
            }
            ImGui::EndCombo();
        }
    }
}

void DetailsWindow::SocketedComponent(paperback::entity::info& EntityInfo, prefab* Prefab, reference_prefab* ReferencePrefab, paperback::u64 CompGuid)
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
                std::string PrevSocket = EntitySocketed->m_ParentSocket;

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

                                    if (!PrefabRefComboCheck(PrevSocket, EntitySocketed->m_ParentSocket))
                                    {
                                        if (Prefab)
                                        {
                                            EntityInfo.m_pArchetype->UpdatePrefabInstanceComponent(EntityInfo.m_PoolDetails, *EntitySocketed);
                                        }

                                        if (ReferencePrefab)
                                            ReferencePrefab->AddModifiedComponentGuid(CompGuid);
                                    }
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

void DetailsWindow::ChildCombo(prefab* Prefab, reference_prefab* ReferencePrefab, paperback::u64 CompGuid)
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
                                DisplayAvailableChildren(Entity, i, NewParent, NewParentEntity, Prefab, ReferencePrefab, CompGuid);
                            }
                        }
                        else
                        {
                            //normal entities
                            if (!Archetype->GetComponentBits().Has(paperback::component::info_v<prefab>.m_UID))
                            {
                                // Non Parent Entities Entity Component
                                auto& Entity = Archetype->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails({ 0, i }));
                                DisplayAvailableChildren(Entity, i, NewParent, NewParentEntity, Prefab, ReferencePrefab, CompGuid);
                            }
                        }
                    }
                }
            }
        }
        ImGui::EndCombo();
    }
}

void DetailsWindow::DisplayAvailableChildren(paperback::component::entity& Entity, paperback::u64 i, parent& NewParent, paperback::component::entity NewParentEntity,
                                             prefab* Prefab, reference_prefab* ReferencePrefab, paperback::u64 CompGuid)
{
    std::string EntityName{};
    
    size_t NumChild = NewParent.m_ChildrenGlobalIndexes.size() ? NewParent.m_ChildrenGlobalIndexes.size() : 0;

    if (NewParent.m_ChildrenGlobalIndexes.find(Entity.m_GlobalIndex) == NewParent.m_ChildrenGlobalIndexes.end()) //not already a child of the selected entity
    {
        if (Entity.m_GlobalIndex != m_Imgui.m_SelectedEntity.first->GetComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, m_Imgui.m_SelectedEntity.second }).m_GlobalIndex) //Not the entity itself
        {
            auto Name = PPB.GetEntityInfo(Entity.m_GlobalIndex).m_pArchetype->FindComponent<name>(PPB.GetEntityInfo(Entity.m_GlobalIndex).m_PoolDetails);

            if (Name)
                EntityName = Name->m_Value;
            else
                EntityName = "UnNamed Entity";

            if (ImGui::Selectable(EntityName.c_str()))
            {
                m_Imgui.LinkParentChild(Entity, NewParent, NewParentEntity);

                //after linking check number of children
                if (NumChild != NewParent.m_ChildrenGlobalIndexes.size()) //More children are added in
                {
                    if (Prefab)
                    {
                        auto& ParentEntityInfo = PPB.GetEntityInfo(NewParentEntity.m_GlobalIndex);
                        ParentEntityInfo.m_pArchetype->UpdatePrefabInstanceComponent(ParentEntityInfo.m_PoolDetails, NewParent);
                    }

                    if (ReferencePrefab)
                        ReferencePrefab->AddModifiedComponentGuid(CompGuid);
                }
            }
        }
    }
}

bool DetailsWindow::PrefabRefComboCheck(std::string& PreviousEntry, std::string& NewEntry)
{
    return (PreviousEntry == NewEntry);
}
