#include "PaperbackPCH.h"
#include "Transform.h"

namespace Paperback
{
	Transform::Transform(const vec3_f& _scale,
						 const vec3_f& _offset,
						 const vec3_f& _position,
						 const vec3_f& _rotation) :
		scale{ _scale },
		offset{ _offset },
		position{ _position },
		rotation{ _rotation }
	{ }

	Transform::~Transform()
	{
		
	}

	void Transform::Init()
	{
		
	}

	void Transform::SetScale(const vec3_f& vec)
	{
		scale = vec;
	}

	const vec3_f& Transform::GetScale() const
	{
		return scale;
	}

	void Transform::SetOffset(const vec3_f& vec)
	{
		offset = vec;
	}

	const vec3_f& Transform::GetOffset() const
	{
		return offset;
	}

	void Transform::SetPosition(const vec3_f& vec)
	{
		position = vec;
	}

	const vec3_f& Transform::GetPosition() const
	{
		return position;
	}

	void Transform::SetRotation(const vec3_f& vec)
	{
		rotation = vec;
	}

	const vec3_f& Transform::GetRotation() const
	{
		return rotation;
	}

	//RTTR_REGISTRATION
	//{
	//	using namespace rttr;

	//	registration::class_<Transform>("Engine::Component::Transform")
	//		.constructor<const vec3_f&, const vec3_f&, const vec3_f&, const vec3_f&>() (policy::ctor::as_std_shared_ptr)
	//		.property("scale", &Transform::GetScale, &Transform::SetScale)
	//		//.property("scale", &Transform::scale, registration::private_access)
	//		.property("scale", &Transform::GetOffset, &Transform::SetOffset)
	//		//.property("offset", &Transform::offset, registration::private_access)
	//		.property("scale", &Transform::GetPosition, &Transform::SetPosition)
	//		//.property("position", &Transform::position, registration::private_access)
	//		.property("scale", &Transform::GetRotation, &Transform::SetRotation);
	//		//.property("rotation", &Transform::rotation, registration::private_access);
	//}
}