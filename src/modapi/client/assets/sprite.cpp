#include "sprite.h"

#include <engine/shared/datafile.h>
#include <modapi/client/graphics.h>

namespace tu
{

void CAsset_Sprite::InitFromAssetsFile(CAssetManager* pAssetManager, IAssetsFile* pAssetsFile, const CAsset_Sprite::CStorageType* pItem)
{
	// copy name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
				
	// copy info
	m_ImagePath = CAssetPath(pItem->m_ImagePath);
	m_X = pItem->m_X;
	m_Y = pItem->m_Y;
	m_Width = pItem->m_Width;
	m_Height = pItem->m_Height;
}

void CAsset_Sprite::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CAsset_Sprite::CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	Item.m_ImagePath = m_ImagePath.ConvertToInteger();
	Item.m_X = m_X;
	Item.m_Y = m_Y;
	Item.m_Width = m_Width;
	Item.m_Height = m_Height;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(CAsset_Sprite::TypeId), Position, sizeof(CAsset_Sprite::CStorageType), &Item);
}

void CAsset_Sprite::Unload(class CAssetManager* pAssetManager)
{
	
}
	
template<>
bool CAsset_Sprite::SetValue<int>(int ValueType, int Path, int Value)
{
	switch(ValueType)
	{
		case X:
			m_X = Value;
			return true;
		case Y:
			m_Y = Value;
			return true;
		case WIDTH:
			m_Width = Value;
			return true;
		case HEIGHT:
			m_Height = Value;
			return true;
	}
	
	return CAsset::SetValue<int>(ValueType, Path, Value);
}
	
template<>
bool CAsset_Sprite::SetValue<CAssetPath>(int ValueType, int Path, CAssetPath Value)
{
	switch(ValueType)
	{
		case IMAGEPATH:
			m_ImagePath = Value;
			return true;
	}
	
	return CAsset::SetValue<CAssetPath>(ValueType, Path, Value);
}

template<>
int CAsset_Sprite::GetValue(int ValueType, int Path, int DefaultValue)
{
	switch(ValueType)
	{
		case X:
			return m_X;
		case Y:
			return m_Y;
		case WIDTH:
			return m_Width;
		case HEIGHT:
			return m_Height;
		default:
			return CAsset::GetValue<int>(ValueType, Path, DefaultValue);
	}
}

template<>
CAssetPath CAsset_Sprite::GetValue(int ValueType, int Path, CAssetPath DefaultValue)
{
	switch(ValueType)
	{
		case IMAGEPATH:
			return m_ImagePath;
		default:
			return CAsset::GetValue<CAssetPath>(ValueType, Path, DefaultValue);
	}
}

}
