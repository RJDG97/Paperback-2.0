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
            if (!m_Activated && (ID == m_JumpUnitID || ID == m_PushUnitID))
            {
                DialogueText dialogue_text = new DialogueText((UInt32)Player.GetDialogueTextID());
                dialogue_text.m_DialogueName = m_DialogueCollider.m_DialogueName;
                dialogue_text.m_ElapsedTime = 0.0f;
                dialogue_text.m_State = 0;
                dialogue_text.m_InitialScale = new Tools.MathLib.Vector3(0.0f, 0.0f, 0.0f);
                m_Activated = true;
            }
        }

        public void OnCollisionStay(UInt32 ID)
        {
        }

        public void OnCollisionExit(UInt32 ID)
        {
        }
    }
}
