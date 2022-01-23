using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public class Transform
    {
        private UInt32 m_ID;

        public Tools.MathLib.Vector3 m_Offset
        {
            get
            {
                return getoffset(m_ID);
            }
            set
            {
                //m_Offset = value;
                setoffset(m_ID, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_Position
        {
            get
            {
                return getposition(m_ID);
            }
            set
            {
                //m_Position = value;
                setposition(m_ID, value.x, value.y, value.z);
            }
        }

        public Transform(UInt32 ID)
        {
            // m_ID = ID;
            m_ID = 40; // For testing (Cardback)
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getoffset(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setoffset(UInt32 ID, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getposition(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setposition(UInt32 ID, float x, float y, float z);
    }
}
