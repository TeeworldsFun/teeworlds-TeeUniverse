#ifndef TU_CLIENT_ASSETS_H
#define TU_CLIENT_ASSETS_H

#include <base/tl/array.h>
#include <engine/graphics.h>

#include <tu/graphics.h>
#include <tu/shared/assetsfile.h>
#include <tu/client/assetpath.h>

//GET/SET
	//Header
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

	//Default value
#define TU_ASSET_GET_FUNC_IMPL_DEFAULT(TypeName) return CAsset::GetValue<TypeName>(ValueType, PathInt, DefaultValue);
#define TU_ASSET_SET_FUNC_IMPL_DEFAULT(TypeName) return CAsset::SetValue<TypeName>(ValueType, PathInt, Value);

	//The data is accessible via a get/set function:
#define TU_ASSET_GET_FUNC_IMPL_FUNC(TypeName, MemberName, Func) case MemberName:\
	return Func();
	
#define TU_ASSET_SET_FUNC_IMPL_FUNC(TypeName, MemberName, Func) case MemberName:\
	Func(Value); \
	return true;

	//The data is accessible via a get/set function and a subpath:
#define TU_ASSET_GET_FUNC_IMPL_SUBFUNC(TypeName, MemberName, Func) case MemberName:\
	return Func(Path);
	
#define TU_ASSET_SET_FUNC_IMPL_SUBFUNC(TypeName, MemberName, Func) case MemberName:\
	Func(Path, Value); \
	return true;

	//The data is stored in a variable:
#define TU_ASSET_GET_FUNC_IMPL_VARIABLE(TypeName, MemberName, Value) case MemberName:\
	return Value;
	
#define TU_ASSET_SET_FUNC_IMPL_VARIABLE(TypeName, MemberName, VariableName) case MemberName:\
	VariableName = Value; \
	return true;
	
	//The data is stored in a array:
	//Asset -> Array[ID]
#define TU_ASSET_GET_FUNC_IMPL_ARRAY(TypeName, MemberName, SubTypeName, ArrayName) case MemberName:\
	if(Path.GetType() == CSubPath::SubTypeName && Path.GetId() >= 0 && Path.GetId() < ArrayName.size())\
		return ArrayName[Path.GetId()];\
	else\
		return DefaultValue;
	
#define TU_ASSET_SET_FUNC_IMPL_ARRAY(TypeName, MemberName, SubTypeName, ArrayName) case MemberName:\
	if(Path.GetType() == CSubPath::SubTypeName && Path.GetId() >= 0 && Path.GetId() < ArrayName.size())\
	{\
		ArrayName[Path.GetId()] = Value;\
		return true;\
	}\
	else return false;
	
	//The data is stored in a array that contains variables:
	//Asset -> Array[ID] -> Variable
#define TU_ASSET_GET_FUNC_IMPL_ARRAY_VARIABLE(TypeName, MemberName, SubTypeName, ArrayName, VariableName) case MemberName:\
	if(Path.GetType() == CSubPath::SubTypeName && Path.GetId() >= 0 && Path.GetId() < ArrayName.size())\
		return ArrayName[Path.GetId()].VariableName;\
	else\
		return DefaultValue;
	
#define TU_ASSET_SET_FUNC_IMPL_ARRAY_VARIABLE(TypeName, MemberName, SubTypeName, ArrayName, VariableName) case MemberName:\
	if(Path.GetType() == CSubPath::SubTypeName && Path.GetId() >= 0 && Path.GetId() < ArrayName.size())\
	{\
		ArrayName[Path.GetId()].VariableName = Value;\
		return true;\
	}\
	else return false;
	
	//The data is stored in a array that contains a raw array of size "SIZE2":
	//Asset -> Array[ID] -> Array[ID2-SHIFT]
#define TU_ASSET_GET_FUNC_IMPL_ARRAY_RARRAY(TypeName, MemberName, SubTypeName, ArrayName, Array2Name, Shift2, Size2) case MemberName:\
	if(Path.GetType() == CSubPath::SubTypeName && Path.GetId() >= 0 && Path.GetId() < ArrayName.size())\
	{\
		if(Path.GetId2() >= Shift2 && Path.GetId2()-Shift2 < Size2)\
			return ArrayName[Path.GetId()].Array2Name[Path.GetId2()-Shift2];\
		else return DefaultValue;\
	}\
	else\
		return DefaultValue;

#define TU_ASSET_SET_FUNC_IMPL_ARRAY_RARRAY(TypeName, MemberName, SubTypeName, ArrayName, Array2Name, Shift2, Size2) case MemberName:\
	if(Path.GetType() == CSubPath::SubTypeName && Path.GetId() >= 0 && Path.GetId() < ArrayName.size())\
	{\
		if(Path.GetId2() >= Shift2 && Path.GetId2()-Shift2 < Size2)\
		{\
			ArrayName[Path.GetId()].Array2Name[Path.GetId2()-Shift2] = Value;\
			return true;\
		}\
		else return false;\
	}\
	else return false;
	
	//The data is stored in a array that contains a raw array of size "SIZE2" that contains a variable:
	//Asset -> Array[ID] -> Array[ID2-SHIFT] -> Variable
#define TU_ASSET_GET_FUNC_IMPL_ARRAY_RARRAY_VARIABLE(TypeName, MemberName, SubTypeName, ArrayName, Array2Name, Shift2, Size2, VariableName) case MemberName:\
	if(Path.GetType() == CSubPath::SubTypeName && Path.GetId() >= 0 && Path.GetId() < ArrayName.size())\
	{\
		if(Path.GetId2() >= Shift2 && Path.GetId2()-Shift2 < Size2)\
			return ArrayName[Path.GetId()].Array2Name[Path.GetId2()-Shift2].VariableName;\
		else return DefaultValue;\
	}\
	else\
		return DefaultValue;

#define TU_ASSET_SET_FUNC_IMPL_ARRAY_RARRAY_VARIABLE(TypeName, MemberName, SubTypeName, ArrayName, Array2Name, Shift2, Size2, VariableName) case MemberName:\
	if(Path.GetType() == CSubPath::SubTypeName && Path.GetId() >= 0 && Path.GetId() < ArrayName.size())\
	{\
		if(Path.GetId2() >= Shift2 && Path.GetId2()-Shift2 < Size2)\
		{\
			ArrayName[Path.GetId()].Array2Name[Path.GetId2()-Shift2].VariableName = Value;\
			return true;\
		}\
		else return false;\
	}\
	else return false;

//Iterator
#define TU_ASSET_ARRAY_ITERATOR(SubTypeName, ArrayName, SubPathConstructor) public:\
	class CIterator##SubTypeName\
	{\
	protected:\
		int m_Part;\
		bool m_Reverse;\
	public:\
		CIterator##SubTypeName() : m_Part(0) {}\
		CIterator##SubTypeName(int Part, bool Reverse) : m_Part(Part), m_Reverse(Reverse) {}\
		CIterator##SubTypeName& operator++() { if(m_Reverse) m_Part--; else m_Part++; return *this; }\
		CSubPath operator*() { return CSubPath::SubPathConstructor(m_Part); }\
		CSubPath operator==(const CIterator##SubTypeName& Iter2) { return Iter2.m_Part == m_Part; }\
		CSubPath operator!=(const CIterator##SubTypeName& Iter2) { return Iter2.m_Part != m_Part; }\
	};\
	CIterator##SubTypeName Begin##SubTypeName() { return CIterator##SubTypeName(0, false); }\
	CIterator##SubTypeName End##SubTypeName() { return CIterator##SubTypeName(ArrayName.size(), false); }\
	CIterator##SubTypeName ReverseBegin##SubTypeName() { return CIterator##SubTypeName(ArrayName.size()-1, true); }\
	CIterator##SubTypeName ReverseEnd##SubTypeName() { return CIterator##SubTypeName(-1, true); }

namespace tu
{

class CAsset
{
public:
	struct CStorageType
	{
		int m_Name;
	};
	
private:
	class CAssetsManager* m_pAssetsManager;
	
public:
	char m_aName[128];

public:
	CAsset() : m_pAssetsManager(0) { }
	virtual ~CAsset() { }
	
	void Update() { }
	void Unload() { }
	
	inline void SetName(const char* pName)
	{
		str_copy(m_aName, pName, sizeof(m_aName));
	}
	
	inline void SetAssetsManager(class CAssetsManager* pAssetsManager)
	{
		m_pAssetsManager = pAssetsManager;
	}
	
	inline class CAssetsManager* AssetsManager() { return m_pAssetsManager; }

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
