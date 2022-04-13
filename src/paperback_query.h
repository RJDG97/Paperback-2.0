/**********************************************************************************
*\file         paperback_query.h
*\brief        paperback_query.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

namespace paperback::query
{
    template < typename... T_COMPONENTS >
    struct must
    {
        using type = std::tuple< T_COMPONENTS... >;;
    };

    template < typename... T_COMPONENTS >
    struct one_of
    {
        using type = std::tuple< T_COMPONENTS... >;;
    };

    template < typename... T_COMPONENTS >
    struct none_of
    {
        using type = std::tuple< T_COMPONENTS... >;;
    };
}

namespace tools
{
    struct query
    {
        bits    m_Must;
        bits    m_OneOf;
        bits    m_NoneOf;

        PPB_INLINE
        bool Compare( const bits& ArchetypeBits ) const noexcept;

        template< typename T_FUNCTION >
        void AddQueryFromFunction( void ) noexcept;

        template<typename... T_QUERIES >
        void AddQueryFromTuple( std::tuple<T_QUERIES...>* ) noexcept;
    };
}