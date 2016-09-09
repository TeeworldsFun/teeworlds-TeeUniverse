#ifndef TU_GRAPHICS_H
#define TU_GRAPHICS_H

#include <base/vmath.h>

namespace tu
{

//Functions
inline int ColorToInt(const vec4& Color)
{
	int Value = static_cast<int>(Color.r * 255.0f);
	Value += (static_cast<int>(Color.g * 255.0f)<<8);
	Value += (static_cast<int>(Color.b * 255.0f)<<16);
	Value += (static_cast<int>(Color.a * 255.0f)<<24);
	return Value;
}

inline vec4 IntToColor(int Value)
{
	return vec4(
		static_cast<float>(Value & 255)/255.0f,
		static_cast<float>((Value>>8) & 255)/255.0f,
		static_cast<float>((Value>>16) & 255)/255.0f,
		static_cast<float>((Value>>24) & 255)/255.0f
	);
}

}

#endif
