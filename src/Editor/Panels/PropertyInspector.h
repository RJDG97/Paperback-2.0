#pragma once
#include "Editor/EditorInclude.h"

struct DetailsWindow : paperback::editor::instance
{
    bool ComboUpdated = false;

    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Property Inspector"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = true;
    }

    void Update(void) noexcept
    {
        Panel();
    }

    void Panel();

    void DisplayProperties();

    void AddComponent();

    void RemoveComponent();

    bool PrefabRefComboCheck(std::string& PreviousEntry, std::string& NewEntry);

//-----------------------------------
//           Components
//-----------------------------------

    void ParentComponent(reference_prefab* ReferencePrefab, paperback::u32 CompGuid);

    void AnimatorComponent(reference_prefab* ReferencePrefab, paperback::u32 CompGuid);

    void SocketedComponent(reference_prefab* ReferencePrefab, paperback::u32 CompGuid);

    void PrefabComponent();

//-----------------------------------
//        Component Combos
//-----------------------------------
    void ChildCombo(reference_prefab* ReferencePrefab, paperback::u32 CompGuid);

    void MeshCombo(reference_prefab* ReferencePrefab, paperback::u32 CompGuid);

    void DisplayAvailableChildren(paperback::component::entity& Entity, paperback::u32 i,
        parent& NewParent, paperback::component::entity NewParentEntity,
        reference_prefab* ReferencePrefab, paperback::u32 CompGuid);

};