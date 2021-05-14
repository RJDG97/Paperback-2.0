#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Paperback/IComponent.h"
#include "Math/Vector.h"

namespace Paperback
{
	class PAPERBACK_API Transform : public IComponent
	{
	public:

		Transform(const vec3_f& _scale    = { 0.0f, 0.0f, 0.0f }, 
				  const vec3_f& _offset   = { 0.0f, 0.0f, 0.0f },
				  const vec3_f& _position = { 0.0f, 0.0f, 0.0f }, 
				  const vec3_f& _rotation = { 0.0f, 0.0f, 0.0f }
				 );
		~Transform();

		void Init() override;

		void SetScale(const vec3_f& vec);
		const vec3_f& GetScale() const;
		void SetOffset(const vec3_f& vec);
		const vec3_f& GetOffset() const;
		void SetPosition(const vec3_f& vec);
		const vec3_f& GetPosition() const;
		void SetRotation(const vec3_f& vec);
		const vec3_f& GetRotation() const;

		//RTTR_ENABLE(IComponent);
		//RTTR_REGISTRATION_FRIEND

	private:

	#pragma warning(disable : 4251)
		vec3_f scale;
		vec3_f offset;
		vec3_f position;
		vec3_f rotation;
	#pragma warning(default : 4251)
	};
}

#endif // !TRANSFORM_H