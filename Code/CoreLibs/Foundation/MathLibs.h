#pragma once
#include "Prerequisites.h"

#include "DirectXMath.h"
#include "DirectXPackedVector.h"
#include "DirectXCollision.h"

using Vec2i = DirectX::XMINT2;
using Vec3i = DirectX::XMINT3;
using Vec4i = DirectX::XMINT4;

using Vec2 = DirectX::XMFLOAT2;
static constexpr Vec2 ZERO_VEC2 = DirectX::XMFLOAT2(0, 0);
static constexpr Vec2 ONE_VEC2 = DirectX::XMFLOAT2(1, 1);
using Vec3 = DirectX::XMFLOAT3;
static constexpr Vec3 ZERO_VEC3 = DirectX::XMFLOAT3(0, 0, 0);
static constexpr Vec3 ONE_VEC3 = DirectX::XMFLOAT3(1, 1, 1);
static constexpr Vec3 MAX_VEC3 = DirectX::XMFLOAT3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
using Vec4 = DirectX::XMFLOAT4;
static constexpr Vec4 ZERO_VEC4 = DirectX::XMFLOAT4(0, 0, 0, 0);
static constexpr Vec4 ONE_VEC4 = DirectX::XMFLOAT4(1, 1, 1, 1);
static constexpr Vec4 IDENTITY_QUAT = DirectX::XMFLOAT4(0, 0, 0, 1);

// 2D Vector; 16 bit floating point components
using Vec2h = DirectX::PackedVector::XMHALF2;
// 4D Vector; 16 bit floating point components
using Vec4h = DirectX::PackedVector::XMHALF4;

using Mat33 = DirectX::XMFLOAT3X3;
using Mat34 = DirectX::XMFLOAT3X4;
using Mat43 = DirectX::XMFLOAT4X3;
using Mat44 = DirectX::XMFLOAT4X4;
static constexpr Mat44 IDENTITY_MAT44 = Mat44(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

using Matrix = DirectX::XMMATRIX;
using Vector = DirectX::XMVECTOR;

using namespace DirectX;
using namespace DirectX::PackedVector;

RB_NAMESPACE_BEGIN

class RB_CORE_API RMath
{
public:
	static constexpr uint32 GetNextPowerOfTwo(uint32 x)
	{
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		return ++x;
	}

	static constexpr uint64 GetNextPowerOfTwo(uint64 x)
	{
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		x |= x >> 32u;
		return ++x;
	}

	static int FloorToInt(float val)
	{
		assert(val >= std::numeric_limits<int32_t>::min() && val <= std::numeric_limits<int32_t>::max());
		// Negative values need offset in order to truncate towards negative infinity (cast truncates towards zero)
		return val >= 0.0f ? (int32_t)val : (int32_t)(val - BIGGEST_FLOAT_SMALLER_THAN_ONE);
	}

	static int32_t RoundToInt(float val) { return FloorToInt(val + 0.5f); }

	static float Distance(const Vector& v1, const Vector& v2)
	{
		Vector vectorSub = XMVectorSubtract(v1, v2);
		Vector length = XMVector3Length(vectorSub);

		float d = 0.0f;
		XMStoreFloat(&d, length);
		return d;
	}

	static float DistanceSquared(const Vector& v1, const Vector& v2)
	{
		Vector vectorSub = XMVectorSubtract(v1, v2);
		Vector length = XMVector3LengthSq(vectorSub);

		float d = 0.0f;
		XMStoreFloat(&d, length);
		return d;
	}

	static float DistanceEstimated(const Vector& v1, const Vector& v2)
	{
		Vector vectorSub = XMVectorSubtract(v1, v2);
		Vector length = XMVector3LengthEst(vectorSub);

		float d = 0.0f;
		XMStoreFloat(&d, length);
		return d;
	}

	static float Distance(const Vec2& v1, const Vec2& v2)
	{
		Vector vector1 = XMLoadFloat2(&v1);
		Vector vector2 = XMLoadFloat2(&v2);
		return XMVectorGetX(XMVector2Length(vector2 - vector1));
	}

	static float Distance(const Vec3& v1, const Vec3& v2)
	{
		Vector vector1 = XMLoadFloat3(&v1);
		Vector vector2 = XMLoadFloat3(&v2);
		return Distance(vector1, vector2);
	}

	static float DistanceSquared(const Vec3& v1, const Vec3& v2)
	{
		Vector vector1 = XMLoadFloat3(&v1);
		Vector vector2 = XMLoadFloat3(&v2);
		return DistanceSquared(vector1, vector2);
	}

	static float DistanceEstimated(const Vec3& v1, const Vec3& v2)
	{
		Vector vector1 = XMLoadFloat3(&v1);
		Vector vector2 = XMLoadFloat3(&v2);
		return DistanceEstimated(vector1, vector2);
	}

	static constexpr float Lerp(float value1, float value2, float amount)
	{
		return value1 + (value2 - value1) * amount;
	}

	static constexpr Vec2 Lerp(const Vec2& a, const Vec2& b, float i)
	{
		return Vec2(Lerp(a.x, b.x, i), Lerp(a.y, b.y, i));
	}

	static constexpr Vec3 Lerp(const Vec3& a, const Vec3& b, float i)
	{
		return Vec3(Lerp(a.x, b.x, i), Lerp(a.y, b.y, i), Lerp(a.z, b.z, i));
	}

	static constexpr Vec4 Lerp(const Vec4& a, const Vec4& b, float i)
	{
		return Vec4(Lerp(a.x, b.x, i), Lerp(a.y, b.y, i), Lerp(a.z, b.z, i), Lerp(a.w, b.w, i));
	}

	static constexpr Vec3 Max(const Vec3& a, const Vec3& b)
	{
		return Vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
	}

	static constexpr Vec3 Min(const Vec3& a, const Vec3& b)
	{
		return Vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
	}

	template< typename _Type >
	static constexpr _Type Min(_Type a, _Type b)
	{
		return std::min< _Type>(a, b);
	}

	template< typename _Type >
	static constexpr _Type Max(_Type a, _Type b)
	{
		return std::max<_Type>(a, b);
	}

	template< typename _Type >
	static constexpr _Type Abs(_Type a)
	{
		return std::abs(a);
	}

	static constexpr float Saturate(float x) { return Min(Max(x, 0.0f), 1.0f); }

	static uint32 CompressNormal(const Vec3& normal)
	{
		uint32 retval = 0;

		retval |= (uint32)((uint8)(normal.x * 127.5f + 127.5f) << 0);
		retval |= (uint32)((uint8)(normal.y * 127.5f + 127.5f) << 8);
		retval |= (uint32)((uint8)(normal.z * 127.5f + 127.5f) << 16);

		return retval;
	}

	static uint32 CompressColor(const Vec3& color)
	{
		uint32 retval = 0;

		retval |= (uint32)((uint8)(Saturate(color.x) * 255.0f) << 0);
		retval |= (uint32)((uint8)(Saturate(color.y) * 255.0f) << 8);
		retval |= (uint32)((uint8)(Saturate(color.z) * 255.0f) << 16);

		return retval;
	}

	static uint32 CompressColor(const Vec4& color)
	{
		uint32 retval = 0;

		retval |= (uint32)((uint8)(Saturate(color.x) * 255.0f) << 0);
		retval |= (uint32)((uint8)(Saturate(color.y) * 255.0f) << 8);
		retval |= (uint32)((uint8)(Saturate(color.z) * 255.0f) << 16);
		retval |= (uint32)((uint8)(Saturate(color.w) * 255.0f) << 24);

		return retval;
	}

	static Vec3 Unpack_R11G11B10_FLOAT(uint32 value)
	{
		XMFLOAT3PK pk;
		pk.v = value;
		Vector V = XMLoadFloat3PK(&pk);
		Vec3 result;
		XMStoreFloat3(&result, V);
		return result;
	}

	static uint32 Pack_R11G11B10_FLOAT(const Vec3& color)
	{
		XMFLOAT3PK pk;
		XMStoreFloat3PK(&pk, XMLoadFloat3(&color));
		return pk.v;
	}

	static float BIGGEST_FLOAT_SMALLER_THAN_ONE;
	static float POS_INFINITY;
	static float NEG_INFINITY;
	static float PI;
	static float TWO_PI;
	static float HALF_PI;
	static float QUARTER_PI;
	static float INV_PI;
	static float INV_HALF_PI;
	static float INV_TWO_PI;
	static float DEG2RAD;
	static float RAD2DEG;
	static float SQRT2;
	static float INV_SQRT2;
};

RB_NAMESPACE_END