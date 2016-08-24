#include "sprite.h"

#include <engine/shared/datafile.h>
#include <tu/client/graphics.h>

namespace tu
{

CAsset_Sprite::CAsset_Sprite() :
	m_X(0),
	m_Y(0),
	m_Width(1),
	m_Height(1)
{
	
}

/* IO *****************************************************************/

void CAsset_Sprite::InitFromAssetsFile(CAssetsManager* pAssetsManager, IAssetsFile* pAssetsFile, const CStorageType* pItem)
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
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	Item.m_ImagePath = m_ImagePath.ConvertToInteger();
	Item.m_X = m_X;
	Item.m_Y = m_Y;
	Item.m_Width = m_Width;
	Item.m_Height = m_Height;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE INT **********************************************************/

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

/* VALUE ASSETPATH ****************************************************/

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

}
