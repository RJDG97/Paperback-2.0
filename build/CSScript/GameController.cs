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
        float m_AbilityDuration = 3.0f;
        float m_AbilityTimer = 0.0f;
        bool m_AbilityActive = false;

        UInt32 m_SelectedID;
        Ability m_AbilityUsed;

        Mesh m_AbilityBar;
        Mesh m_InnerBar;
        Offset m_InnerBarOffset;
        Tools.MathLib.Vector3 m_InnerBarInitialPos;
        Tools.MathLib.Vector3 m_InnerBarInitialScale;

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

            Parent m_Parent = new Parent(m_ID);
            Int32 m_ChildID = m_Parent.GetChildIDofName("Ability Bar");

            if (m_ChildID != -1)
            {
                m_AbilityBar = new Mesh((UInt32)m_ChildID);
                m_AbilityBar.m_Active = false;

                Parent m_AbilityBarParent = new Parent((UInt32)m_ChildID);
                Int32 m_InnerBarID = m_AbilityBarParent.GetChildIDofName("Inner Bar");
                
                if (m_InnerBarID != -1)
                {
                    m_InnerBar = new Mesh((UInt32)m_InnerBarID);
                    m_InnerBarOffset = new Offset((UInt32)m_InnerBarID);
                    m_InnerBarInitialPos = m_InnerBarOffset.m_PosOffset;
                    m_InnerBarInitialScale = m_InnerBarOffset.m_ScaleOffset;
                    m_InnerBar.m_Active = false;
                }
            }

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

            if (Input.IsMousePress(Input.PB_MOUSE_BUTTON_1) && !m_AbilityActive)
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

            if (m_AbilityActive)
            {
                if (m_AbilityTimer > m_AbilityDuration)
                {
                    m_AbilityActive = false;
                    m_AbilityTimer = 0.0f;

                    m_AbilityBar.m_Active = false;
                    m_InnerBar.m_Active = false;

                    switch (m_AbilityUsed)
                    {
                        case Ability.STOP_MOVING_PLATFORM:
                        {
                            Name name = new Name(m_SelectedID);

                            if (name.m_Name == "Moving Platform" || name.m_Name == "Moving Billboard")
                            {
                                PathFollower path_follower = new PathFollower(m_SelectedID);
                                path_follower.m_PauseTravel = false;
                            
                                Mesh collided_mesh = new Mesh(m_SelectedID);
                                collided_mesh.m_Model = collided_mesh.m_Model.Substring(0, collided_mesh.m_Model.Length - 7);
                            }

                            else if (name.m_Name == "Platform")
                            {
                                Child child = new Child(m_SelectedID);
                                Animator parent_animator = new Animator((UInt32)child.m_ParentID);
                                Mesh parent_mesh = new Mesh((UInt32)child.m_ParentID);
                                parent_mesh.m_Model = parent_mesh.m_Model.Substring(0, parent_mesh.m_Model.Length - 7);
                                parent_animator.m_PauseAnimation = false;
                            }

                            else if (name.m_Name == "Elevator")
                            {
                                Animator animator = new Animator(m_SelectedID);
                                Mesh collided_mesh = new Mesh(m_SelectedID);
                                collided_mesh.m_Model = collided_mesh.m_Model.Substring(0, collided_mesh.m_Model.Length - 7);
                                animator.m_PauseAnimation = true;
                            }

                            break;
                        }

                        case Ability.GROW:
                        {
                            Pushable pushable = new Pushable(m_SelectedID);

                            Scale scale = new Scale(m_SelectedID);
                            scale.m_Value = new Tools.MathLib.Vector3(scale.m_Value.x / m_ScaleFactor, scale.m_Value.y / m_ScaleFactor, scale.m_Value.z / m_ScaleFactor);

                            BoundingBox bounding_box = new BoundingBox(m_SelectedID);
                            bounding_box.Min = new Tools.MathLib.Vector3(bounding_box.Min.x / m_ScaleFactor, bounding_box.Min.y / m_ScaleFactor, bounding_box.Min.z / m_ScaleFactor);
                            bounding_box.Max = new Tools.MathLib.Vector3(bounding_box.Max.x / m_ScaleFactor, bounding_box.Max.y / m_ScaleFactor, bounding_box.Max.z / m_ScaleFactor);

                            Rigidforce rigid_force = new Rigidforce(m_SelectedID);
                            rigid_force.m_CollisionAffected = true;
                            rigid_force.m_GravityAffected = true;

                            pushable.m_State = --pushable.m_State;
                            break;
                        }

                        case Ability.SHRINK:
                        {
                            Pushable pushable = new Pushable(m_SelectedID);
                            Scale scale = new Scale(m_SelectedID);
                            scale.m_Value = new Tools.MathLib.Vector3(scale.m_Value.x * m_ScaleFactor, scale.m_Value.y * m_ScaleFactor, scale.m_Value.z * m_ScaleFactor);

                            BoundingBox bounding_box = new BoundingBox(m_SelectedID);
                            bounding_box.Min = new Tools.MathLib.Vector3(bounding_box.Min.x * m_ScaleFactor, bounding_box.Min.y * m_ScaleFactor, bounding_box.Min.z * m_ScaleFactor);
                            bounding_box.Max = new Tools.MathLib.Vector3(bounding_box.Max.x * m_ScaleFactor, bounding_box.Max.y * m_ScaleFactor, bounding_box.Max.z * m_ScaleFactor);

                            Rigidforce rigid_force = new Rigidforce(m_SelectedID);
                            rigid_force.m_CollisionAffected = true;
                            rigid_force.m_GravityAffected = true;

                            pushable.m_State = ++pushable.m_State;
                            break;
                        }
                    }
                }

                else
                {
                    m_AbilityTimer += dt;
                    m_InnerBarOffset.m_ScaleOffset = new Tools.MathLib.Vector3(m_InnerBarInitialScale.x * (m_AbilityDuration - m_AbilityTimer) / m_AbilityDuration,
                                                                               m_InnerBarOffset.m_ScaleOffset.y,
                                                                               m_InnerBarOffset.m_ScaleOffset.z);

                    m_InnerBarOffset.m_PosOffset = new Tools.MathLib.Vector3(m_InnerBarInitialPos.x - (m_InnerBarInitialScale.x * (m_AbilityTimer) / m_AbilityDuration) * 200.0f,
                                                                             m_InnerBarInitialPos.y,
                                                                             m_InnerBarInitialPos.z);
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

                        if ( name.m_Name == "Moving Platform" || name.m_Name == "Moving Billboard" /*&& (m_JumpUnitPC.m_FreezeAvailable || m_PushUnitPC.m_FreezeAvailable)*/ )
                        {
                            PathFollower path_follower = new PathFollower(collided_id);

                            if (path_follower.m_Distance > 0.0001f)
                            {
                                path_follower.m_PauseTravel = !path_follower.m_PauseTravel;

                                m_AbilityActive = true;
                                m_SelectedID = collided_id;
                                m_AbilityUsed = Ability.STOP_MOVING_PLATFORM;

                                Mesh collided_mesh = new Mesh(collided_id);
                                collided_mesh.m_Model = collided_mesh.m_Model + "_Freeze";
                                    
                                ChangeBar();
                                return;
                            }
                        }

                        else if (name.m_Name == "Platform")
                        {
                            m_AbilityActive = true;
                            m_SelectedID = collided_id;
                            m_AbilityUsed = Ability.STOP_MOVING_PLATFORM;

                            Child child  = new Child(collided_id);
                            Animator parent_animator = new Animator((UInt32)child.m_ParentID);
                            Mesh parent_mesh = new Mesh((UInt32)child.m_ParentID);
                            parent_mesh.m_Model = parent_mesh.m_Model + "_Freeze";
                            parent_animator.m_PauseAnimation = true;
                            ChangeBar();
                        }

                        else if (name.m_Name == "Elevator")
                        {
                            m_AbilityActive = true;
                            m_SelectedID = collided_id;
                            m_AbilityUsed = Ability.STOP_MOVING_PLATFORM;

                            Animator animator = new Animator(collided_id);
                            Mesh collided_mesh = new Mesh(collided_id);
                            collided_mesh.m_Model = collided_mesh.m_Model + "_Freeze";
                            animator.m_PauseAnimation = true;
                            ChangeBar();
                        }

                        break;
                    }

                    case Ability.GROW:
                    {
                        if (Tools.Tag.IsPushable(collided_id))
                        {
                            Pushable pushable = new Pushable(collided_id);

                            if ( pushable.m_State != ((uint)PushableState.GROWN) /*&& (m_JumpUnitPC.m_GrowAvailable || m_PushUnitPC.m_GrowAvailable)*/ )
                            {
                                Scale scale = new Scale(collided_id);
                                scale.m_Value = new Tools.MathLib.Vector3(scale.m_Value.x * m_ScaleFactor, scale.m_Value.y * m_ScaleFactor, scale.m_Value.z * m_ScaleFactor);

                                BoundingBox bounding_box = new BoundingBox(collided_id);
                                bounding_box.Min = new Tools.MathLib.Vector3(bounding_box.Min.x * m_ScaleFactor, bounding_box.Min.y * m_ScaleFactor, bounding_box.Min.z * m_ScaleFactor);
                                bounding_box.Max = new Tools.MathLib.Vector3(bounding_box.Max.x * m_ScaleFactor, bounding_box.Max.y * m_ScaleFactor, bounding_box.Max.z * m_ScaleFactor);

                                Rigidforce rigid_force = new Rigidforce(collided_id);
                                rigid_force.m_CollisionAffected = true;
                                rigid_force.m_GravityAffected = true;

                                pushable.m_State = ++pushable.m_State;

                                m_AbilityActive = true;
                                m_SelectedID = collided_id;
                                m_AbilityUsed = Ability.GROW;
                                ChangeBar();
                                return;
                            }
                        }

                        break;
                    }

                    case Ability.SHRINK:
                    {
                        if (Tools.Tag.IsPushable(collided_id))
                        {
                            Pushable pushable = new Pushable(collided_id);

                            if (pushable.m_State != ((uint)PushableState.SHRUNK) /*&& (m_JumpUnitPC.m_ShrinkAvailable || m_PushUnitPC.m_ShrinkAvailable)*/ )
                            {
                                Scale scale = new Scale(collided_id);
                                scale.m_Value = new Tools.MathLib.Vector3(scale.m_Value.x / m_ScaleFactor, scale.m_Value.y / m_ScaleFactor, scale.m_Value.z / m_ScaleFactor);

                                BoundingBox bounding_box = new BoundingBox(collided_id);
                                bounding_box.Min = new Tools.MathLib.Vector3(bounding_box.Min.x / m_ScaleFactor, bounding_box.Min.y / m_ScaleFactor, bounding_box.Min.z / m_ScaleFactor);
                                bounding_box.Max = new Tools.MathLib.Vector3(bounding_box.Max.x / m_ScaleFactor, bounding_box.Max.y / m_ScaleFactor, bounding_box.Max.z / m_ScaleFactor);

                                Rigidforce rigid_force = new Rigidforce(collided_id);
                                rigid_force.m_CollisionAffected = true;
                                rigid_force.m_GravityAffected = true;

                                pushable.m_State = --pushable.m_State;

                                m_AbilityActive = true;
                                m_SelectedID = collided_id;
                                m_AbilityUsed = Ability.SHRINK;
                                ChangeBar();
                                return;
                            }
                        }

                        break;
                    }
                }
            }
        }

        private void ChangeBar()
        {
            m_AbilityBar.m_Active = true;
            m_InnerBar.m_Active = true;

            m_InnerBarOffset.m_PosOffset = m_InnerBarInitialPos;
            m_InnerBarOffset.m_ScaleOffset = m_InnerBarInitialScale;

            switch (m_AbilityUsed)
            {
                case Ability.STOP_MOVING_PLATFORM:
                {
                    m_AbilityBar.m_Texture = "SkillUIBase_Freeze";
                    m_InnerBar.m_Texture = "SkillUIBigBar_Freeze";
                    break;
                }

                case Ability.GROW:
                {
                    m_AbilityBar.m_Texture = "SkillUIBase_Grow";
                    m_InnerBar.m_Texture = "SkillUIBigBar_Grow";
                    break;
                }

                case Ability.SHRINK:
                {
                    m_AbilityBar.m_Texture = "SkillUIBase_Shrink";
                    m_InnerBar.m_Texture = "SkillUIBigBar_Shrink";
                    break;
                }
            }
        }
    }

}
