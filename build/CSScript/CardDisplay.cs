﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Linq;

namespace CSScript
{
    public class CardDisplay : MonoBehaviour
    {
        UInt32 m_ID;
        UInt32 m_JumpID;
        UInt32 m_PushID;
        PlayerController m_JumpUnitPC;
        PlayerController m_PushUnitPC;

        float m_CooldownTimer;

        Mesh m_E;
        Mesh m_CardOne;
        Mesh m_CardTwo;
        Mesh m_CardThree;

        enum Ability
        {
            STOP_MOVING_PLATFORM,
            GROW,
            SHRINK
        };

        List<Ability> m_Abilities = new List<Ability>();

        public static CardDisplay getInst()
        {
            return new CardDisplay();
        }
        public void Start(UInt32 ID)
        {
            m_ID = ID;

            m_JumpID = (UInt32)Player.GetJumpUnitID();
            m_PushID = (UInt32)Player.GetPushUnitID();
            m_JumpUnitPC = new PlayerController(m_JumpID);
            m_PushUnitPC = new PlayerController(m_PushID);

            m_E = new Mesh(m_ID);
            Parent m_Parent = new Parent(m_ID);

            Int32 m_ChildID1 = m_Parent.GetChildIDofName("Card 1");
            Int32 m_ChildID2 = m_Parent.GetChildIDofName("Card 2");
            Int32 m_ChildID3 = m_Parent.GetChildIDofName("Card 3");

            if (m_ChildID1 != -1 && m_ChildID2 != -1 && m_ChildID3 != -1)
            {
                m_CardOne = new Mesh((UInt32)m_ChildID1);
                m_CardTwo = new Mesh((UInt32)m_ChildID2);
                m_CardThree = new Mesh((UInt32)m_ChildID3);
                m_CardOne.m_Active = false;
                m_CardTwo.m_Active = false;
                m_CardThree.m_Active = false;
            }

            m_E.m_Active = true;
            m_CooldownTimer = -1.0f;
        }

        public void PreUpdate(float dt)
        {
        }

        public void Update(float dt)
        {
            if (m_CooldownTimer > 0.0f)
            {

                m_CooldownTimer -= dt;
            }

            if (m_JumpUnitPC.m_FPSMode || m_PushUnitPC.m_FPSMode)
            {
                m_E.m_Active = false;

                if (m_Abilities.Count > 0)
                {
                    m_CardOne.m_Active = true;
                    m_CardTwo.m_Active = false;
                    m_CardThree.m_Active = false;
                }

                if (m_Abilities.Count > 1)
                {
                    m_CardTwo.m_Active = true;
                    m_CardThree.m_Active = false;
                }

                if (m_Abilities.Count > 2)
                {
                    m_CardThree.m_Active = true;
                }

                if ((Input.IsKeyPress(Input.PB_TAB) || Input.IsGamepadButtonPressDown(Input.PB_GAMEPAD_BUTTON_RIGHT_BUMPER)) && m_Abilities.Count > 1 && m_CooldownTimer < 0.0f)
                {
                    Ability first = m_Abilities[0];
                    m_Abilities.RemoveAt(0);
                    m_Abilities.Add(first);
                    m_CooldownTimer = 0.2f;
                }
            }

            else
            {
                m_E.m_Active = true;
                m_CardOne.m_Active = false;
                m_CardTwo.m_Active = false;
                m_CardThree.m_Active = false;
            }

            if (m_Abilities.Count < 3) //keep checking if not all abilities are picked up
            {
                if (m_JumpUnitPC.m_FreezeAvailable)
                {
                    if (!m_Abilities.Any(x => x == Ability.STOP_MOVING_PLATFORM))
                    {
                        m_Abilities.Add(Ability.STOP_MOVING_PLATFORM);
                    }
                }

                if (m_JumpUnitPC.m_GrowAvailable)
                {
                    if (!m_Abilities.Any(x => x == Ability.GROW))
                    {
                        m_Abilities.Add(Ability.GROW);
                    }
                }

                if (m_JumpUnitPC.m_ShrinkAvailable)
                {
                    if (!m_Abilities.Any(x => x == Ability.SHRINK))
                    {
                        m_Abilities.Add(Ability.SHRINK);
                    }
                }
            }

            if (m_Abilities.Count > 0)
            {
                UpdateCardMesh(m_CardOne, 0);
            }

            if (m_Abilities.Count > 1)
            {
                UpdateCardMesh(m_CardTwo, 1);
            }

            if (m_Abilities.Count > 2)
            {
                UpdateCardMesh(m_CardThree, 2);
            }
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
        }
        public void OnCollisionStay(UInt32 ID)
        {
        }
        public void OnCollisionExit(UInt32 ID)
        {
        }

        private void UpdateCardMesh(Mesh card, int i)
        {
            switch (m_Abilities[i])
            {
                case Ability.STOP_MOVING_PLATFORM:
                {
                    card.m_Texture = "FreezeCard";
                    break;
                }

                case Ability.GROW:
                {
                    card.m_Texture = "GrowCard";
                    break;
                }

                case Ability.SHRINK:
                {
                    card.m_Texture = "ShrinkCard";
                    break;
                }
            }
        }
    }
}
