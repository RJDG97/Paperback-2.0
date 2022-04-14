/**********************************************************************************
*\file         DialogueSystem.h
*\brief        DialogueSystem.h
*
*\author       Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#ifndef _DIALOGUESYSTEM_H_
#define _DIALOGUESYSTEM_H_

#include "../../../src/Functionality/Dialogue/DialogueManager.h"

struct dialogue_system : paperback::system::pausable_instance
{
	// System Ptrs
	sound_system* sound_sys;

	// Manager Ptrs
	DialogueManager* dialogue_manager;

	// Queries
	tools::query Query_DialogueText;
	tools::query Query_PlayerCamera;
	tools::query Query_Camera;
	tools::query Query_Transition;

	bool m_CameraDone = false;

	enum Unit
	{
		JUMP,
		PUSH
	};

	Unit m_CurrentUnit = JUMP;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "path_system"
	};

	PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
		sound_sys = &GetSystem<sound_system>();

		Query_DialogueText.m_Must.AddFromComponents<dialogue_text, text, child>();
		Query_DialogueText.m_NoneOf.AddFromComponents<prefab>();

		Query_PlayerCamera.m_Must.AddFromComponents<player_controller, camera, name>();
		Query_PlayerCamera.m_NoneOf.AddFromComponents<prefab>();

		Query_Camera.m_Must.AddFromComponents<camera, name, path_follower>();
		Query_Camera.m_NoneOf.AddFromComponents<prefab, cinematic>();

		Query_Transition.m_Must.AddFromComponents<transition, transform, scale, mesh>();
		Query_Transition.m_NoneOf.AddFromComponents<prefab>();

		dialogue_manager = &DialogueManager::GetInstanced();
	}

	PPB_INLINE
	void OnStateQueued( void ) noexcept
	{
		ForEach(Search(Query_Transition), [&](transition& Transition, transform& Transform, scale& Scale, mesh& Mesh) noexcept
		{
			Mesh.m_Active = true;
			Transform.m_Position.x = Scale.m_Value.x; // set to center
			Transition.m_State = transition::EXITING_LEVEL;
		});
	}

	PPB_INLINE
	void OnStateChanged(void) noexcept
	{
		ForEach(Search(Query_Transition), [&](transition& Transition, transform& Transform, scale& Scale, mesh& Mesh) noexcept
		{
			Mesh.m_Active = false;
		});
	}

	PPB_FORCEINLINE
		void OnPause(const bool& Status) noexcept	//function activates whenever play / pause is clicked
	{
		if (Status) //is paused, play is clicked
		{
			ForEach(Search(Query_Transition), [&](transition& Transition, transform& Transform, scale& Scale, mesh& Mesh) noexcept
			{
				Mesh.m_Active = false;
			});
		}
	}

	PPB_INLINE
	void OnStateLoad( void ) noexcept
	{
		ForEach(Search(Query_Transition), [&](transition& Transition, transform& Transform, scale& Scale, mesh& Mesh) noexcept
		{
			Mesh.m_Active = true;
			Transform.m_Position = {0.0f, 0.0f, 3.0f}; // set to center
			Transition.m_State = transition::ENTERING_LEVEL;
		});

		ForEach(Search(Query_Camera), [&](camera& Camera, name& Name, path_follower& PathFollower) noexcept
		{
			Camera.m_Active = false;
		});
	}

	PPB_FORCEINLINE
	void Update(void) noexcept
	{
		ForEach(Search(Query_Transition), [&](transition& Transition, transform& Transform, scale& Scale) noexcept
		{
			switch (Transition.m_State)
			{
				case transition::ENTERING_LEVEL:
				{
					if (Transform.m_Position.x < -Scale.m_Value.x)
					{
						Transition.m_State = transition::INACTIVE;
					}

					float distance{ 1.3f * -Scale.m_Value.x - Transform.m_Position.x };

					if (distance < 0.0f)
					{
						Transform.m_Position.x += distance * 2.2f * DeltaTime();
					}

					break;
				}

				case transition::EXITING_LEVEL:
				{
					if (Transform.m_Position.x < 0.0f)
					{
						Transform.m_Position.x = 0.0f;
						Transition.m_State = transition::INACTIVE;
					}

					float distance{ 0.8f * -Scale.m_Value.x - Transform.m_Position.x };

					if (distance < 0.0f)
					{
						Transform.m_Position.x += distance * 2.2f * DeltaTime();
					}

					break;
				}
				
			}
		});

		ForEach(Search(Query_DialogueText), [&](dialogue_text& DialogueText, text& Text, child& Child) noexcept
		{
			auto dialogue_it {dialogue_manager->m_Dialogues.find(DialogueText.m_DialogueName)};

			if (dialogue_it != dialogue_manager->m_Dialogues.end())
			{
				Dialogue dialogue{ dialogue_it->second };

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

							if (!dialogue_manager->m_Dialogues[DialogueText.m_DialogueName].m_Lines.empty())
							{
								std::string audio_file{ dialogue.m_Lines[0].m_AudioFile };

								if (audio_file != "")
								{
									sound_sys->TriggerTaggedSound(audio_file);
									ActivateCamera(dialogue.m_Lines[0].m_CameraName);
									m_CameraDone = false;
								}
							}

							break;
						}

						if (!dialogue.m_Lines.empty())
						{
							if (dialogue.m_Lines[0].m_Speaker == Line::RED)
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
						float add_value = static_cast<float>(90.0f * (1.0f + cosf(DialogueText.m_ElapsedTime / 2.0f * 2 * static_cast<float>(M_PI) + static_cast<float>(M_PI))));

						CScale->m_Value += paperback::Vector3f{ add_value, add_value * ratio_xy, add_value * ratio_xz };

						break;
					}

					case dialogue_text::PLAYING:
					{
						DialogueText.m_ElapsedTime += DeltaTime();
						std::string current_line_text = dialogue.m_Lines[DialogueText.m_CurrentIndex].m_Content;

						auto& ParentInfo = GetEntityInfo(Child.m_ParentGlobalIndex);
						auto [CMesh] = ParentInfo.m_pArchetype->FindComponents<mesh>(ParentInfo.m_PoolDetails);

						if (!m_CameraDone)
						{
							CheckCameraDone(dialogue.m_Lines[DialogueText.m_CurrentIndex].m_CameraName);
						}

						if (dialogue.m_Lines[DialogueText.m_CurrentIndex].m_Speaker == Line::RED)
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

						else if (DialogueText.m_ElapsedTime >= DialogueText.m_HoldTime && m_CameraDone)
						{
							++DialogueText.m_CurrentIndex;

							if (DialogueText.m_CurrentIndex == dialogue.m_Lines.size())
							{
								DialogueText.m_State = dialogue_text::CLOSING;
								m_CameraDone = false;
							}

							else
							{
								std::string audio_file{ dialogue.m_Lines[DialogueText.m_CurrentIndex].m_AudioFile };

								if (audio_file != "")
								{
									sound_sys->TriggerTaggedSound(audio_file);
									ActivateCamera(dialogue.m_Lines[DialogueText.m_CurrentIndex].m_CameraName);
									m_CameraDone = false;
								}
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
						float add_value = static_cast<float>(70.0f * (1.0f + sinf(DialogueText.m_ElapsedTime / 2.0f * 2 * static_cast<float>(M_PI) + static_cast<float>(M_PI))));

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
			}
		});
	}

	void ActivateCamera(std::string cam_name)
	{
		if (cam_name != "")
		{
			ForEach(Search(Query_PlayerCamera), [&](player_controller& PC, camera& Camera, name& Name) noexcept
			{
				if (Camera.m_Active)
				{
					if (Name.m_Value == "Push Unit")
					{
						m_CurrentUnit = PUSH;
					}

					else
					{
						m_CurrentUnit = JUMP;
					}
				}

				Camera.m_Active = false;
				m_Coordinator.SetToggleEnabled(false);
			});

			ForEach(Search(Query_Camera), [&](camera& Camera, name& Name, path_follower& PathFollower) noexcept
			{
				if (Name.m_Value == cam_name)
				{
					Camera.m_Active = true;
					PathFollower.m_PauseTravel = false;
				}
			});
		}
	}

	void CheckCameraDone(std::string cam_name)
	{
		if (cam_name != "")
		{
			ForEach(Search(Query_Camera), [&](camera& Camera, name& Name, path_follower& PathFollower) noexcept
			{
				if (Name.m_Value == cam_name && PathFollower.m_FinishedTravelling)
				{
					Camera.m_Active = false;

					ForEach(Search(Query_PlayerCamera), [&](player_controller& PC, camera& Camera, name& Name) noexcept
					{
						if (m_CurrentUnit == PUSH && Name.m_Value == "Push Unit")
						{
							Camera.m_Active = true;
						}

						if (m_CurrentUnit == JUMP && Name.m_Value == "Jump Unit")
						{
							Camera.m_Active = true;
						}
					});
					
					m_CameraDone = true;
					m_Coordinator.SetToggleEnabled(true);
				}
			});
		}

		else
		{
			m_CameraDone = true;
		}
	}
};

#endif
