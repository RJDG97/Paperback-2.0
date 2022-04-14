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
    UInt32 ID;

    public ParticleEmitter(UInt32 id)
    {
            ID = id;
            m_Address = getaddress(id);
    }

    public float m_Lifetime
    {
      get
      {
        return GetEmitterLifetime(ID);
      }
      set
      {
        SetEmitterLifetime(ID, value);
      }
    }

    public String m_ParticleTexture
    {
        get
        {
            return GetParticleTexture(ID);
        }
        set
        {
            SetParticleTexture(ID, value);
        }
    }

    public int m_EmissionRate
        {
        get
        {
            return GetEmissionRate(ID);
        }
        set
        {
            SetEmissionRate(ID, value);
        }
    }

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void* getaddress( UInt32 ID );

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static float GetEmitterLifetime(UInt32 ID);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetEmitterLifetime(UInt32 ID, float time );

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static String GetParticleTexture(UInt32 ID);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetParticleTexture(UInt32 ID, String texture);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static int GetEmissionRate(UInt32 ID);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void SetEmissionRate(UInt32 ID, int value);

    }
}
