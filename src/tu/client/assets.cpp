#include "assets.h"

namespace tu
{

template<>
const char* CAsset::GetValue(int ValueType, int Path, const char* DefaultValue) const
{
	switch(ValueType)
	{
		case NAME:
			return m_aName;
	}
	
	return DefaultValue;
}

template<>
bool CAsset::SetValue(int ValueType, int Path, const char* pText)
{
	switch(ValueType)
	{
		case NAME:
			SetName(pText);
			return true;
	}
	
	return false;
}

}
