/**********************************************************************************
*\file         paperback_nbitset.h
*\brief        paperback_nbitset.h
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
    template < size_t MAX_SIZE = paperback::settings::max_entities_v >
    class n_bits
    {
    public:

        std::array< uint64_t, (MAX_SIZE / 64) + 1 > m_Bits{};

        //-----------------------------------
        //              Add
        //-----------------------------------
        PPB_FORCEINLINE
        void Set( const int Bit ) noexcept;

        PPB_FORCEINLINE
        void Remove( const int Bit ) noexcept;


        //-----------------------------------
        //            Compare
        //-----------------------------------
        PPB_FORCEINLINE
        bool Has( const int Bit ) const noexcept;

        PPB_FORCEINLINE
        bool None( const int Bit ) const noexcept;
		
        PPB_INLINE
        bool Match( const n_bits<MAX_SIZE>& Query ) const noexcept;

        PPB_INLINE
        bool Compare( const n_bits<MAX_SIZE>& Query ) const noexcept;


        /*
        //-----------------------------------
        //            Generate
        //-----------------------------------
        PPB_INLINE
        const paperback::u64 GenerateGUID( void ) const noexcept;
        */
    };
}