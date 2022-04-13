/**********************************************************************************
*\file         paperback_bitset.h
*\brief        paperback_bitset.h
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
    class bits
    {
    public:

        std::array<uint64_t, 4> m_bits{};

        //-----------------------------------
        //              Add
        //-----------------------------------
        PPB_FORCEINLINE
        void Set( const int Bit ) noexcept;

        PPB_FORCEINLINE
        void Remove( const int Bit ) noexcept;

        template < typename... T_COMPONENTS >
        void AddFromComponents();

        //-----------------------------------
        //            Compare
        //-----------------------------------
        PPB_FORCEINLINE
        bool Has( const int Bit ) const noexcept;

        template < typename... T_COMPONENTS >
        bool Has( void ) const noexcept;

        PPB_FORCEINLINE
        bool None( const int Bit ) const noexcept;
		
        PPB_INLINE
        bool Match( const bits& Query ) const noexcept;

        PPB_INLINE
        bool Compare( const bits& Query ) const noexcept;

        PPB_INLINE
        bool OneOf( const bits& Query ) const noexcept;

        PPB_INLINE
        bool NoneOf( const bits& Query ) const noexcept;

        PPB_INLINE
        bool Empty( void ) const noexcept;

        //-----------------------------------
        //            Generate
        //-----------------------------------
        PPB_INLINE
        const paperback::u64 GenerateGUID( void ) const noexcept;
    };
}