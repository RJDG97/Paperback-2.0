#pragma once
#ifndef BOUNDIINGBOX_H
#define BOUNDIINGBOX_H

#include "Math/Vector3f.h"

struct boundingbox
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "BoundingBox"
	};

	paperback::Vector3f Min, Max;
	bool m_Collided;
	std::unordered_map<uint32_t, bool> m_CollisionState;

	// used in debugDraw
	boundingbox( paperback::Vector3f minimum = paperback::Vector3f{},
		         paperback::Vector3f maximum = paperback::Vector3f{},
		         bool collide = false) :
		Min{ minimum },
		Max{ maximum },
		m_Collided{ collide }
	{ }

	float Volume( void ) noexcept
	{
		return ( std::abs( Max.x ) * std::abs( Min.x ) ) + 
			   ( std::abs( Max.y ) * std::abs( Min.y ) ) +
			   ( std::abs( Max.z ) * std::abs( Min.z ) );
	}

	paperback::Vector3f ComputeCentre( void ) noexcept
	{
		return paperback::Vector3f{ ( Min.x + Max.x ) * 0.5f
								  , ( Min.y + Max.y ) * 0.5f
								  , ( Min.z + Max.z ) * 0.5f };
	}

	paperback::Vector3f Merge( const boundingbox& lhs
		                     , const boundingbox& rhs ) noexcept
	{
		Min.x = std::min( lhs.Min.x, rhs.Min.x );
		Min.y = std::min( lhs.Min.y, rhs.Min.y );
		Min.z = std::min( lhs.Min.z, rhs.Min.z );

		Max.x = std::max( lhs.Max.x, rhs.Max.x );
		Max.y = std::max( lhs.Max.y, rhs.Max.y );
		Max.z = std::max( lhs.Max.z, rhs.Max.z );

		return ComputeCentre( );
	}

	paperback::Vector3f Merge( const boundingbox& rhs ) noexcept
	{
		Min.x = std::min( Min.x, rhs.Min.x );
		Min.y = std::min( Min.y, rhs.Min.y );
		Min.z = std::min( Min.z, rhs.Min.z );

		Max.x = std::max( Max.x, rhs.Max.x );
		Max.y = std::max( Max.y, rhs.Max.y );
		Max.z = std::max( Max.z, rhs.Max.z );

		return ComputeCentre( );
	}

	boundingbox Extend( paperback::Vector3f Position
					  , float SkinThickness ) const noexcept
	{
		return Extend( Position
				     , paperback::Vector3f{ SkinThickness, SkinThickness, SkinThickness } );
	}

	boundingbox Extend( paperback::Vector3f Position
					  , paperback::Vector3f SkinThickness ) const noexcept
	{
		return boundingbox{ paperback::Vector3f{ Position.x + ( Min.x - std::abs( SkinThickness.x ) )
					                           , Position.y + ( Min.y - std::abs( SkinThickness.y ) )
			                                   , Position.z + ( Min.z - std::abs( SkinThickness.z ) ) }
			       	      , paperback::Vector3f{ Position.x + ( Max.x + std::abs( SkinThickness.x ) )
			                                   , Position.y + ( Max.y + std::abs( SkinThickness.y ) )
			                                   , Position.z + ( Max.z + std::abs( SkinThickness.z ) ) } };
	}

	// Ensure RHS's Values Are Global Coordinates, Not Local Scaling Factor
	bool Contains( const boundingbox& rhs ) const noexcept
	{
		if ( Min.x <= rhs.Min.x && Max.x >= rhs.Max.x &&
		     Min.y <= rhs.Min.y && Max.y >= rhs.Max.y &&
		     Min.z <= rhs.Min.z && Max.z >= rhs.Max.z )
		{
			return true;
		}
		return false;
	}

	bool Contains( const paperback::Vector3f& Position
			     , const boundingbox& rhs ) const noexcept
	{
		if ( Min.x <= ( Position.x + rhs.Min.x ) && Max.x >= ( Position.x + rhs.Max.x ) &&
		     Min.y <= ( Position.y + rhs.Min.y ) && Max.y >= ( Position.y + rhs.Max.y ) &&
		     Min.z <= ( Position.z + rhs.Min.z ) && Max.z >= ( Position.z + rhs.Max.z ) )
		{
			return true;
		}
		return false;
	}

	// Ensure RHS's Values Are Global Coordinates, Not Local Scaling Factor
	bool Intersecting( const boundingbox& rhs ) const noexcept
	{
		return !( Min.x > rhs.Max.x || rhs.Min.x > Max.x
			   || Min.y > rhs.Max.y || rhs.Min.y > Max.y
			   || Min.z > rhs.Max.z || rhs.Min.z > Max.z );
	}
};


namespace RR_BOUNDINGBOX
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<boundingbox>(boundingbox::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
		   .property("Min Point", &boundingbox::Min)(rttr::policy::prop::as_reference_wrapper)
		   .property("Max Point", &boundingbox::Max)(rttr::policy::prop::as_reference_wrapper)
		   .property("Is Collide", &boundingbox::m_Collided);
	}
}

#endif