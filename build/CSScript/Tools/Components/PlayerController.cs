using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class PlayerController
    {
        private void* m_Address;

        public enum Ability
        {
            FREEZE,
            GROW,
            SHRINK,
            NONE
        }

        public PlayerController(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public bool m_FPSMode
        {
            get
            {
                return GetFPSMode(m_Address);
            }
            set 
            {
                SetFPSMode(m_Address, value);
            }
        }

        public Int32[] m_Abilities
        {
            get
            {
                return GetAbilities(m_Address);
            }
        }

        public int m_CheckpointID
        {
            get
            {
                return GetCheckpointID(m_Address);
            }
            set
            {
                SetCheckpointID(m_Address, value);
            }
        }

        public void AddAbility(Ability ability)
        {
            Add_Ability(m_Address, (Int32)ability);
        }

        public void RotateAbilities()
        {
            Rotate_Abilities(m_Address);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool GetFPSMode(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetFPSMode(void* address, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Int32[] GetAbilities(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void Add_Ability(void* address, int value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void Rotate_Abilities(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static int GetCheckpointID(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetCheckpointID(void* address, int value);
    }
}
