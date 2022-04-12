using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
  public unsafe class ParticleEmitter
  {
    private void* m_Address;

    public ParticleEmitter(UInt32 id)
    {
      m_Address = getaddress(id);
    }

    public float m_Lifetime
    {
      get
      {
        return GetEmitterLifetime(m_Address);
      }
      set
      {
        SetEmitterLifetime(m_Address, value);
      }
    }

    public String m_ParticleTexture
    {
        get
        {
            return GetParticleTexture(m_Address);
        }
        set
        {
            SetParticleTexture(m_Address, value);
        }
    }

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void* getaddress( UInt32 ID );

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static float GetEmitterLifetime( void* address );

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetEmitterLifetime( void* address, float time );

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static String GetParticleTexture(void* address);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetParticleTexture(void* address, String texture);

    }
}
