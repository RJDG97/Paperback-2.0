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