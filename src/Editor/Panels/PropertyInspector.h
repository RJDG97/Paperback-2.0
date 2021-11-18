#pragma once
#include "Editor/EditorInclude.h"

struct DetailsWindow : paperback::editor::instance
{
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

    void UpdateComponents( paperback::u32 EntityGlobalIndex );

//-----------------------------------
//        Component Combos
//-----------------------------------

    void ParentComponent();

    void ChildCombo();

    void DisplayAvailableChildren(paperback::component::entity& Entity, paperback::u32 i, 
                                  parent& NewParent, paperback::component::entity NewParentEntity);

    void MeshCombo();

    void AnimatorComponent();

    void SocketedComponent();

};