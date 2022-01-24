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