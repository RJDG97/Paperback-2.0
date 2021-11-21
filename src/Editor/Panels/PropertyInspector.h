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

    void ParentComponent(prefab* Prefab, reference_prefab* ReferencePrefab, paperback::u64 CompGuid);

    void AnimatorComponent(paperback::entity::info& EntityInfo, prefab* Prefab, reference_prefab* ReferencePrefab, paperback::u64 CompGuid);

    void SocketedComponent(paperback::entity::info& EntityInfo, prefab* Prefab, reference_prefab* ReferencePrefab, paperback::u64 CompGuid);

    void PrefabComponent();

//-----------------------------------
//        Component Combos
//-----------------------------------
    void ChildCombo(prefab* Prefab, reference_prefab* ReferencePrefab, paperback::u64 CompGuid);

    void MeshCombo(paperback::entity::info& EntityInfo, prefab* Prefab, reference_prefab* ReferencePrefab, paperback::u64 CompGuid);

    void DisplayAvailableChildren(paperback::component::entity& Entity, paperback::u64 i,
        parent& NewParent, paperback::component::entity NewParentEntity,
        prefab* Prefab, reference_prefab* ReferencePrefab, paperback::u64 CompGuid);

};