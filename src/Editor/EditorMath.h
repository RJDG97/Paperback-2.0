#pragma once
#include "glm/inc/gtx/matrix_decompose.hpp"

namespace Editor::Math
{
	void GlmtoVec3(paperback::Vector3f& Vec3f, glm::vec3& Vec3)
	{
		Vec3f.x = Vec3.x;
		Vec3f.y = Vec3.y;
		Vec3f.z = Vec3.z;
	}

	bool DecomposeTransform(const glm::mat4& Transform, glm::vec3& Translate, glm::vec3& Scale, glm::vec3& Rotation)
	{
		//using namespace glm;
		using T = float;

		glm::mat4 LocalMatrix(Transform);

		// Normalize the matrix.
		if (glm::epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), glm::epsilon<T>()))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (
			glm::epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), glm::epsilon<T>()) ||
			glm::epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), glm::epsilon<T>()) ||
			glm::epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), glm::epsilon<T>()))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of translation (easy).
		Translate = glm::vec3(LocalMatrix[3]);
		LocalMatrix[3] = glm::vec4(0, 0, 0, LocalMatrix[3].w);

		glm::vec3 Row[3], Pdum3;

		// Now get scale and shear.
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		Scale.x = glm::length(Row[0]);
		Row[0] = glm::detail::scale(Row[0], static_cast<T>(1));
		Scale.y = glm::length(Row[1]);
		Row[1] = glm::detail::scale(Row[1], static_cast<T>(1));
		Scale.z = glm::length(Row[2]);
		Row[2] = glm::detail::scale(Row[2], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
#if 0
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

		Rotation.y = asin(-Row[0][2]);
		if (cos(Rotation.y) != 0) {
			Rotation.x = atan2(Row[1][2], Row[2][2]);
			Rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			Rotation.x = atan2(-Row[2][0], Row[1][1]);
			Rotation.z = 0;
		}


		return true;
	}

}
