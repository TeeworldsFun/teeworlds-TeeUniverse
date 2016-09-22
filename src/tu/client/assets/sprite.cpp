#include "sprite.h"
#include "image.h"

#include <engine/shared/datafile.h>
#include <tu/client/assetsrenderer.h>

namespace tu
{

CAsset_Sprite::CAsset_Sprite() :
	m_X(0),
	m_Y(0),
	m_Width(1),
	m_Height(1)
{
	
}

int CAsset_Sprite::GetPixelWidth() const
{
	const CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(m_ImagePath);
	if(pImage)
		return (pImage->GetWidth() * m_Width)/pImage->GetGridWidth();
	else
		return m_Width;
}

int CAsset_Sprite::GetPixelHeight() const
{
	const CAsset_Image* pImage = AssetsManager()->GetAsset<CAsset_Image>(m_ImagePath);
	if(pImage)
		return (pImage->GetHeight() * m_Height)/pImage->GetGridHeight();
	else
		return m_Height;
}

/* IO *****************************************************************/

void CAsset_Sprite::InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem)
{
	// copy name
	SetName((char *)pFileReader->GetData(pItem->m_Name));
				
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
	Item.m_ImagePath = m_ImagePath;
	Item.m_X = m_X;
	Item.m_Y = m_Y;
	Item.m_Width = m_Width;
	Item.m_Height = m_Height;
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE INT **********************************************************/

template<>
int CAsset_Sprite::GetValue(int ValueType, int PathInt, int DefaultValue) const
{
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(int, X, GetX())
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(int, Y, GetY())
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(int, WIDTH, GetWidth())
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(int, HEIGHT, GetHeight())
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(int)
}
	
template<>
bool CAsset_Sprite::SetValue(int ValueType, int PathInt, int Value)
{
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(int, X, m_X)
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(int, Y, m_Y)
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(int, WIDTH, m_Width)
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(int, HEIGHT, m_Height)
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(int)
}

/* VALUE ASSETPATH ****************************************************/

template<>
CAssetPath CAsset_Sprite::GetValue(int ValueType, int PathInt, CAssetPath DefaultValue) const
{
	switch(ValueType)
	{
		TU_ASSET_GET_FUNC_IMPL_VARIABLE(CAssetPath, IMAGEPATH, GetImagePath())
	}
	
	TU_ASSET_GET_FUNC_IMPL_DEFAULT(CAssetPath)
}
	
template<>
bool CAsset_Sprite::SetValue(int ValueType, int PathInt, CAssetPath Value)
{
	switch(ValueType)
	{
		TU_ASSET_SET_FUNC_IMPL_VARIABLE(CAssetPath, IMAGEPATH, m_ImagePath)
	}
	
	TU_ASSET_SET_FUNC_IMPL_DEFAULT(CAssetPath)
}

}
