/**********************************************************************************
*\file         paperback_nbitset_inline.h
*\brief        paperback_nbitset_inline.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

namespace tools
{
    //-----------------------------------
    //              Add
    //-----------------------------------

    template < size_t MAX_SIZE >
    void n_bits<MAX_SIZE>::Set( const int Bit ) noexcept
    {
		int x = Bit / 64;
		int y = Bit % 64;
		m_Bits[x] |= (1ull << y);
    }

    template < size_t MAX_SIZE >
    void n_bits<MAX_SIZE>::Remove( const int Bit ) noexcept
    {
        int x = Bit / 64;
        int y = Bit % 64;
        m_Bits[x] &= ~(1ull << y);
    }


    //-----------------------------------
    //            Compare
    //-----------------------------------

    template < size_t MAX_SIZE >
    bool n_bits<MAX_SIZE>::Has( const int Bit ) const noexcept
    {
		int x = Bit / 64;
		int y = Bit % 64;
		
		return !!( m_Bits[x] & (1ull << y) );
    }

    template < size_t MAX_SIZE >
    bool n_bits<MAX_SIZE>::None( const int Bit ) const noexcept
    {
		int x = Bit / 64;
		int y = Bit % 64;
		
		return !( m_Bits[x] & (1ull << y) );
    }

    template < size_t MAX_SIZE >
    bool n_bits<MAX_SIZE>::Match( const n_bits& Query ) const noexcept
    {
        PPB_ASSERT_MSG( Query.m_Bits.size() != m_Bits.size(), "n_bits::Match - Bit Array Size Is Different!" );

		for (size_t i = 0, size = m_Bits.size(); i < size; ++i)
        {
            if ( m_Bits[i] != Query.m_Bits[i] )
                return false;
        }

        return true;
    }

    template < size_t MAX_SIZE >
    bool n_bits<MAX_SIZE>::Compare( const n_bits& Query ) const noexcept
    {
        PPB_ASSERT_MSG( Query.m_Bits.size() != m_Bits.size(), "n_bits::Match - Bit Array Size Is Different!" );

		for (size_t i = 0, size = m_Bits.size(); i < size; ++i)
        {
            if ( (m_Bits[i] & Query.m_Bits[i]) != Query.m_Bits[i] )
                return false;
        }

        return true;
    }
}