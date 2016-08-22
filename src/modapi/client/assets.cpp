#include "assets.h"

namespace tu
{

template<>
bool CAsset::SetValue<const char*>(int ValueType, int Path, const char* pText)
{
	switch(ValueType)
	{
		case NAME:
			SetName(pText);
			return true;
	}
	
	return false;
}

template<>
char* CAsset::GetValue(int ValueType, int Path, char* DefaultValue)
{
	switch(ValueType)
	{
		case NAME:
			return m_aName;
	}
	
	return DefaultValue;
}

}
