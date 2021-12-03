//// ----- Start of header guard
//#ifndef GJK_H
//#define GJK_H
//
//#include "Math/Math_includes.h"
//#include "geometry.h"
//
//#include <utility>
//#include <tuple>
//
//namespace VoronoiRegion
//{
//	enum Type {
//		Point0, Point1, Point2, Point3,
//		Edge01, Edge02, Edge03, Edge12, Edge13, Edge23,
//		Triangle012, Triangle013, Triangle023, Triangle123,
//		Tetrahedra0123,
//		Unknown
//	};
//	static const char* Names[] = { "Point0", "Point1", "Point2", "Point3",
//								  "Edge01", "Edge02", "Edge03", "Edge12", "Edge13", "Edge23",
//								  "Triangle012", "Triangle013", "Triangle023", "Triangle123",
//								  "Tetrahedra0123",
//								  "Unknown" };
//}
//
//struct SupportShape
//{
//	virtual paperback::Vector3f GetCenter() const = 0;
//	virtual paperback::Vector3f Support(const paperback::Vector3f& worldDirection) const = 0;
//
//	paperback::Vector3f GetCenter(const std::vector<paperback::Vector3f>& localPoints, const Matrix4& transform) const
//	{
//		paperback::Vector3f center = paperback::Vector3f{};
//		//
//
//		for (auto& pt : localPoints)
//			center += pt;
//		center /= (float)localPoints.size();
//
//		return TransformPoint(transform, center);
//	}
//
//	paperback::Vector3f Support(const paperback::Vector3f& worldDirection, const std::vector<paperback::Vector3f>& localPoints, const Matrix4& localToWorldTransform) const
//	{
//		paperback::Vector3f result = paperback::Vector3f{};
//		//
//
//		auto transformed = TransformNormal(localToWorldTransform.Inverted(), worldDirection);
//		float maximum = -Math::PositiveMax();
//		bool projected = false;
//		for (auto& pt : localPoints)
//		{
//			float projection = transformed.Dot(pt);
//			if (projection > maximum)
//			{
//				projected = true;
//				result = pt;
//				maximum = projection;
//			}
//		}
//		if (!projected)
//			return result;
//
//		return TransformPoint(localToWorldTransform, result);
//	}
//};
//
//struct PointsSupportShape : public SupportShape
//{
//	std::vector<paperback::Vector3f> mLocalSpacePoints;
//	paperback::Vector3f mScale;
//	Matrix3 mRotation;
//	paperback::Vector3f mTranslation;
//
//	PointsSupportShape()
//	{
//		mScale = paperback::Vector3f(1);
//		mRotation = Matrix3::cIdentity;
//		mTranslation = paperback::Vector3f::cZero;
//	}
//
//	paperback::Vector3f GetCenter() const
//	{
//		Matrix4 transform = Math::BuildTransform(mTranslation, mRotation, mScale);
//		return SupportShape::GetCenter(mLocalSpacePoints, transform);
//	}
//
//	paperback::Vector3f Support(const Vector3& worldDirection) const
//	{
//		Matrix4 transform = Math::BuildTransform(mTranslation, mRotation, mScale);
//		return SupportShape::Support(worldDirection, mLocalSpacePoints, transform);
//	}
//};
//
//struct SphereSupportShape : public SupportShape
//{
//	sphere mSphere;
//
//	paperback::Vector3f GetCenter() const
//	{
//		return mSphere.mCenter;
//	}
//
//	paperback::Vector3f Support(const Vector3& worldDirection) const
//	{
//		//
//		return GetCenter() + worldDirection.Normalized() * mSphere.mRadius;
//	}
//};
//
//struct ObbSupportShape : public SupportShape
//{
//	paperback::Vector3f mScale;
//	Matrix3 mRotation;
//	paperback::Vector3f mTranslation;
//
//	paperback::Vector3f ObbSupportShape::GetCenter() const
//	{
//		return mTranslation;
//	}
//
//	paperback::Vector3f ObbSupportShape::Support(const Vector3& worldDirection) const
//	{
//		//
//		Matrix4 transform;
//		transform.BuildTransform(mTranslation, mRotation, mScale);
//		transform.Invert();
//
//		auto local = TransformNormal(transform, worldDirection);
//		auto result = mTranslation;
//		for (int i = 0; i < 3; ++i)
//			result += Math::GetSign(local[i]) * (mScale[i] * 0.5f) * mRotation.Basis(i);
//
//		return result;
//	}
//};
//
////------------------------------------------------------------ Voronoi Region Tests
//struct GJK
//{
//	Gjk() {}
//
//	// contains all information for a point in Gjk
//	struct CsoPoint
//	{
//		paperback::Vector3f mPointA;
//		paperback::Vector3f mPointB;
//		paperback::Vector3f mCsoPoint;
//	};
//
//	// Point
//	VoronoiRegion::Type IdentifyVoronoiRegion(
//		const paperback::Vector3f& q, const paperback::Vector3f& p0,
//		size_t& newSize, int newIndices[4],
//		paperback::Vector3f& closestPoint, paperback::Vector3f& searchDirection)
//	{
//		newSize = 1;
//		newIndices[0] = 0;
//		closestPoint = p0;
//		searchDirection = q - closestPoint;
//		return VoronoiRegion::Point0;
//	}
//
//	// Edge Test
//	VoronoiRegion::Type IdentifyVoronoiRegion(
//		const paperback::Vector3f& q, const paperback::Vector3f& p0, const paperback::Vector3f& p1,
//		size_t& newSize, int newIndices[4],
//		paperback::Vector3f& closestPoint, paperback::Vector3f& searchDirection)
//	{
//		// Use barycentric
//		float u, v;
//		if (BarycentricCoordinates(q, p0, p1, u, v))
//		{
//			newSize = 2;
//			newIndices[0] = 0;
//			newIndices[1] = 1;
//			closestPoint = u * p0 + v * p1;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Edge01;
//		}
//
//		newSize = 1;
//
//		if (u >= 1.f)
//		{
//			newIndices[0] = 0;
//			closestPoint = p0;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Point0;
//		}
//
//		newIndices[0] = 1;
//		closestPoint = p1;
//		searchDirection = q - closestPoint;
//		return VoronoiRegion::Point1;
//	}
//
//	// Triangle Test
//	VoronoiRegion::Type IdentifyVoronoiRegion(
//		const paperback::Vector3f& q, const paperback::Vector3f& p0,
//		const paperback::Vector3f& p1, const paperback::Vector3f& p2,
//		size_t& newSize, int newIndices[4],
//		paperback::Vector3f& closestPoint, paperback::Vector3f& searchDirection)
//	{
//		// Point regions
//		newSize = 1;
//		std::pair<float, float> baryCoords[3];
//		BarycentricCoordinates(q, p0, p1, baryCoords[0].first, baryCoords[0].second);
//		BarycentricCoordinates(q, p1, p2, baryCoords[1].first, baryCoords[1].second);
//		BarycentricCoordinates(q, p2, p0, baryCoords[2].first, baryCoords[2].second);
//		if (baryCoords[2].first <= 0.f && baryCoords[0].second <= 0.f)
//		{
//			newIndices[0] = 0;
//			closestPoint = p0;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Point0;
//		}
//		if (baryCoords[0].first <= 0.f && baryCoords[1].second <= 0.f)
//		{
//			newIndices[0] = 1;
//			closestPoint = p1;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Point1;
//		}
//		if (baryCoords[1].first <= 0.f && baryCoords[2].second <= 0.f)
//		{
//			newIndices[0] = 2;
//			closestPoint = p2;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Point2;
//		}
//
//		// Line regions
//		newSize = 2;
//		float u, v, w;
//		BarycentricCoordinates(q, p0, p1, p2, u, v, w);
//		if (baryCoords[0].first > 0.f && baryCoords[0].second > 0.f && w <= 0.f)
//		{
//			newIndices[0] = 0;
//			newIndices[1] = 1;
//			closestPoint = baryCoords[0].first * p0 + baryCoords[0].second * p1;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Edge01;
//		}
//		if (baryCoords[1].first > 0.f && baryCoords[1].second > 0.f && u <= 0.f)
//		{
//			newIndices[0] = 1;
//			newIndices[1] = 2;
//			closestPoint = baryCoords[1].first * p1 + baryCoords[1].second * p2;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Edge12;
//		}
//		if (baryCoords[2].first > 0.f && baryCoords[2].second > 0.f && v <= 0.f)
//		{
//			newIndices[0] = 0;
//			newIndices[1] = 2;
//			closestPoint = baryCoords[2].first * p2 + baryCoords[2].second * p0;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Edge02;
//		}
//
//		// Triangle
//		newSize = 3;
//		for (int i = 0; i < 3; ++i)
//			newIndices[i] = i;
//		closestPoint = u * p0 + v * p1 + w * p2;
//		searchDirection = q - closestPoint;
//		return VoronoiRegion::Triangle012;
//	}
//
//	// Tetrahedron
//	VoronoiRegion::Type IdentifyVoronoiRegion(
//		const paperback::Vector3f& q, const paperback::Vector3f& p0,
//		const paperback::Vector3f& p1, const paperback::Vector3f& p2,
//		const paperback::Vector3f& p3,
//		size_t& newSize, int newIndices[4],
//		paperback::Vector3f& closestPoint, paperback::Vector3f& searchDirection)
//	{
//		// Point regions
//		newSize = 1;
//		std::pair<float, float> baryCoords[6];
//		BarycentricCoordinates(q, p0, p1, baryCoords[0].first, baryCoords[0].second);
//		BarycentricCoordinates(q, p0, p2, baryCoords[1].first, baryCoords[1].second);
//		BarycentricCoordinates(q, p3, p0, baryCoords[2].first, baryCoords[2].second);
//		BarycentricCoordinates(q, p2, p1, baryCoords[3].first, baryCoords[3].second);
//		BarycentricCoordinates(q, p1, p3, baryCoords[4].first, baryCoords[4].second);
//		BarycentricCoordinates(q, p2, p3, baryCoords[5].first, baryCoords[5].second);
//		if (baryCoords[2].first <= 0.f && baryCoords[0].second <= 0.f && baryCoords[1].second <= 0.f)
//		{
//			newIndices[0] = 0;
//			closestPoint = p0;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Point0;
//		}
//		if (baryCoords[0].first <= 0.f && baryCoords[3].first <= 0.f && baryCoords[4].second <= 0.f)
//		{
//			newIndices[0] = 1;
//			closestPoint = p1;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Point1;
//		}
//		if (baryCoords[1].first <= 0.f && baryCoords[3].second <= 0.f && baryCoords[5].second <= 0.f)
//		{
//			newIndices[0] = 2;
//			closestPoint = p2;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Point2;
//		}
//		if (baryCoords[4].first <= 0.f && baryCoords[5].first <= 0.f && baryCoords[2].second <= 0.f)
//		{
//			newIndices[0] = 3;
//			closestPoint = p3;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Point3;
//		}
//
//		// Line regions
//		newSize = 2;
//		std::tuple<float, float, float> baryTuple[4];
//		BarycentricCoordinates(q, p0, p1, p2, std::get<0>(baryTuple[0]), std::get<1>(baryTuple[0]), std::get<2>(baryTuple[0]));
//		BarycentricCoordinates(q, p0, p1, p3, std::get<0>(baryTuple[1]), std::get<1>(baryTuple[1]), std::get<2>(baryTuple[1]));
//		BarycentricCoordinates(q, p2, p0, p3, std::get<0>(baryTuple[2]), std::get<1>(baryTuple[2]), std::get<2>(baryTuple[2]));
//		BarycentricCoordinates(q, p1, p2, p3, std::get<0>(baryTuple[3]), std::get<1>(baryTuple[3]), std::get<2>(baryTuple[3]));
//		if (baryCoords[0].first > 0.f && baryCoords[0].second > 0.f && std::get<2>(baryTuple[0]) <= 0.f && std::get<2>(baryTuple[1]) <= 0.f)
//		{
//			newIndices[0] = 0;
//			newIndices[1] = 1;
//			closestPoint = baryCoords[0].first * p0 + baryCoords[0].second * p1;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Edge01;
//		}
//		if (baryCoords[1].first > 0.f && baryCoords[1].second > 0.f && std::get<1>(baryTuple[0]) <= 0.f && std::get<2>(baryTuple[2]) <= 0.f)
//		{
//			newIndices[0] = 0;
//			newIndices[1] = 2;
//			closestPoint = baryCoords[1].first * p0 + baryCoords[1].second * p2;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Edge02;
//		}
//		if (baryCoords[3].first > 0.f && baryCoords[3].second > 0.f && std::get<0>(baryTuple[0]) <= 0.f && std::get<2>(baryTuple[3]) <= 0.f)
//		{
//			newIndices[0] = 1;
//			newIndices[1] = 2;
//			closestPoint = baryCoords[3].first * p2 + baryCoords[3].second * p1;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Edge12;
//		}
//		if (baryCoords[2].first > 0.f && baryCoords[2].second > 0.f && std::get<1>(baryTuple[1]) <= 0.f && std::get<0>(baryTuple[2]) <= 0.f)
//		{
//			newIndices[0] = 0;
//			newIndices[1] = 3;
//			closestPoint = baryCoords[2].first * p3 + baryCoords[2].second * p0;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Edge03;
//		}
//		if (baryCoords[4].first > 0.f && baryCoords[4].second > 0.f && std::get<0>(baryTuple[1]) <= 0.f && std::get<1>(baryTuple[3]) <= 0.f)
//		{
//			newIndices[0] = 1;
//			newIndices[1] = 3;
//			closestPoint = baryCoords[4].first * p1 + baryCoords[4].second * p3;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Edge13;
//		}
//		if (baryCoords[5].first > 0.f && baryCoords[5].second > 0.f && std::get<1>(baryTuple[2]) <= 0.f && std::get<0>(baryTuple[3]) <= 0.f)
//		{
//			newIndices[0] = 2;
//			newIndices[1] = 3;
//			closestPoint = baryCoords[5].first * p2 + baryCoords[5].second * p3;
//			searchDirection = q - closestPoint;
//			return VoronoiRegion::Edge23;
//		}
//
//		// Triangle regions
//		newSize = 3;
//		if (std::get<0>(baryTuple[0]) >= 0.f && std::get<1>(baryTuple[0]) >= 0.f && std::get<2>(baryTuple[0]) >= 0.f)
//		{
//			auto n = (p1 - p0).Cross(p2 - p0);
//			if (n.Dot(p3 - p0) > 0.f)
//				n *= -1;
//			if (n.Dot(q - p0) >= 0.f)
//			{
//				newIndices[0] = 0;
//				newIndices[1] = 1;
//				newIndices[2] = 2;
//				closestPoint = std::get<0>(baryTuple[0]) * p0 + std::get<1>(baryTuple[0]) * p1 + std::get<2>(baryTuple[0]) * p2;
//				searchDirection = q - closestPoint;
//				return VoronoiRegion::Triangle012;
//			}
//		}
//		if (std::get<0>(baryTuple[1]) >= 0.f && std::get<1>(baryTuple[1]) >= 0.f && std::get<2>(baryTuple[1]) >= 0.f)
//		{
//			auto n = (p1 - p0).Cross(p3 - p0);
//			if (n.Dot(p2 - p0) > 0.f)
//				n *= -1;
//			if (n.Dot(q - p0) >= 0.f)
//			{
//				newIndices[0] = 0;
//				newIndices[1] = 1;
//				newIndices[2] = 3;
//				closestPoint = std::get<0>(baryTuple[1]) * p0 + std::get<1>(baryTuple[1]) * p1 + std::get<2>(baryTuple[1]) * p3;
//				searchDirection = q - closestPoint;
//				return VoronoiRegion::Triangle013;
//			}
//		}
//		if (std::get<0>(baryTuple[2]) >= 0.f && std::get<1>(baryTuple[2]) >= 0.f && std::get<2>(baryTuple[2]) >= 0.f)
//		{
//			auto n = (p2 - p0).Cross(p3 - p0);
//			if (n.Dot(p1 - p0) > 0.f)
//				n *= -1;
//			if (n.Dot(q - p0) >= 0.f)
//			{
//				newIndices[0] = 0;
//				newIndices[1] = 2;
//				newIndices[2] = 3;
//				closestPoint = std::get<0>(baryTuple[2]) * p2 + std::get<1>(baryTuple[2]) * p0 + std::get<2>(baryTuple[2]) * p3;
//				searchDirection = q - closestPoint;
//				return VoronoiRegion::Triangle023;
//			}
//		}
//		if (std::get<0>(baryTuple[3]) >= 0.f && std::get<1>(baryTuple[3]) >= 0.f && std::get<2>(baryTuple[3]) >= 0.f)
//		{
//			auto n = (p2 - p1).Cross(p3 - p1);
//			if (n.Dot(p0 - p1) > 0.f)
//				n *= -1;
//			if (n.Dot(q - p1) >= 0.f)
//			{
//				newIndices[0] = 1;
//				newIndices[1] = 2;
//				newIndices[2] = 3;
//				closestPoint = std::get<0>(baryTuple[3]) * p1 + std::get<1>(baryTuple[3]) * p2 + std::get<2>(baryTuple[3]) * p3;
//				searchDirection = q - closestPoint;
//				return VoronoiRegion::Triangle123;
//			}
//		}
//
//		// Tetrahedron
//		newSize = 4;
//		newIndices[0] = 0;
//		newIndices[1] = 1;
//		newIndices[2] = 2;
//		newIndices[3] = 3;
//		closestPoint = q;
//		searchDirection = paperback::Vector3f{};
//		return VoronoiRegion::Tetrahedra0123;
//	}
//
//
//	// Returns true if the shapes intersect. If the shapes don't intersect then closestPoint is filled out with the closest points
//	// on each object as well as the cso point. Epsilon should be used for checking if sufficient progress has been made at any step.
//	bool Intersect(
//		const SupportShape* shapeA, const SupportShape* shapeB,
//		unsigned int maxIterations, CsoPoint& closestPoint,
//		float epsilon, int debuggingIndex, bool debugDraw)
//	{
//		// Set search direction to the same as assignment brief (difference of centers, if zero, set to [-1,0,0])
//		auto searchDir = shapeA->GetCenter() - shapeB->GetCenter();
//		if (searchDir == paperback::Vector3f{})
//			searchDir = paperback::Vector3f{ -1.f, 0.f, 0.f };
//
//		// To identify voronoi regions
//		size_t size = 1;
//		int indices[4] = { 0, 1, 2, 3 };
//		paperback::Vector3f closestPt = paperback::Vector3f{};
//		CsoPoint supports[4];
//		supports[0] = ComputeSupport(shapeA, shapeB, searchDir);
//
//		// Loop for max iterations
//		for (unsigned i = 0; i < maxIterations; ++i)
//		{
//			switch (size)
//			{
//			case 1:
//				IdentifyVoronoiRegion(paperback::Vector3f{}, supports[indices[0]].mCsoPoint, size, indices, closestPt, searchDir);
//				break;
//			case 2:
//				IdentifyVoronoiRegion(paperback::Vector3f{}, supports[indices[0]].mCsoPoint, supports[indices[1]].mCsoPoint, size, indices, closestPt, searchDir);
//				break;
//			case 3:
//				IdentifyVoronoiRegion(paperback::Vector3f{}, supports[indices[0]].mCsoPoint, supports[indices[1]].mCsoPoint, supports[indices[2]].mCsoPoint, size, indices, closestPt, searchDir);
//				break;
//			default:
//				IdentifyVoronoiRegion(paperback::Vector3f{}, supports[indices[0]].mCsoPoint, supports[indices[1]].mCsoPoint, supports[indices[2]].mCsoPoint, supports[indices[3]].mCsoPoint, size, indices, closestPt, searchDir);
//				break;
//			}
//			if (closestPt == paperback::Vector3f{})
//				return true;
//
//			// Update for next iteration
//			for (int j = 0; j < 4; ++j)
//			{
//				supports[j] = supports[indices[j]];
//				indices[j] = j;
//			}
//			searchDir = -closestPt.Normalized();
//			supports[size] = ComputeSupport(shapeA, shapeB, searchDir);
//			if (searchDir.Dot(supports[size].mCsoPoint - closestPt) <= epsilon)
//				break;
//			if (i < maxIterations - 1)
//				++size;
//		}
//
//		// Max iterations or break condition reached (no intersection), update closest point info
//		switch (size)
//		{
//		case 1:
//			closestPoint = supports[indices[0]];
//			break;
//		case 2:
//		{
//			float u, v;
//			BarycentricCoordinates(closestPt, supports[indices[0]].mCsoPoint, supports[indices[1]].mCsoPoint, u, v);
//
//			closestPoint.mPointA = u * supports[indices[0]].mPointA + v * supports[indices[1]].mPointA;
//			closestPoint.mPointB = u * supports[indices[0]].mPointB + v * supports[indices[1]].mPointB;
//			closestPoint.mCsoPoint = closestPt;
//
//			break;
//		}
//		case 3:
//		{
//			float u, v, w;
//			BarycentricCoordinates(closestPt, supports[indices[0]].mCsoPoint, supports[indices[1]].mCsoPoint, supports[indices[2]].mCsoPoint, u, v, w);
//
//			closestPoint.mPointA = u * supports[indices[0]].mPointA + v * supports[indices[1]].mPointA + w * supports[indices[2]].mPointA;
//			closestPoint.mPointB = u * supports[indices[0]].mPointB + v * supports[indices[1]].mPointB + w * supports[indices[2]].mPointB;
//			closestPoint.mCsoPoint = closestPt;
//
//			break;
//		}
//		}
//
//		return false;
//	}
//
//	CsoPoint ComputeSupport(
//		const SupportShape* shapeA, const SupportShape* shapeB, const paperback::Vector3f& direction)
//	{
//		CsoPoint result;
//
//		result.mPointA = shapeA->Support(direction);
//		result.mPointB = shapeB->Support(-direction);
//		result.mCsoPoint = result.mPointA - result.mPointB;
//
//		return result;
//	}
//};
//
//// ----- End of header guard
//#endif