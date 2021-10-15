#pragma once
#include "Editor/EditorInclude.h"

struct ArchetypeInspector : paperback::editor::instance
{
    paperback::archetype::instance* m_pArchetype; //refers back to the archetype that the entity is referencing to
    std::vector <const char*> m_ComponentNames = {};

    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Archetypes/Prefabs"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = true;
        m_pArchetype = nullptr;
    }

    void Update(void) noexcept
    {
        int Index = 0;
        std::string ArchetypeName;

        ImGui::Begin(ArchetypeInspector::typedef_v.m_pName);

        static ImGuiTextFilter Filter;
        Filter.Draw(ICON_FA_FILTER, 150.0f);


        ImGui::BeginChild("Archetypes: ", { ImGui::GetContentRegionAvailWidth() / 2, ImGui::GetContentRegionAvail().y }, true);

        for (auto& Archetype : PPB.GetArchetypeList())
        {
            ++Index;

            char Buffer[256]{};
            ArchetypeName = Archetype->GetName();
            std::copy(ArchetypeName.begin(), ArchetypeName.end(), Buffer);

            if (Filter.PassFilter(ArchetypeName.c_str()))
            {
                ImGui::PushItemWidth( 150.0f );

                if (ImGui::InputText(("##ArchetypeName" + std::to_string(Index)).c_str(), Buffer, IM_ARRAYSIZE(Buffer), ImGuiInputTextFlags_EnterReturnsTrue))
                    Archetype->SetName(std::string(Buffer));

                ImGui::PopItemWidth();

                if ( ImGui::IsItemClicked() )
                {
                    m_pArchetype = Archetype;
                    m_ComponentNames.clear();

                    for ( paperback::u32 i = 0; i < Archetype->GetComponentNumber(); ++i )
                    {
                        m_ComponentNames.push_back( Archetype->GetComponentInfos()[i]->m_pName );
                    }
                }
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Details: ");

        if ( !m_ComponentNames.empty() && m_pArchetype )
        {
            ImGui::Text( "Number of Entities: %d", m_pArchetype->GetEntityCount() );

            if ( ImGui::Button("Spawn New Entity") )
                m_pArchetype->CreateEntity();

            ImGui::Separator();

            ImGui::Text( "Archetype Components: " );

            for (auto& Names : m_ComponentNames)
                ImGui::Text(Names);
        }
        ImGui::EndChild();

        ImGui::End();
    }

};