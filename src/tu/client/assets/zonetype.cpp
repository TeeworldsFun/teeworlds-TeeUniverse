#include "zonetype.h"

#include <engine/shared/datafile.h>
#include <tu/client/graphics.h>

namespace tu
{

CAsset_ZoneType::CAsset_ZoneType()
{
	
}

/* IO *****************************************************************/

void CAsset_ZoneType::InitFromAssetsFile(IAssetsFile* pAssetsFile, const CStorageType* pItem)
{
	// copy name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
	
	// load indices
	const CStorageType::CIndex* pIndices = static_cast<CStorageType::CIndex*>(pAssetsFile->GetData(pItem->m_IndicesData));
	for(int i=0; i<pItem->m_NumIndices; i++)
	{
		m_Indices.add(CIndex());
		CIndex& Index = m_Indices[m_Indices.size()-1];
		str_copy(Index.m_aName, pIndices[i].m_aName, sizeof(Index.m_aName));
		Index.m_Number = pIndices[i].m_Number;
		Index.m_Color = IntToColor(pIndices[i].m_Color);
	}
}

void CAsset_ZoneType::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	{
		CStorageType::CIndex* pIndices = new CStorageType::CIndex[m_Indices.size()];
		for(int i=0; i<m_Indices.size(); i++)
		{
			str_copy(pIndices[i].m_aName, m_Indices[i].m_aName, sizeof(pIndices[i].m_aName));
			pIndices[i].m_Number = m_Indices[i].m_Number;
			pIndices[i].m_Color = ColorToInt(m_Indices[i].m_Color);
		}
		Item.m_NumIndices = m_Indices.size();
		Item.m_IndicesData = pFileWriter->AddData(Item.m_NumIndices * sizeof(CStorageType::CIndex), pIndices);
		delete[] pIndices;
	}
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE INT **********************************************************/

template<>
int CAsset_ZoneType::GetValue(int ValueType, int PathInt, int DefaultValue)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_SUBFUNC(int, INDEX_NUMBER, GetIndexNumber);
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(int)
}
	
template<>
bool CAsset_ZoneType::SetValue(int ValueType, int PathInt, int Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_SUBFUNC(int, INDEX_NUMBER, SetIndexNumber);
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(int)
}

/* VALUE VEC4 *********************************************************/

template<>
vec4 CAsset_ZoneType::GetValue(int ValueType, int PathInt, vec4 DefaultValue)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_SUBFUNC(vec4, INDEX_COLOR, GetIndexColor);
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(vec4)
}
	
template<>
bool CAsset_ZoneType::SetValue(int ValueType, int PathInt, vec4 Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_SUBFUNC(vec4, INDEX_COLOR, SetIndexColor);
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(vec4)
}

/* VALUE STRING *******************************************************/

template<>
char* CAsset_ZoneType::GetValue(int ValueType, int PathInt, char* DefaultValue)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_SUBFUNC(char*, INDEX_NAME, GetIndexName);
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(char*)
}
	
template<>
bool CAsset_ZoneType::SetValue(int ValueType, int PathInt, const char* Value)
{
	CSubPath Path(PathInt);
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_SUBFUNC(char*, INDEX_NAME, SetIndexName);
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(const char*)
}

}
