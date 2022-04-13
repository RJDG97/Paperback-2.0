/**********************************************************************************
*\file         DialogueTrigger.cs
*\brief        DialogueTrigger.cs Script
*
*\author	     Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			         or disclosure of this file or its contents without the prior
			         written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class DialogueTrigger : MonoBehaviour
    {
        UInt32 m_ID;
        DialogueCollider m_DialogueCollider;

        Int32 m_JumpUnitID;
        Int32 m_PushUnitID;

        bool m_Activated = false;

        public static DialogueTrigger getInst()
        {
            return new DialogueTrigger();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_DialogueCollider = new DialogueCollider(m_ID);
            m_JumpUnitID = Player.GetJumpUnitID();
            m_PushUnitID = Player.GetPushUnitID();
        }

        public void PreUpdate(float dt)
        {
        }

        public void Update(float dt)
        {
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            Debug.Log("OnCollisonEnter DialogueTrigger: " + m_ID.ToString());
            if (!m_Activated && (ID == m_JumpUnitID || ID == m_PushUnitID))
            {
                DialogueText dialogue_text = new DialogueText((UInt32)Player.GetDialogueTextID());
                dialogue_text.m_DialogueName = m_DialogueCollider.m_DialogueName;
                dialogue_text.m_ElapsedTime = 0.0f;
                dialogue_text.m_State = 1;
                dialogue_text.m_Index = 0;
                m_Activated = true;
            }
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
    }
}
