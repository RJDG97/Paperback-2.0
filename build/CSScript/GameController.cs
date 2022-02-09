using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class GameController : MonoBehaviour
    {
        UInt32 m_ID;
        UInt32 m_JumpID;
        UInt32 m_PushID;
        Camera m_JumpUnitCamera;
        Camera m_PushUnitCamera;
        PlayerController m_JumpUnitPC;
        PlayerController m_PushUnitPC;
        Sound  m_SFX;

        enum Ability
        {
            STOP_MOVING_PLATFORM,
            GROW,
            SHRINK
        };

        Ability m_Current_Ability = Ability.STOP_MOVING_PLATFORM;
        float m_ScaleFactor = 1.5f;

        enum PushableState
        {
            SHRUNK = 0,
            NORMAL,
            GROWN
        };

        public static GameController getInst()
        {
            return new GameController();
        }
        public void Start(UInt32 ID)
        {
            
            m_ID = ID;
            m_JumpID = (UInt32)Player.GetJumpUnitID();
            m_PushID = (UInt32)Player.GetPushUnitID();

            m_JumpUnitCamera = new Camera(m_JumpID);
            m_PushUnitCamera = new Camera(m_PushID);
            m_JumpUnitPC = new PlayerController(m_JumpID);
            m_PushUnitPC = new PlayerController(m_PushID);


            m_SFX = new Sound(ID);

            m_JumpUnitCamera.m_Active = true;
            m_PushUnitCamera.m_Active = false;
        }
        public void Update(float dt)
        {
            if (Input.IsKeyPress(Input.PB_Q) && !(m_JumpUnitPC.m_FPSMode || m_PushUnitPC.m_FPSMode))
            {
                m_SFX.m_Trigger = true;
                Player.TogglePlayers();
            }

            if (Input.IsMousePress(Input.PB_MOUSE_BUTTON_1))
            {
                if (m_JumpUnitPC.m_FPSMode)
                {
                    CastRay(m_JumpID);
                }

                else if (m_PushUnitPC.m_FPSMode)
                {
                    CastRay(m_PushID);
                }
            }

            if (m_JumpUnitPC.m_FPSMode || m_PushUnitPC.m_FPSMode)
            {
                if (Input.IsKeyPress(Input.PB_TAB))
                {
                    m_Current_Ability = (Ability)(((uint)++m_Current_Ability) % 3);
                }
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

        private void CastRay(UInt32 id)
        {
            UInt32[] collided_ids = Tools.Raycast.rayaab(id);

            foreach (UInt32 collided_id in collided_ids)
            {
                switch (m_Current_Ability)
                {
                    case Ability.STOP_MOVING_PLATFORM:
                    {
                        Name name = new Name(collided_id);

                        if (name.m_Name == "Moving Platform")
                        {
                            PathFollower path_follower = new PathFollower(collided_id);
                            path_follower.m_PauseTravel = true;
                        }

                        break;
                    }

                    case Ability.GROW:
                    {
                        if (Tools.Tag.IsPushable(collided_id))
                        {
                            Pushable pushable = new Pushable(collided_id);

                            if (pushable.m_State != ((uint)PushableState.GROWN))
                            {
                                Scale scale = new Scale(collided_id);
                                scale.m_Value = new Tools.MathLib.Vector3(scale.m_Value.x * m_ScaleFactor, scale.m_Value.y * m_ScaleFactor, scale.m_Value.z * m_ScaleFactor);

                                BoundingBox bounding_box = new BoundingBox(collided_id);
                                bounding_box.Min = new Tools.MathLib.Vector3(bounding_box.Min.x * m_ScaleFactor, bounding_box.Min.y * m_ScaleFactor, bounding_box.Min.z * m_ScaleFactor);
                                bounding_box.Max = new Tools.MathLib.Vector3(bounding_box.Max.x * m_ScaleFactor, bounding_box.Max.y * m_ScaleFactor, bounding_box.Max.z * m_ScaleFactor);
                                pushable.m_State = ++pushable.m_State;
                            }
                        }

                        break;
                    }

                    case Ability.SHRINK:
                    {
                        if (Tools.Tag.IsPushable(collided_id))
                        {
                            Pushable pushable = new Pushable(collided_id);

                            if (pushable.m_State != ((uint)PushableState.SHRUNK))
                            {
                                Scale scale = new Scale(collided_id);
                                scale.m_Value = new Tools.MathLib.Vector3(scale.m_Value.x / m_ScaleFactor, scale.m_Value.y / m_ScaleFactor, scale.m_Value.z / m_ScaleFactor);

                                BoundingBox bounding_box = new BoundingBox(collided_id);
                                bounding_box.Min = new Tools.MathLib.Vector3(bounding_box.Min.x / m_ScaleFactor, bounding_box.Min.y / m_ScaleFactor, bounding_box.Min.z / m_ScaleFactor);
                                bounding_box.Max = new Tools.MathLib.Vector3(bounding_box.Max.x / m_ScaleFactor, bounding_box.Max.y / m_ScaleFactor, bounding_box.Max.z / m_ScaleFactor);
                                pushable.m_State = --pushable.m_State;
                            }
                        }

                        break;
                    }
                }
            }
        }
    }
}
