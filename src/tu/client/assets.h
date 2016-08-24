#ifndef TU_CLIENT_ASSETS_H
#define TU_CLIENT_ASSETS_H

#include <base/tl/array.h>
#include <engine/graphics.h>

#include <tu/graphics.h>
#include <tu/shared/assetsfile.h>
#include <tu/client/assetpath.h>

#define TU_ASSET_GETSET_FUNC() 	template<typename T>\
	T GetValue(int ValueType, int Path, T DefaultValue)\
	{\
		return CAsset::GetValue<T>(ValueType, Path, DefaultValue);\
	}\
	\
	template<typename T>\
	bool SetValue(int ValueType, int Path, T Value)\
	{\
		return CAsset::SetValue<T>(ValueType, Path, Value);\
	}
	

namespace tu
{

class CAsset
{
public:
	struct CStorageType
	{
		int m_Name;
	};
	
public:
	char m_aName[128];

public:
	virtual ~CAsset() { }
	
	void Update() { }
	void Unload() { }
	
	inline void SetName(const char* pName)
	{
		str_copy(m_aName, pName, sizeof(m_aName));
	}

public:
	enum
	{
		NAME = 0, //String
		NUM_MEMBERS,
	};
	
	template<typename T>
	T GetValue(int ValueType, int Path, T DefaultValue)
	{
		return DefaultValue;
	}
	
	template<typename T>\
	bool SetValue(int ValueType, int Path, T Value)
	{
		return false;
	}

public:
	int AddSubItem(int SubItemType) { return -1; }
	bool DeleteSubItem(int SubItemPath) { return false; }
	
	void OnAssetDeleted(const CAssetPath& Path) { }
	void OnSubItemDeleted(const CAssetPath& Path, int SubItemPath) { }
};

}

#endif
