using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class CollisionLayer
    {
        private void* m_Address;
        private UInt32 m_ID;

        public enum CollisionLayerEnum : sbyte
        {
            NONE,
            BACKGROUND,
            ENEMYBASE,
            PLAYERBASE,
            UNIT,

            ROUTE_CENTER_1,
            ROUTE1_CENTER_2,
            ROUTE1_CENTER_3,

            ROUTE2_SIDE_1,
            ROUTE2_SIDE_2,
            ROUTE2_SIDE_3,

            ROUTE3_COMPLEX_1,
            ROUTE3_COMPLEX_2,
            ROUTE3_COMPLEX_3,
            ROUTE3_COMPLEX_4,
            ROUTE3_COMPLEX_5,

            // extras
            INTERACTABLE,
            HOLE
        };

        public CollisionLayer(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public CollisionLayerEnum m_CollisionLayer
        {
            get
            {
                return (CollisionLayerEnum)getcollisionlayer(m_Address);
            }
            set
            {
                setcollisionlayer(m_Address, (sbyte)value);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static sbyte getcollisionlayer(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setcollisionlayer(void* address, sbyte value);
    }
}
