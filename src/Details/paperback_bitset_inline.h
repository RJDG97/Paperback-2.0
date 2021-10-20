#pragma once

namespace tools
{
    //-----------------------------------
    //              Add
    //-----------------------------------
    void bits::Set( const int Bit ) noexcept
    {
		int x = Bit / 64;
		int y = Bit % 64;
		m_bits[x] |= (1ull << y);
    }

    void bits::Remove( const int Bit ) noexcept
    {
        int x = Bit / 64;
        int y = Bit % 64;
        m_bits[x] &= ~(1ull << y);
    }

    template < typename... T_COMPONENTS >
    void bits::AddFromComponents( void )
    {
        ( (Set( paperback::component::info_v<T_COMPONENTS>.m_UID)), ... );
    }

    //-----------------------------------
    //            Compare
    //-----------------------------------
    bool bits::Has( const int Bit ) const noexcept
    {
		int x = Bit / 64;
		int y = Bit % 64;
		
		return !!( m_bits[x] & (1ull << y) );
    }

    template < typename... T_COMPONENTS >
    bool bits::Has(void) const noexcept
    {
        bits Check{};
        Check.AddFromComponents<T_COMPONENTS...>();

        return Check.Compare( *this );
    }

    bool bits::None( const int Bit ) const noexcept
    {
		int x = Bit / 64;
		int y = Bit % 64;
		
		return !(m_bits[x] & (1ull << y));
    }

    bool bits::Match( const bits& Query ) const noexcept
    {
		for (size_t i = 0, size = m_bits.size(); i < size; ++i)
        {
            if (m_bits[i] != Query.m_bits[i])
                return false;
        }

        return true;
    }

    bool bits::Compare( const bits& Query ) const noexcept
    {
		for (size_t i = 0, size = m_bits.size(); i < size; ++i)
        {
            if ( (m_bits[i] & Query.m_bits[i]) != Query.m_bits[i] )
                return false;
        }

        return true;
    }

    const paperback::u64 bits::GenerateGUID( void ) const noexcept
    {
        paperback::u64 HashValue;
        std::hash<paperback::u64> Hasher;

        for ( size_t i = 0, max = m_bits.size(); i < max; ++i )
        {
            HashValue ^= Hasher( m_bits[i] ) + ( HashValue << 3 ) + ( HashValue >> 1 );
        }

        return HashValue;
    }
}