using System;
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
        Offset m_CardOneOffset;
        Tools.MathLib.Vector3 m_InitialCardOnePos;
        Tools.MathLib.Vector3 m_InitialCardOneScale;
        int m_CurrentOrderOne;

        Mesh m_CardTwo;
        Offset m_CardTwoOffset;
        Tools.MathLib.Vector3 m_InitialCardTwoPos;
        Tools.MathLib.Vector3 m_InitialCardTwoScale;
        int m_CurrentOrderTwo;

        Mesh m_CardThree;
        Offset m_CardThreeOffset;
        Tools.MathLib.Vector3 m_InitialCardThreePos;
        Tools.MathLib.Vector3 m_InitialCardThreeScale;
        int m_CurrentOrderThree;

        bool m_JustEnteredFPS = false;
        bool m_JustLeftFPS = true;

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

                m_CardOneOffset = new Offset((UInt32)m_ChildID1);
                m_CardTwoOffset = new Offset((UInt32)m_ChildID2);
                m_CardThreeOffset = new Offset((UInt32)m_ChildID3);

                m_InitialCardOnePos = m_CardOneOffset.m_PosOffset;
                m_InitialCardTwoPos = m_CardTwoOffset.m_PosOffset;
                m_InitialCardThreePos = m_CardThreeOffset.m_PosOffset;

                m_InitialCardOneScale = m_CardOneOffset.m_ScaleOffset;
                m_InitialCardTwoScale = m_CardTwoOffset.m_ScaleOffset;
                m_InitialCardThreeScale = m_CardThreeOffset.m_ScaleOffset;

                m_CardOne.m_Active = false;
                m_CardTwo.m_Active = false;
                m_CardThree.m_Active = false;

                m_CurrentOrderOne = 1;
                m_CurrentOrderTwo = 2;
                m_CurrentOrderThree = 3;
            }

            m_E.m_Active = true;
            m_CooldownTimer = -1.0f;
        }

        public void PreUpdate(float dt)
        {
            //if (m_JumpUnitPC.m_FPSMode || m_PushUnitPC.m_FPSMode)
            //{
            //    m_JustLeftFPS = false;

            //    if (!m_JustEnteredFPS)
            //    {
            //        m_JustEnteredFPS = true;

            //        switch (m_JumpUnitPC.m_Abilities.Length)
            //        {
            //            case 0: m_CardOne.m_Active = false; m_CardTwo.m_Active = false; m_CardThree.m_Active = false; break;
            //            case 1: m_CardOne.m_Active = true; m_CardTwo.m_Active = false; m_CardThree.m_Active = false; break;
            //            case 2: m_CardOne.m_Active = true; m_CardTwo.m_Active = true; m_CardThree.m_Active = false; break;
            //            case 3: m_CardOne.m_Active = true; m_CardTwo.m_Active = true; m_CardThree.m_Active = true; break;
            //        }

            //        int i = 1;

            //        foreach (Int32 ability in m_JumpUnitPC.m_Abilities)
            //        {
            //            String texture = "";

            //            switch ((PlayerController.Ability)ability)
            //            {
            //                case PlayerController.Ability.FREEZE: texture = "FreezeCard"; break;
            //                case PlayerController.Ability.GROW: texture = "GrowCard"; break;
            //                case PlayerController.Ability.SHRINK: texture = "ShrinkCard"; break;
            //            }

            //            switch (i)
            //            {
            //                case 1: m_CardOne.m_Texture = texture; break;
            //                case 2: m_CardTwo.m_Texture = texture + "_Unselected"; break;
            //                case 3: m_CardThree.m_Texture = texture + "_Unselected"; break;
            //            }

            //            Debug.Log(texture);
            //            ++i;
            //        }
            //    }

            //    m_E.m_Active = false;


            //    if (m_CooldownTimer > 0.0f)
            //    {
            //        m_CooldownTimer -= dt;
            //    }

            //    if ((Input.IsKeyPress(Input.PB_TAB) || Input.IsGamepadButtonPressDown(Input.PB_GAMEPAD_BUTTON_X)) && m_JumpUnitPC.m_Abilities.Length > 1 && m_CooldownTimer < 0.0f)
            //    {
            //        m_CooldownTimer = 0.15f;

            //        switch (m_JumpUnitPC.m_Abilities.Length)
            //        {
            //            case 2:
            //            {
            //                int temp = m_CurrentOrderOne;
            //                m_CurrentOrderOne = m_CurrentOrderTwo;
            //                m_CurrentOrderTwo = temp;
            //                ChangeCardTexture(m_CardOne, m_CurrentOrderOne);
            //                ChangeCardTexture(m_CardTwo, m_CurrentOrderTwo);
            //                break;
            //            }

            //            case 3:
            //            {
            //                int temp = m_CurrentOrderTwo;
            //                m_CurrentOrderTwo = m_CurrentOrderOne;
            //                m_CurrentOrderOne = m_CurrentOrderThree;
            //                m_CurrentOrderThree = temp;

            //                ChangeCardTexture(m_CardOne, m_CurrentOrderOne);
            //                ChangeCardTexture(m_CardTwo, m_CurrentOrderTwo);
            //                ChangeCardTexture(m_CardThree, m_CurrentOrderThree);
            //                break;
            //            }
            //        }
            //    }
            //}

            //else
            //{
            //    m_JustEnteredFPS = false;

            //    if (!m_JustLeftFPS)
            //    {
            //        m_JustLeftFPS = true;
            //    }

            //    if (m_JumpUnitPC.m_Abilities.Length == 0)
            //    {
            //        m_E.m_Active = false;
            //    }

            //    else
            //    {
            //        m_E.m_Active = true;
            //    }

            //    m_CardOne.m_Active = false;
            //    m_CardTwo.m_Active = false;
            //    m_CardThree.m_Active = false;
            //}

            //if (m_JumpUnitPC.m_Abilities.Length > 0)
            //{
            //    RotateCard(m_CardOneOffset, m_CurrentOrderOne, dt);
            //}

            //if (m_JumpUnitPC.m_Abilities.Length > 1)
            //{
            //    RotateCard(m_CardTwoOffset, m_CurrentOrderTwo, dt);
            //}

            //if (m_JumpUnitPC.m_Abilities.Length > 2)
            //{
            //    RotateCard(m_CardThreeOffset, m_CurrentOrderThree, dt);
            //}
        }
        public void Update(float dt)
        {
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
        public void Reset()
        {
        }

        private void ChangeCardTexture(Mesh mesh, int destination)
        {
            switch (destination)
            {
                case 1:
                {
                    switch((PlayerController.Ability)m_JumpUnitPC.m_Abilities[0])
                    {
                        case PlayerController.Ability.FREEZE: mesh.m_Texture = "FreezeCard"; break;
                        case PlayerController.Ability.GROW: mesh.m_Texture = "GrowCard"; break;
                        case PlayerController.Ability.SHRINK: mesh.m_Texture = "ShrinkCard"; break;
                    }

                    break;
                }

                case 2:
                {
                    switch((PlayerController.Ability)m_JumpUnitPC.m_Abilities[1])
                    {
                        case PlayerController.Ability.FREEZE: mesh.m_Texture = "FreezeCard_Unselected"; break;
                        case PlayerController.Ability.GROW: mesh.m_Texture = "GrowCard_Unselected"; break;
                        case PlayerController.Ability.SHRINK: mesh.m_Texture = "ShrinkCard_Unselected"; break;
                    }
                    break;
                }

                case 3:
                {
                    switch((PlayerController.Ability)m_JumpUnitPC.m_Abilities[2])
                    {
                        case PlayerController.Ability.FREEZE: mesh.m_Texture = "FreezeCard_Unselected"; break;
                        case PlayerController.Ability.GROW: mesh.m_Texture = "GrowCard_Unselected"; break;
                        case PlayerController.Ability.SHRINK: mesh.m_Texture = "ShrinkCard_Unselected"; break;
                    }
                    break;
                }
            }
        }

        private void RotateCard(Offset offset, int destination, float dt)
        {
            Tools.MathLib.Vector3 dest_pos = new Tools.MathLib.Vector3();
            Tools.MathLib.Vector3 dest_scale = new Tools.MathLib.Vector3();

            switch (destination)
            {
                case 1:
                {
                    dest_pos = m_InitialCardOnePos;
                    dest_scale = m_InitialCardOneScale;
                    break;
                }

                case 2:
                {
                    dest_pos = m_InitialCardTwoPos;
                    dest_scale = m_InitialCardTwoScale;
                    break;
                }

                case 3:
                {
                    dest_pos = m_InitialCardThreePos;
                    dest_scale = m_InitialCardThreeScale;
                    break;
                }
            }

            offset.m_PosOffset = offset.m_PosOffset + 15.0f * dt * (dest_pos - offset.m_PosOffset);
            offset.m_ScaleOffset = offset.m_ScaleOffset + 15.0f * dt * (dest_scale - offset.m_ScaleOffset);
        }
    }
}
