#ifndef __FIX_RESHARPER_ERROR__
#define __FIX_RESHARPER_ERROR__

#ifndef __HLSL_SHADER__

#define inout
#define in
#define out

#define RAY_FLAG_FORCE_OPAQUE 1
#define RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH 1
#define RAY_FLAG_SKIP_CLOSEST_HIT_SHADER 1

#define SV_RayPayload
#define SV_POSITION
#define SV_TARGET
#define SV_InstanceID

#define POSITION
#define COLOR

#define SHADER_STRUCTURED_BUFFER_DEFINE(type, name, base, space) type name[1]
#define SHADER_CONSTANT_BUFFER_DEFINE(type, name, base, space) type name
#define SHADER_RESOURCE_DEFINE(type, name, base, space) type name

#define SYSTEM_VALUE(type, name, value) type name

#define OPERATOR_DEFINE(type, op, argument) type operator op (argument) { return type(); }

#define ANY_OPERATOR_DEFINE() \
	OPERATOR_DEFINE(this_type, +, this_type); \
	OPERATOR_DEFINE(this_type, -, this_type); \
	OPERATOR_DEFINE(this_type, *, this_type); \
	OPERATOR_DEFINE(this_type, /, this_type); \
	OPERATOR_DEFINE(this_type, +, T); \
	OPERATOR_DEFINE(this_type, *, T); \
	OPERATOR_DEFINE(this_type, -, T); \
	OPERATOR_DEFINE(this_type, / , T); \
	OPERATOR_DEFINE(this_type, +, ); \
	OPERATOR_DEFINE(this_type, -, ); \
	OPERATOR_DEFINE(this_type, +=, this_type); \
	OPERATOR_DEFINE(this_type, -=, this_type); \
	OPERATOR_DEFINE(this_type, *=, this_type); \
	OPERATOR_DEFINE(this_type, +=, T); \
	OPERATOR_DEFINE(this_type, *=, T); \
	OPERATOR_DEFINE(this_type, -=, T); \
	OPERATOR_DEFINE(this_type, /=, T); 


using uint = unsigned;

template <typename T>
struct any1 {
	using this_type = any1;
	
	T x;

	any1() = default;
	any1(T x) {}

	ANY_OPERATOR_DEFINE();
};

template <typename T>
struct any2 {
	using this_type = any2;
	
	T x, y;

	T xy;
	
	any2() = default;
	any2(T x, T y) {}
	any2(T v) {}

	operator any1<T>() { return any1<T>(); }

	ANY_OPERATOR_DEFINE();
};

template <typename T>
struct any3 {
	using this_type = any3;
	
	T x, y, z;

	T xyz;
	T xy;
	
	any3() = default;
	any3(T x, T y, T z) {}
	any3(any2 v, T z) {}
	any3(T v) {}

	operator any2<T>() { return any2<T>(); }
	operator any1<T>() { return any1<T>(); }

	ANY_OPERATOR_DEFINE();
};

template <typename T>
struct any4 {
	using this_type = any4;
	
	T x, y, z, w;

	T xyzw;
	T xyz;
	T xy;
	
	any4() = default;
	any4(T x, T y, T z, T w) {}
	any4(any3 v, T w) {}
	any4(any2 v, T z, T w) {}
	any4(T v) {}

	operator any3<T>() { return any3<T>(); }
	operator any2<T>() { return any2<T>(); }
	operator any1<T>() { return any1<T>(); }

	ANY_OPERATOR_DEFINE();
};

using uint1 = any1<uint>;
using uint2 = any2<uint>;
using uint3 = any3<uint>;
using uint4 = any4<uint>;

using float1 = any1<float>;
using float2 = any2<float>;
using float3 = any3<float>;
using float4 = any4<float>;

template <typename T>
struct RWTexture2D { T value; T& operator[](float2 value) { return value; } };

struct SamplerState {};

struct Texture2D {
	float4 Sample(SamplerState state, float2 value) { return float4(); }

	float4 SampleLevel(SamplerState state, float2 value, float level) { return float4(); }
};

struct RayDesc {
	float3 Direction;
	float3 Origin;
	float TMin;
	float TMax;
};

struct float4x4 {
	float4 x, y, z, w;
};

struct float4x3 {
	float4 x, y, w;

	float4 operator[](uint index) { return 1; }
};

struct float3x3 {
	float3 x, y, z;

	float3x3() = default;

	float3x3(float3 x, float3 y, float3 z) : x(x), y(y), z(z) {}
};

struct BuiltInTriangleIntersectionAttributes {
	float2 barycentrics;
};

struct RaytracingAccelerationStructure {
	
};

template <typename Payload>
void TraceRay(
	RaytracingAccelerationStructure AccelerationStructure,
	uint RayFlags,
	uint InstanceInclusionMask,
	uint RayContributionToHitGroupIndex,
	uint MultiplierForGeometryContributionToShaderIndex,
	uint MissShaderIndex,
	RayDesc ray,
	inout Payload payload) {}

uint2 DispatchRaysDimensions() { return 0; };

uint2 DispatchRaysIndex() { return 0; }

uint PrimitiveIndex() { return 0; }

uint InstanceID() { return 0; }

float4x3 ObjectToWorld4x3() { return float4x3(); }

float4x3 WorldToObject4x3() { return float4x3(); }

float3 WorldRayDirection() { return float3(); }

float3 WorldRayOrigin() { return float3(); }

float RayTCurrent() { return 0; }

template <typename T>
T normalize(T value) { return value; }

template <typename T>
T abs(T value) { return value; }

template <typename T>
T cos(T value) { return value; }

template <typename T>
T sin(T value) { return value; }

template <typename T>
T acos(T value) { return value; }

template <typename T>
T atan2(T y, T x) { return x; }

template <typename T>
T sqrt(T value) { return value; }

template <typename T>
T floor(T value) { return value; }

template <typename T>
float dot(T x, T y) { return x; }

template <typename T>
float length(T x) { return 0; }

template <typename T>
T pow(T x, float v) { return x; }

template <typename T>
T cross(T x, T y) { return x; }

template <typename T>
T lerp(T x, T y, float s) { return x * (1 - s) + y * s; }

template <typename T>
bool isnan(T x) { return true; }

template <typename T>
bool isinf(T x) { return true; }

template <typename T>
T log(T x) { return x; }

template <typename T>
T transpose(T x) { return x; }

float4 mul(float4 x, float4x4 y) { return 0; }
float4 mul(float4 x, float4x3 y) { return 0; }
float3 mul(float3x3 x, float3 y) { return 0; }

float max(float x, float y) { return x; }
float min(float x, float y) { return x; }

float clamp(float x, float min, float max) { return x; }

float ldexp(float x, float y) { return x; }

#else

#define SHADER_STRUCTURED_BUFFER_DEFINE(type, name, base, space) StructuredBuffer<type> name : register(base, space)
#define SHADER_CONSTANT_BUFFER_DEFINE(type, name, base, space) ConstantBuffer<type> name : register(base, space)
#define SHADER_RESOURCE_DEFINE(type, name, base, space) type name : register(base, space)

#define SYSTEM_VALUE(type, name, value) type name : value

#endif

#endif