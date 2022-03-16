#pragma once
#ifndef _DIALOGUESYSTEM_H_
#define _DIALOGUESYSTEM_H_

#include "../../../src/Functionality/Dialogue/DialogueManager.h"

struct dialogue_system : paperback::system::pausable_instance
{
	// System Ptrs
	debug_system* debug_sys;
	sound_system* sound_sys;

	// Manager Ptrs
	DialogueManager* dialogue_manager;

	// Queries
	tools::query Query_DialogueText;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "path_system"
	};

	PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
		debug_sys = &GetSystem<debug_system>();
		sound_sys = &GetSystem<sound_system>();

		Query_DialogueText.m_Must.AddFromComponents<dialogue_text, text, child>();
		Query_DialogueText.m_NoneOf.AddFromComponents<prefab>();

		dialogue_manager = &DialogueManager::GetInstanced();
	}

	PPB_INLINE
	void OnStateChange( void ) noexcept
	{

	}

	PPB_INLINE
	void OnStateLoad( void ) noexcept
	{
	}

	PPB_FORCEINLINE
	void Update(void) noexcept
	{
		ForEach(Search(Query_DialogueText), [&](dialogue_text& DialogueText, text& Text, child& Child) noexcept
		{
			switch (DialogueText.m_State)
			{
				case dialogue_text::INACTIVE:
				{
					auto& ParentInfo = GetEntityInfo(Child.m_ParentGlobalIndex);
					auto [CScale] = ParentInfo.m_pArchetype->FindComponents<scale>(ParentInfo.m_PoolDetails);
					CScale->m_Value = {};

					break;
				}

				case dialogue_text::OPENING:
				{
					auto& ParentInfo = GetEntityInfo(Child.m_ParentGlobalIndex);
					auto [CScale, CMesh] = ParentInfo.m_pArchetype->FindComponents<scale, mesh>(ParentInfo.m_PoolDetails);

					if (CScale->m_Value.x > DialogueText.m_InitialScale.x)
					{
						CScale->m_Value = DialogueText.m_InitialScale;
						DialogueText.m_State = dialogue_text::PLAYING;
						DialogueText.m_ElapsedTime = 0.0f;
						sound_sys->TriggerTaggedSound("SFX_RedWalk");
						break;
					}

					if (!dialogue_manager->m_Dialogues[DialogueText.m_DialogueName].m_Lines.empty())
					{
						if (dialogue_manager->m_Dialogues[DialogueText.m_DialogueName].m_Lines[0].m_Speaker == Line::RED)
						{
							CMesh->m_Texture = "StrongToy_DialogueBox";
						}

						else
						{
							CMesh->m_Texture = "JumpToy_DialogueBox";
						}
					}

					DialogueText.m_ElapsedTime += DeltaTime();

					float ratio_xy = DialogueText.m_InitialScale.y / DialogueText.m_InitialScale.x;
					float ratio_xz = DialogueText.m_InitialScale.z / DialogueText.m_InitialScale.x;
					float add_value = 90.0f * (1.0f + cosf(DialogueText.m_ElapsedTime / 2.0f * 2 * M_PI + M_PI));
					
					CScale->m_Value += paperback::Vector3f{add_value, add_value * ratio_xy, add_value * ratio_xz};

					break;
				}

				case dialogue_text::PLAYING:
				{
					DialogueText.m_ElapsedTime += DeltaTime();
					std::string current_line_text = dialogue_manager->m_Dialogues[DialogueText.m_DialogueName].m_Lines[DialogueText.m_CurrentIndex].m_Content;

					auto& ParentInfo = GetEntityInfo(Child.m_ParentGlobalIndex);
					auto [CMesh] = ParentInfo.m_pArchetype->FindComponents<mesh>(ParentInfo.m_PoolDetails);

					if (dialogue_manager->m_Dialogues[DialogueText.m_DialogueName].m_Lines[DialogueText.m_CurrentIndex].m_Speaker == Line::RED)
					{
						CMesh->m_Texture = "StrongToy_DialogueBox";
					}

					else
					{
						CMesh->m_Texture = "JumpToy_DialogueBox";
					}

					if (!DialogueText.m_OnHold && DialogueText.m_ElapsedTime * DialogueText.m_TextSpeed < current_line_text.size() - 1)
						Text.m_Text = current_line_text.substr(0, static_cast<size_t>(DialogueText.m_ElapsedTime * DialogueText.m_TextSpeed));

					else if (!DialogueText.m_OnHold)
					{
						Text.m_Text = current_line_text;
						DialogueText.m_ElapsedTime = 0.0f;
						DialogueText.m_OnHold = true;
					}

					else if (DialogueText.m_ElapsedTime < DialogueText.m_HoldTime)
					{
						//just hold
					}

					else // move to next line
					{
						++DialogueText.m_CurrentIndex;

						if (DialogueText.m_CurrentIndex == dialogue_manager->m_Dialogues[DialogueText.m_DialogueName].m_Lines.size())
						{
							DialogueText.m_State = dialogue_text::CLOSING;
						}

						else
						{
							sound_sys->TriggerTaggedSound("SFX_RedWalk");
						}

						DialogueText.m_ElapsedTime = 0.0f;
						DialogueText.m_OnHold = false;
					}

					break;
				}


				case dialogue_text::CLOSING:
				{
					auto& ParentInfo = GetEntityInfo(Child.m_ParentGlobalIndex);
					auto [CScale] = ParentInfo.m_pArchetype->FindComponents<scale>(ParentInfo.m_PoolDetails);

					DialogueText.m_ElapsedTime += DeltaTime();

					float ratio_xy = DialogueText.m_InitialScale.x / DialogueText.m_InitialScale.y;
					float ratio_xz = DialogueText.m_InitialScale.x / DialogueText.m_InitialScale.z;
					float add_value = 70.0f * (1.0f + sinf(DialogueText.m_ElapsedTime / 2.0f * 2 * M_PI + M_PI));

					CScale->m_Value -= paperback::Vector3f{ add_value, add_value * ratio_xy, add_value * ratio_xz };

					if (CScale->m_Value.x < 0.0f)
					{
						CScale->m_Value = {};
						DialogueText.m_State = dialogue_text::INACTIVE;
						DialogueText.m_ElapsedTime = 0.0f;
						Text.m_Text = "";
					}

					break;
				}
			}
		});
	}
};

#endif
