/**********************************************************************************
*\file         PathFollower.cs
*\brief        PathFollower.cs Script
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

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class PathFollower
    {
        private void* m_Address;
        UInt32 ID;
        public PathFollower(UInt32 id)
        {
            ID = id;
            m_Address = getaddress(id);
        }

        public int m_PathID
        {
            get
            {
                return getpathid(ID);
            }
            set
            {
                setpathid(ID, value);
            }
        }

        public float m_Distance
        {
            get
            {
                return getdistance(ID);
            }
            set
            {
                setdistance(ID, value);
            }
        }

        public float m_TravelSpeed
        {
            get
            {
                return gettravelspeed(ID);
            }
            set
            {
                settravelspeed(ID, value);
            }
        }

        public bool m_BackAndForth
        {
            get
            {
                return getbackandforth(ID);
            }
            set
            {
                //m_Position = value;
                setbackandforth(ID, value);
            }
        }

        public bool m_Reversed
        {
            get
            {
                return getreversed(ID);
            }
            set
            {
                setreversed(ID, value);
            }
        }

        public bool m_PauseTravel
        {
            get
            {
                return getpausetravel(ID);
            }
            set
            {
                //m_Position = value;
                setpausetravel(ID, value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static int getpathid(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setpathid(UInt32 ID, int path_id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getdistance(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setdistance(UInt32 ID, float distance);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float gettravelspeed(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void settravelspeed(UInt32 ID, float travel_speed);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getbackandforth(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setbackandforth(UInt32 ID, bool back_and_forth);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getreversed(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setreversed(UInt32 ID, bool traversed);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getpausetravel(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setpausetravel(UInt32 ID, bool stopped);
    }
}
