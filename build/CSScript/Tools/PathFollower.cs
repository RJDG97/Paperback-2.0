using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class PathFollower
    {
        private void* m_Address;
        private UInt32 m_ID;

        public PathFollower(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public int m_PathID
        {
            get
            {
                return getpathid(m_Address);
            }
            set
            {
                setpathid(m_Address, value);
            }
        }

        public float m_Distance
        {
            get
            {
                return getdistance(m_Address);
            }
            set
            {
                //m_Position = value;
                setdistance(m_Address, value);
            }
        }

        public float m_TravelSpeed
        {
            get
            {
                return gettravelspeed(m_Address);
            }
            set
            {
                settravelspeed(m_Address, value);
            }
        }

        public bool m_BackAndForth
        {
            get
            {
                return getbackandforth(m_Address);
            }
            set
            {
                //m_Position = value;
                setbackandforth(m_Address, value);
            }
        }

        public bool m_Reversed
        {
            get
            {
                return getreversed(m_Address);
            }
            set
            {
                setreversed(m_Address, value);
            }
        }

        public bool m_PauseTravel
        {
            get
            {
                return getpausetravel(m_Address);
            }
            set
            {
                //m_Position = value;
                setpausetravel(m_Address, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static int getpathid(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setpathid(void* address, int path_id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getdistance(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setdistance(void* address, float distance);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float gettravelspeed(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void settravelspeed(void* address, float travel_speed);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getbackandforth(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setbackandforth(void* address, bool back_and_forth);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getreversed(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setreversed(void* address, bool traversed);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getpausetravel(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setpausetravel(void* address, bool stopped);
    }
}
