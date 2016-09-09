/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef TU_SHARED_GEOMETRY_H
#define TU_SHARED_GEOMETRY_H

#include <base/vmath.h>

namespace tu
{

namespace geometry
{

inline void UpdateBoundingBox(vec2 p, vec2* bbmin, vec2* bbmax)
{
	if(bbmin->x > p.x)
		bbmin->x = p.x;
	if(bbmin->y > p.y)
		bbmin->y = p.y;
	if(bbmax->x < p.x)
		bbmax->x = p.x;
	if(bbmax->y < p.y)
		bbmax->y = p.y;
}

inline bool SameSide(const vec2& l0, const vec2& l1, const vec2& p0, const vec2& p1)
{
	vec2 l0l1 = l1-l0;
	vec2 l0p0 = p0-l0;
	vec2 l0p1 = p1-l0;
	
	return sign(l0l1.x*l0p0.y - l0l1.y*l0p0.x) == sign(l0l1.x*l0p1.y - l0l1.y*l0p1.x);
}

//t0, t1 and t2 are position of triangle vertices
inline vec3 BarycentricCoordinates(const vec2& t0, const vec2& t1, const vec2& t2, const vec2& p)
{
    vec2 e0 = t1 - t0;
    vec2 e1 = t2 - t0;
    vec2 e2 = p - t0;
    
    float d00 = dot(e0, e0);
    float d01 = dot(e0, e1);
    float d11 = dot(e1, e1);
    float d20 = dot(e2, e0);
    float d21 = dot(e2, e1);
    float denom = d00 * d11 - d01 * d01;
    
    vec3 bary;
    bary.x = (d11 * d20 - d01 * d21) / denom;
    bary.y = (d00 * d21 - d01 * d20) / denom;
    bary.z = 1.0f - bary.x - bary.y;
    
    return bary;
}

//t0, t1 and t2 are position of triangle vertices
inline bool InsideTriangle(const vec2& t0, const vec2& t1, const vec2& t2, const vec2& p)
{
    vec3 bary = BarycentricCoordinates(t0, t1, t2, p);
    return (bary.x >= 0.0f && bary.y >= 0.0f && bary.x + bary.y < 1.0f);
}

//t0, t1 and t2 are position of quad vertices
inline bool InsideQuad(const vec2& q0, const vec2& q1, const vec2& q2, const vec2& q3, const vec2& p)
{
	if(SameSide(q1, q2, p, q0))
		return InsideTriangle(q0, q1, q2, p);
	else
		return InsideTriangle(q1, q2, q3, p);
}

//v0, v1 and v2 are values at triangle vertices
template<typename T>
inline T BarycentricInterpolation(const vec3& bary, const T& v0, const T& v1, const T& v2)
{
	return v1 * bary.x + v2 * bary.y + v0 * bary.z;
}

template<typename T>
inline T SampleTriangle(const vec2& t0, const vec2& t1, const vec2& t2, const vec2 p, const T& v0, const T& v1, const T& v2)
{
	return BarycentricInterpolation(BarycentricCoordinates(t0, t1, t2, p), v0, v1, v2);
}

//the quad should not intersect himself
template<typename T>
inline T SampleQuad(const vec2& q0, const vec2& q1, const vec2& q2, const vec2& q3, const vec2 p, const T& v0, const T& v1, const T& v2, const T& v3)
{
	if(SameSide(q1, q2, p, q0))
		return SampleTriangle(q0, q1, q2, p, v0, v1, v2);
	else
		return SampleTriangle(q1, q2, q3, p, v1, v2, v3);
}

}

}

#endif
