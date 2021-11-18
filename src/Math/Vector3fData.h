#ifndef VECTOR3_DATA_H
#define VECTOR3_DATA_H

namespace paperback
{
//----- Ignore this page, refer only when required

//----- V3 Directions
#define		V3_RIGHT		Vector3f(1.f, 0.f, 0.f)
#define		V3_LEFT			Vector3f(-1.f, 0.f, 0.f)

#define		V3_FORWARD		Vector3f(0.f, 1.f, 0.f)
#define		V3_BACKWARD		Vector3f(0.f, -1.f, 0.f)

#define		V3_UP			Vector3f(0.f, 0.f, 1.f)
#define		V3_DOWN			Vector3f(0.f, 0.f, -1.f)

//----- V4 Directions
#define		V4_RIGHT		Vector4f(1.f, 0.f, 0.f, 0.f)
#define		V4_LEFT			Vector4f(-1.f, 0.f, 0.f, 0.f)

#define		V4_FORWARD		Vector4f(0.f, 1.f, 0.f, 0.f)
#define		V4_BACKWARD		Vector4f(0.f, -1.f, 0.f, 0.f)

#define		V4_UP			Vector4f(0.f, 0.f, 1.f, 0.f)
#define		V4_DOWN			Vector4f(0.f, 0.f, -1.f, 0.f)


//----- One & Zero
#define		V2_ONE			Vector3f(1.f, 1.f, 0.f)
#define		V2_ZERO			Vector3f(0.f, 0.f, 0.f)

#define		V3_ONE			Vector3f(1.f, 1.f, 1.f)
#define		V3_ZERO			Vector3f(0.f, 0.f, 0.f)

#define		V4_ONE			Vector4f(1.f, 1.f, 1.f, 1.f)
#define		V4_ZERO			Vector4f(0.f, 0.f, 0.f, 0.f)


//-----  R, G, B, B = 0, W = 1
}


//const Vector4f Vector4f::cZero(0.0f, 0.0f, 0.0f, 0.0f);
//const Vector4f Vector4f::cXAxis(1.0f, 0.0f, 0.0f, 0.0f);
//const Vector4f Vector4f::cYAxis(0.0f, 1.0f, 0.0f, 0.0f);
//const Vector4f Vector4f::cZAxis(0.0f, 0.0f, 1.0f, 0.0f);
//const Vector4f Vector4f::cWAxis(0.0f, 0.0f, 0.0f, 1.0f);

#endif
