/**********************************************************************************
*\file         GameController.cs
*\brief        GameController.cs Script
*
*\author	     Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			         or disclosure of this file or its contents without the prior
			         written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Linq;

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
        float m_FlickerCooldownDuration = 0.1f;
        float m_FlickerCooldownTimer;

        enum Ability
        {
            STOP_MOVING_PLATFORM,
            GROW,
            SHRINK,
            NONE
        };

        float m_AbilitySwapCoolDownTimer = -1.0f;
        float m_RayCastTimer = -1.0f;
        float m_ScaleFactor = 1.5f;
        float m_InverseScaleFactor = 1.0f / 1.5f;
        float m_AbilityDuration = 5.0f;
        float m_InverseAbilityDuration = 1.0f / 5.0f;
        float m_AbilityTimer = 0.0f;
        bool m_AbilityActive = false;

        UInt32 m_HoveredID;
        Ability m_AbilityHovered;
        float m_HoverRayCastTimer = -1.0f;

        UInt32 m_SelectedID;
        Ability m_AbilityUsed;

        Mesh m_AbilityBar;
        Mesh m_InnerBar;
        Offset m_InnerBarOffset;
        Tools.MathLib.Vector3 m_InnerBarInitialPos;
        Tools.MathLib.Vector3 m_InnerBarInitialScale;

        bool m_JustEnteredFPS = false;
        bool m_JustLeftFPS = true;

        List<Ability> m_Abilities = new List<Ability>();

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

        public void PreUpdate(float dt)
        {
            Application.NotifyDone();
        }

        public void Update(float dt)
        {
            if (m_RayCastTimer > 0.0f)
                m_RayCastTimer -= dt;

            if (m_HoverRayCastTimer > 0.0f)
                m_HoverRayCastTimer -= dt;


            if ((Input.IsKeyPress(Input.PB_Q) || Input.IsKeyPress(Input.PB_GAMEPAD_BUTTON_Y))  && !(m_JumpUnitPC.m_FPSMode || m_PushUnitPC.m_FPSMode))
            {
                m_SFX.m_Trigger = true;
                Player.TogglePlayers();
            }

            if (!m_AbilityActive && m_RayCastTimer < 0.0f && (Input.IsMousePress(Input.PB_MOUSE_BUTTON_1) || Input.IsGamepadButtonPressDown(Input.PB_GAMEPAD_BUTTON_RIGHT_BUMPER)))
            {
                if (m_JumpUnitPC.m_FPSMode)
                {
                    CastRay(m_JumpID);
                    m_RayCastTimer = 0.1f;
                }

                else if (m_PushUnitPC.m_FPSMode)
                {
                    CastRay(m_PushID);
                    m_RayCastTimer = 0.1f;
                }
            }

            CheckAbilitiesUnlocked();

            if ((m_JumpUnitPC.m_FPSMode || m_PushUnitPC.m_FPSMode) && m_Abilities.Count > 0)
            {
                m_JustLeftFPS = false;

                if (!m_JustEnteredFPS)
                {
                    m_JustEnteredFPS = true;
                    ChangeToUnhovered();
                }

                if (m_HoverRayCastTimer > 0.0f)
                    m_HoverRayCastTimer -= dt;

                if (m_HoverRayCastTimer <= 0.0f)
                {
                    m_HoverRayCastTimer = 0.15f;
                    CastHoveredRay();
                }

                if (m_AbilitySwapCoolDownTimer > 0.0f)
                    m_AbilitySwapCoolDownTimer -= dt;

                if (m_Abilities.Count > 1 && m_AbilitySwapCoolDownTimer < 0.0f && (Input.IsKeyPress(Input.PB_TAB) || Input.IsGamepadButtonPressDown(Input.PB_GAMEPAD_BUTTON_X)))
                {
                    RevertHovered(true);
                    m_HoveredID = 0;
                    RevertUnhovered();

                    m_AbilitySwapCoolDownTimer = 0.15f;
                    Ability first = m_Abilities[0];
                    m_Abilities.RemoveAt(0);
                    m_Abilities.Add(first);

                    ChangeToUnhovered();
                }
            }

            else
            {
                m_JustEnteredFPS = false;

                if (!m_JustLeftFPS)
                {
                    m_JustLeftFPS = true;
                    RevertHovered(false);
                    RevertUnhovered();
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
                            Freezable freezable = new Freezable((UInt32)m_SelectedID);
                            freezable.m_Frozen = false;

                            if (name.m_Name == "Moving Platform" || name.m_Name == "Moving Billboard")
                            {
                                PathFollower path_follower = new PathFollower(m_SelectedID);
                                path_follower.m_PauseTravel = false;
                            
                                Mesh collided_mesh = new Mesh(m_SelectedID);
                                if (collided_mesh.m_Model.Substring(collided_mesh.m_Model.Length - 7, 7) == "_Freeze")
                                {
                                    collided_mesh.m_Model = collided_mesh.m_Model.Substring(0, collided_mesh.m_Model.Length - 7);
                                }

                                if ((m_JumpUnitPC.m_FPSMode || m_PushUnitPC.m_FPSMode) && m_Abilities.Count > 0)
                                    if (m_AbilityUsed == m_Abilities[0])
                                        collided_mesh.m_Model = collided_mesh.m_Model + "_Freeze_Unhovered";
                            }

                            else if (name.m_Name == "Elevator" || name.m_Name == "Gate")
                            {
                                Animator animator = new Animator(m_SelectedID);

                                Mesh collided_mesh = new Mesh(m_SelectedID);
                                if (collided_mesh.m_Model.Substring(collided_mesh.m_Model.Length - 7, 7) == "_Freeze")
                                {
                                    collided_mesh.m_Model = collided_mesh.m_Model.Substring(0, collided_mesh.m_Model.Length - 7);
                                }

                                if ((m_JumpUnitPC.m_FPSMode || m_PushUnitPC.m_FPSMode) && m_Abilities.Count > 0)
                                    if (m_AbilityUsed == m_Abilities[0])
                                        collided_mesh.m_Model = collided_mesh.m_Model + "_Freeze_Unhovered";

                                animator.m_PauseAnimation = false;
                            }

                            break;
                        }

                        case Ability.GROW:
                        {
                            Shrink(m_SelectedID);
                            Mesh collided_mesh = new Mesh(m_SelectedID);
                            if (collided_mesh.m_Model.Substring(collided_mesh.m_Model.Length - 5, 5) == "_Grow")
                            {
                                collided_mesh.m_Model = collided_mesh.m_Model.Substring(0, collided_mesh.m_Model.Length - 5);
                            }

                            if ((m_JumpUnitPC.m_FPSMode || m_PushUnitPC.m_FPSMode) && m_Abilities.Count > 0)
                            {
                                if (m_Abilities[0] == Ability.GROW)
                                    collided_mesh.m_Model = collided_mesh.m_Model + "_Grow_Unhovered";

                                else if (m_Abilities[0] == Ability.SHRINK)
                                    collided_mesh.m_Model = collided_mesh.m_Model + "_Shrink_Unhovered";
                            }

                            break;
                        }

                        case Ability.SHRINK:
                        {
                            Grow(m_SelectedID);
                            Mesh collided_mesh = new Mesh(m_SelectedID);
                            if (collided_mesh.m_Model.Substring(collided_mesh.m_Model.Length - 7, 7) == "_Shrink")
                            {
                                collided_mesh.m_Model = collided_mesh.m_Model.Substring(0, collided_mesh.m_Model.Length - 7);
                            }
                                    
                            if ((m_JumpUnitPC.m_FPSMode || m_PushUnitPC.m_FPSMode) && m_Abilities.Count > 0)
                            {
                                if (m_Abilities[0] == Ability.GROW)
                                    collided_mesh.m_Model = collided_mesh.m_Model + "_Grow_Unhovered";

                                else if (m_Abilities[0] == Ability.SHRINK)
                                    collided_mesh.m_Model = collided_mesh.m_Model + "_Shrink_Unhovered";
                            }

                            break;
                        }
                    }
                }

                else
                {
                    if (m_AbilityTimer > m_AbilityDuration - 1.0f)
                    {
                        m_FlickerCooldownTimer += dt;

                        if (m_FlickerCooldownTimer > m_FlickerCooldownDuration)
                        {
                            m_FlickerCooldownTimer = 0.0f;

                            switch (m_AbilityUsed)
                            {
                                case Ability.STOP_MOVING_PLATFORM:
                                {
                                    Mesh collided_mesh = new Mesh(m_SelectedID);

                                    if (collided_mesh.m_Model.Length <= 7)
                                    {
                                        collided_mesh.m_Model = collided_mesh.m_Model + "_Freeze";
                                    }

                                    else
                                    {
                                        if (collided_mesh.m_Model.Substring(collided_mesh.m_Model.Length - 7, 7) == "_Freeze")
                                        {
                                            collided_mesh.m_Model = collided_mesh.m_Model.Substring(0, collided_mesh.m_Model.Length - 7);
                                        }

                                        else
                                        {
                                            collided_mesh.m_Model = collided_mesh.m_Model + "_Freeze";
                                        }
                                    }

                                    break;
                                }

                                case Ability.GROW:
                                {
                                    Mesh collided_mesh = new Mesh(m_SelectedID);

                                    if (collided_mesh.m_Model.Length <= 5)
                                    {
                                        collided_mesh.m_Model = collided_mesh.m_Model + "_Grow";
                                    }

                                    else
                                    {
                                        if (collided_mesh.m_Model.Substring(collided_mesh.m_Model.Length - 5, 5) == "_Grow")
                                        {
                                            collided_mesh.m_Model = collided_mesh.m_Model.Substring(0, collided_mesh.m_Model.Length - 5);
                                        }

                                        else
                                        {
                                            collided_mesh.m_Model = collided_mesh.m_Model + "_Grow";
                                        }
                                    }

                                    break;
                                }

                                case Ability.SHRINK:
                                {
                                    Mesh collided_mesh = new Mesh(m_SelectedID);

                                    if (collided_mesh.m_Model.Length <= 7)
                                    {
                                        collided_mesh.m_Model = collided_mesh.m_Model + "_Shrink";
                                    }

                                    else
                                    {
                                        if (collided_mesh.m_Model.Substring(collided_mesh.m_Model.Length - 7, 7) == "_Shrink")
                                        {
                                            collided_mesh.m_Model = collided_mesh.m_Model.Substring(0, collided_mesh.m_Model.Length - 7);
                                        }

                                        else
                                        {
                                            collided_mesh.m_Model = collided_mesh.m_Model + "_Shrink";
                                        }
                                    }

                                    break;
                                }
                            }
                        }
                    }

                    m_AbilityTimer += dt;
                    m_InnerBarOffset.m_ScaleOffset = new Tools.MathLib.Vector3(m_InnerBarInitialScale.x * (m_AbilityDuration - m_AbilityTimer) * m_InverseAbilityDuration,
                                                                               m_InnerBarOffset.m_ScaleOffset.y,
                                                                               m_InnerBarOffset.m_ScaleOffset.z);

                    m_InnerBarOffset.m_PosOffset = new Tools.MathLib.Vector3(m_InnerBarInitialPos.x - (m_InnerBarInitialScale.x * (m_AbilityTimer) * m_InverseAbilityDuration) * 200.0f,
                                                                             m_InnerBarInitialPos.y,
                                                                             m_InnerBarInitialPos.z);
                }
            }

            Application.NotifyDone();
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
            if (m_AbilityActive)
            {
                m_AbilityTimer = m_AbilityDuration + 1.0f;
            }
        }

        private void CheckAbilitiesUnlocked()
        {
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
        }

        private void ChangeToUnhovered()
        {
            if (m_Abilities.Count > 0)
            {
                switch (m_Abilities[0])
                {
                    case Ability.STOP_MOVING_PLATFORM: ChangeToUnhoveredFreezable(); break;
                    case Ability.GROW: ChangeToUnhoveredGrowable(); break;
                    case Ability.SHRINK: ChangeToUnhoveredShrinkable(); break;
                }
            }
        }
        private void RevertUnhovered()
        {
            if (m_Abilities.Count > 0)
            {
                switch (m_Abilities[0])
                {
                    case Ability.STOP_MOVING_PLATFORM: RevertUnhoveredFreezable(); break;
                    case Ability.GROW: RevertUnhoveredGrowable(); break;
                    case Ability.SHRINK: RevertUnhoveredShrinkable(); break;
                }
            }
        }
        private void ChangeToUnhoveredFreezable()
        {
            UInt32[] collided_ids = Player.GetAllFreezeable();

            foreach (UInt32 collided_id in collided_ids)
            {
                Freezable freezable = new Freezable(collided_id);
                
                if (!freezable.m_Frozen && collided_id != m_HoveredID)
                {
                    Mesh mesh = new Mesh(collided_id);
                    mesh.m_Model = mesh.m_Model + "_Freeze_Unhovered";
                }
            }
        }
        private void RevertUnhoveredFreezable()
        {
            UInt32[] collided_ids = Player.GetAllFreezeable();

            foreach (UInt32 collided_id in collided_ids)
            {
                Freezable freezable = new Freezable(collided_id);

                if (!freezable.m_Frozen && collided_id != m_HoveredID)
                {
                    Mesh mesh = new Mesh(collided_id);
                    
                    if (mesh.m_Model.Length > 17)
                        mesh.m_Model = mesh.m_Model.Substring(0, mesh.m_Model.Length - 17);
                }
            }
        }
        private void ChangeToUnhoveredGrowable()
        {
            UInt32[] collided_ids = Player.GetAllPushable();

            foreach (UInt32 collided_id in collided_ids)
            {
                Pushable pushable = new Pushable(collided_id);

                if (pushable.m_State == 0 && collided_id != m_HoveredID)
                {
                    Mesh mesh = new Mesh(collided_id);
                    mesh.m_Model = mesh.m_Model + "_Grow_Unhovered";
                }
            }
        }
        private void RevertUnhoveredGrowable()
        {
            UInt32[] collided_ids = Player.GetAllPushable();

            foreach (UInt32 collided_id in collided_ids)
            {
                Pushable pushable = new Pushable(collided_id);

                if (pushable.m_State == 0 && collided_id != m_HoveredID)
                {
                    Mesh mesh = new Mesh(collided_id);

                    if (mesh.m_Model.Length > 15)
                        mesh.m_Model = mesh.m_Model.Substring(0, mesh.m_Model.Length - 15);
                }
            }
        }
        private void ChangeToUnhoveredShrinkable()
        {
            UInt32[] collided_ids = Player.GetAllPushable();

            foreach (UInt32 collided_id in collided_ids)
            {
                Pushable pushable = new Pushable(collided_id);

                if (pushable.m_State == 0 && collided_id != m_HoveredID)
                {
                    Mesh mesh = new Mesh(collided_id);
                    mesh.m_Model = mesh.m_Model + "_Shrink_Unhovered";
                }
            }
        }
        private void RevertUnhoveredShrinkable()
        {
            UInt32[] collided_ids = Player.GetAllPushable();

            foreach (UInt32 collided_id in collided_ids)
            {
                Pushable pushable = new Pushable(collided_id);

                if (pushable.m_State == 0 && collided_id != m_HoveredID)
                {
                    Mesh mesh = new Mesh(collided_id);

                    if (mesh.m_Model.Length > 17)
                        mesh.m_Model = mesh.m_Model.Substring(0, mesh.m_Model.Length - 17);
                }
            }
        }

        private void RevertHovered(bool in_fps)
        {
            if (m_Abilities.Count > 0 && m_HoveredID > 0)
            {
                Mesh mesh = new Mesh(m_HoveredID);
                switch (m_AbilityHovered)
                {
                    case Ability.STOP_MOVING_PLATFORM:
                    {
                        if (mesh.m_Model.Length > 15)
                            if (mesh.m_Model.Substring(mesh.m_Model.Length - 15, 15) == "_Freeze_Hovered")
                                mesh.m_Model = mesh.m_Model.Substring(0, mesh.m_Model.Length - 15);

                        if (in_fps && m_Abilities[0] == m_AbilityHovered)
                            mesh.m_Model = mesh.m_Model + "_Freeze_Unhovered";

                        break;
                    }

                    case Ability.GROW:
                    {
                        if (mesh.m_Model.Length > 13)
                            if (mesh.m_Model.Substring(mesh.m_Model.Length - 13, 13) == "_Grow_Hovered")
                                mesh.m_Model = mesh.m_Model.Substring(0, mesh.m_Model.Length - 13);
                        
                        if (in_fps && m_Abilities[0] == m_AbilityHovered)
                            mesh.m_Model = mesh.m_Model + "_Grow_Unhovered";
                        
                        break;
                    }

                    case Ability.SHRINK:
                    {
                        if (mesh.m_Model.Length > 15)
                            if (mesh.m_Model.Substring(mesh.m_Model.Length - 15, 15) == "_Shrink_Hovered")
                                mesh.m_Model = mesh.m_Model.Substring(0, mesh.m_Model.Length - 15);

                        if (in_fps && m_Abilities[0] == m_AbilityHovered)
                            mesh.m_Model = mesh.m_Model + "_Shrink_Unhovered";
                        
                        break;
                    }
                }
            }
        }

        private void CastHoveredRay()
        {
            UInt32 cam_id = 0;

            if (m_JumpUnitPC.m_FPSMode)
            {
                cam_id = m_JumpID;
            }

            else if (m_PushUnitPC.m_FPSMode)
            {
                cam_id = m_PushID;
            }

            UInt32[] collided_ids = Tools.Raycast.rayaab(cam_id);

            if (m_Abilities.Count > 0)
            {
                foreach (UInt32 collided_id in collided_ids)
                {
                    switch (m_Abilities[0])
                    {
                        case Ability.STOP_MOVING_PLATFORM:
                        {
                            UInt32 temp_id = collided_id;
                            Name name = new Name(collided_id);

                            if (name.m_Name == "PlatformSlopeA")
                            {
                                Child child = new Child(collided_id);
                                Child nextchild = new Child((UInt32)child.m_ParentID);
                                temp_id = (UInt32)nextchild.m_ParentID;
                            }
                                
                            if (Tools.Tag.IsFreezable(temp_id))
                            {   
                                Freezable freezable = new Freezable(temp_id);
                                    
                                if (!freezable.m_Frozen)
                                {
                                    RevertHovered(true);
                                    m_HoveredID = temp_id;
                                    m_AbilityHovered = Ability.STOP_MOVING_PLATFORM;
                                    Mesh mesh = new Mesh(temp_id);
                                    
                                    if (mesh.m_Model.Length > 17)
                                        if (mesh.m_Model.Substring(mesh.m_Model.Length - 17, 17) == "_Freeze_Unhovered")
                                            mesh.m_Model = mesh.m_Model.Substring(0, mesh.m_Model.Length - 17);

                                    mesh.m_Model = mesh.m_Model + "_Freeze_Hovered";
                                    return;
                                }
                            }

                            break;
                        }

                        case Ability.GROW:
                        {
                            if (Tools.Tag.IsPushable(collided_id))
                            {
                                Pushable pushable = new Pushable(collided_id);

                                if (pushable.m_State == 0)
                                {
                                    RevertHovered(true);
                                    m_HoveredID = collided_id;
                                    m_AbilityHovered = Ability.GROW;
                                    Mesh mesh = new Mesh(collided_id);
                                    
                                    if (mesh.m_Model.Length > 15)
                                        if (mesh.m_Model.Substring(mesh.m_Model.Length - 15, 15) == "_Grow_Unhovered")
                                            mesh.m_Model = mesh.m_Model.Substring(0, mesh.m_Model.Length - 15);

                                    mesh.m_Model = mesh.m_Model + "_Grow_Hovered";
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

                                if (pushable.m_State == 0)
                                {
                                    RevertHovered(true);
                                    m_HoveredID = collided_id;
                                    m_AbilityHovered = Ability.SHRINK;
                                    Mesh mesh = new Mesh(collided_id);
                                    
                                    if (mesh.m_Model.Length > 17)
                                        if (mesh.m_Model.Substring(mesh.m_Model.Length - 17, 17) == "_Shrink_Unhovered")
                                            mesh.m_Model = mesh.m_Model.Substring(0, mesh.m_Model.Length - 17);

                                    mesh.m_Model = mesh.m_Model + "_Shrink_Hovered";
                                    return;
                                }
                            }

                            break;
                        }
                    }
                }
            }

            //doesn't find anything that is hovered over
            RevertHovered(true);
            m_HoveredID = 0;
        }

        private void CastRay(UInt32 id)
        {
            UInt32[] collided_ids = Tools.Raycast.rayaab(id);

            if (m_Abilities.Count > 0)
            { 
                foreach (UInt32 collided_id in collided_ids)
                {
                    switch (m_Abilities[0])
                    {
                        case Ability.STOP_MOVING_PLATFORM:
                        {
                            Name name = new Name(collided_id);

                            if ( name.m_Name == "Moving Platform" || name.m_Name == "Moving Billboard")
                            {
                                PathFollower path_follower = new PathFollower(collided_id);
                                Freezable freezable = new Freezable(collided_id);

                                freezable.m_Frozen = true;
                                path_follower.m_PauseTravel = true;

                                m_AbilityActive = true;
                                m_SelectedID = collided_id;
                                m_AbilityUsed = Ability.STOP_MOVING_PLATFORM;

                                RevertHovered(false);
                                m_HoveredID = 0;
                                Mesh collided_mesh = new Mesh(collided_id);
                                collided_mesh.m_Model = collided_mesh.m_Model + "_Freeze";

                                ChangeBar();
                                return;
                            }

                            else if (name.m_Name == "PlatformSlopeA")
                            {
                                RevertHovered(false);
                                m_HoveredID = 0;
                                Child child  = new Child(collided_id);
                                Child nextchild  = new Child((UInt32)child.m_ParentID);
                                FreezeAnim((UInt32)nextchild.m_ParentID);
                                return;
                            }

                            else if (name.m_Name == "Elevator" || name.m_Name == "Gate")
                            {
                                RevertHovered(false);
                                m_HoveredID = 0;
                                FreezeAnim(collided_id);
                                return;
                            }

                            break;
                        }

                        case Ability.GROW:
                        {
                            if (Tools.Tag.IsPushable(collided_id))
                            {
                                Pushable pushable = new Pushable(collided_id);
                                Grow(collided_id);
                                    
                                RevertHovered(false);
                                m_HoveredID = 0;
                                Mesh collided_mesh = new Mesh(collided_id);
                                collided_mesh.m_Model = collided_mesh.m_Model + "_Grow";

                                m_AbilityActive = true;
                                m_SelectedID = collided_id;
                                m_AbilityUsed = Ability.GROW;
                                ChangeBar();
                                return;
                            }

                            break;
                        }

                        case Ability.SHRINK:
                        {
                            if (Tools.Tag.IsPushable(collided_id))
                            {
                                Pushable pushable = new Pushable(collided_id);
                                Shrink(collided_id);
                                    
                                RevertHovered(false);
                                m_HoveredID = 0;
                                Mesh collided_mesh = new Mesh(collided_id);
                                collided_mesh.m_Model = collided_mesh.m_Model + "_Shrink";

                                m_AbilityActive = true;
                                m_SelectedID = collided_id;
                                m_AbilityUsed = Ability.SHRINK;
                                ChangeBar();
                                return;
                            }

                            break;
                        }
                    }
                }
            }
        }

        private void FreezeAnim(UInt32 ID)
        {
            Freezable freezable = new Freezable(ID);
            freezable.m_Frozen = true;

            m_AbilityActive = true;
            m_SelectedID = ID;
            m_AbilityUsed = Ability.STOP_MOVING_PLATFORM;

            Animator animator = new Animator(ID);
            Mesh collided_mesh = new Mesh(ID);
            collided_mesh.m_Model = collided_mesh.m_Model + "_Freeze";
            animator.m_PauseAnimation = true;
            ChangeBar();
        }

        private void Grow(UInt32 ID)
        {
            Pushable pushable = new Pushable(ID);
            pushable.m_State += 1;

            Scale scale = new Scale(ID);
            Mass mass = new Mass(ID);

            mass.m_Mass = 6.0f;
            scale.m_Value = new Tools.MathLib.Vector3(scale.m_Value.x * m_ScaleFactor, scale.m_Value.y * m_ScaleFactor, scale.m_Value.z * m_ScaleFactor);

            BoundingBox bounding_box = new BoundingBox(ID);
            bounding_box.Min = new Tools.MathLib.Vector3(bounding_box.Min.x * m_ScaleFactor, bounding_box.Min.y * m_ScaleFactor, bounding_box.Min.z * m_ScaleFactor);
            bounding_box.Max = new Tools.MathLib.Vector3(bounding_box.Max.x * m_ScaleFactor, bounding_box.Max.y * m_ScaleFactor, bounding_box.Max.z * m_ScaleFactor);

            Rigidforce rigid_force = new Rigidforce(ID);
            rigid_force.m_CollisionAffected = true;
            rigid_force.m_GravityAffected = true;
        }

        private void Shrink(UInt32 ID)
        {
            Pushable pushable = new Pushable(ID);
            pushable.m_State -= 1;

            Scale scale = new Scale(ID);
            Mass mass = new Mass(ID);

            mass.m_Mass = 6.0f;
            scale.m_Value = new Tools.MathLib.Vector3(scale.m_Value.x * m_InverseScaleFactor, scale.m_Value.y * m_InverseScaleFactor, scale.m_Value.z * m_InverseScaleFactor);

            BoundingBox bounding_box = new BoundingBox(ID);
            bounding_box.Min = new Tools.MathLib.Vector3(bounding_box.Min.x * m_InverseScaleFactor, bounding_box.Min.y * m_InverseScaleFactor, bounding_box.Min.z * m_InverseScaleFactor);
            bounding_box.Max = new Tools.MathLib.Vector3(bounding_box.Max.x * m_InverseScaleFactor, bounding_box.Max.y * m_InverseScaleFactor, bounding_box.Max.z * m_InverseScaleFactor);

            Rigidforce rigid_force = new Rigidforce(ID);
            rigid_force.m_CollisionAffected = true;
            rigid_force.m_GravityAffected = true;
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
