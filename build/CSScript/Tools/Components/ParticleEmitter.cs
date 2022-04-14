/**********************************************************************************
*\file         ParticleEmitter.cs
*\brief        ParticleEmitter.cs Script
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

    public int m_EmissionRate
        {
        get
        {
            return GetEmissionRate(m_Address);
        }
        set
        {
            SetEmissionRate(m_Address, value);
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

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static int GetEmissionRate(void* address);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetEmissionRate(void* address, int value);

    }
}
