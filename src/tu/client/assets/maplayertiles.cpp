#include "maplayertiles.h"

#include <engine/shared/datafile.h>
#include <tu/client/graphics.h>

namespace tu
{

CAsset_MapLayerTiles::CAsset_MapLayerTiles() :
	m_Width(0),
	m_Height(0),
	m_pTiles(0),
	m_Color(vec4(1.0f, 1.0f, 1.0f, 1.0f))
{
	
}

CAsset_MapLayerTiles::CAsset_MapLayerTiles(const CAsset_MapLayerTiles& Layer)
{
	*this = Layer;
}

CAsset_MapLayerTiles::~CAsset_MapLayerTiles()
{
	if(m_pTiles)
		delete[] m_pTiles;
}

CAsset_MapLayerTiles& CAsset_MapLayerTiles::operator=(const CAsset_MapLayerTiles& Layer)
{
	CAsset::operator=(Layer);
	
	if(Layer.m_pTiles)
	{
		SetSize(Layer.m_Width, Layer.m_Height);
		mem_copy(m_pTiles, Layer.m_pTiles, m_Width*m_Height*sizeof(CTile));
		
		m_ImagePath = Layer.m_ImagePath;
		m_Color = Layer.m_Color;
	}
	else
	{
		m_Width = 0;
		m_Height = 0;
		m_pTiles = 0;
		
		m_ImagePath = CAssetPath::Null();
		m_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	return *this;
}

void CAsset_MapLayerTiles::SetSize(int Width, int Height)
{
	m_Width = max(1, Width);
	m_Height = max(1, Height);
	
	if(m_pTiles)
		delete[] m_pTiles;
	
	m_pTiles = new CTile[m_Width*m_Height];
	
	for(int j=0; j<m_Height; j++)
	{
		for(int i=0; i<m_Width; i++)
		{
			m_pTiles[j*m_Width+i] = CTile();
		}
	}
}

void CAsset_MapLayerTiles::Resize(int Width, int Height)
{
	int NewWidth = max(1, Width);
	int NewHeight = max(1, Height);
	CTile* pNewTiles = new CTile[NewWidth*NewHeight];
	
	if(m_pTiles)
	{
		for(int j=0; j<NewHeight; j++)
		{
			for(int i=0; i<NewWidth; i++)
			{
				if(j<m_Height && i<m_Width)
					pNewTiles[j*NewWidth+i] = m_pTiles[j*m_Width+i];
				else
					pNewTiles[j*NewWidth+i] = CTile();
			}
		}
		
		delete[] m_pTiles;
	}
	
	m_Width = NewWidth;
	m_Height = NewHeight;
	m_pTiles = pNewTiles;
}

CAsset_MapLayerTiles::CTile* CAsset_MapLayerTiles::GetTile(int x, int y)
{
	if(x >= 0 && x < m_Width && y >= 0 && y < m_Height)
		return &m_pTiles[y*m_Width+x];
	else
		return 0;
}

/* IO *****************************************************************/

void CAsset_MapLayerTiles::InitFromAssetsFile(CAssetsManager* pAssetsManager, tu::IAssetsFile* pAssetsFile, const CAsset_MapLayerTiles::CStorageType* pItem)
{
	// copy name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
	
	SetSize(pItem->m_Width, pItem->m_Height);
	
	m_ImagePath = pItem->m_ImagePath;
	m_Color = tu::IntToColor(pItem->m_Color);
	
	// load tiles
	int nbTiles = pItem->m_Width * pItem->m_Height;
	const CStorageType::CTile* pTiles = static_cast<CStorageType::CTile*>(pAssetsFile->GetData(pItem->m_TilesData));
	for(int i=0; i<nbTiles; i++)
	{
		m_pTiles[i].m_Index = pTiles[i].m_Index;
		m_pTiles[i].m_Flags = pTiles[i].m_Flags;
		m_pTiles[i].m_Skip = pTiles[i].m_Skip;
		m_pTiles[i].m_Reserved = pTiles[i].m_Reserved;
	}
}

void CAsset_MapLayerTiles::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CAsset_MapLayerTiles::CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);
	
	Item.m_ImagePath = m_ImagePath.ConvertToInteger();
	Item.m_Color = tu::ColorToInt(m_Color);
	
	{
		int nbTiles = m_Width * m_Height;
		CStorageType::CTile* pTiles = new CStorageType::CTile[nbTiles];
		for(int i=0; i<nbTiles; i++)
		{
			pTiles[i].m_Index = m_pTiles[i].m_Index;
			pTiles[i].m_Flags = m_pTiles[i].m_Flags;
			pTiles[i].m_Skip = m_pTiles[i].m_Skip;
			pTiles[i].m_Reserved = m_pTiles[i].m_Reserved;
		}
		Item.m_Width = m_Width;
		Item.m_Height = m_Height;
		Item.m_TilesData = pFileWriter->AddData(nbTiles * sizeof(CStorageType::CTile), pTiles);
		delete[] pTiles;
	}
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE INT **********************************************************/
	
template<>
bool CAsset_MapLayerTiles::SetValue<int>(int ValueType, int Path, int Value)
{
	switch(ValueType)
	{
		case WIDTH:
			if(m_Width != Value)
				Resize(Value, m_Height);
			return true;
		case HEIGHT:
			if(m_Height != Value)
				Resize(m_Width, Value);
			return true;
	}
	
	return CAsset::SetValue<int>(ValueType, Path, Value);
}

template<>
int CAsset_MapLayerTiles::GetValue(int ValueType, int Path, int DefaultValue)
{
	switch(ValueType)
	{
		case WIDTH:
			return max(1, m_Width);
		case HEIGHT:
			return max(1, m_Height);
		default:
			return CAsset::GetValue<int>(ValueType, Path, DefaultValue);
	}
}

/* VALUE ASSETPATH ****************************************************/
	
template<>
CAssetPath CAsset_MapLayerTiles::GetValue<CAssetPath>(int ValueType, int PathInt, CAssetPath DefaultValue)
{
	switch(ValueType)
	{
		case IMAGEPATH:
			return m_ImagePath;
		default:
			return CAsset::GetValue<CAssetPath>(ValueType, PathInt, DefaultValue);
	}
}
	
template<>
bool CAsset_MapLayerTiles::SetValue<CAssetPath>(int ValueType, int Path, CAssetPath Value)
{
	switch(ValueType)
	{
		case IMAGEPATH:
			m_ImagePath = Value;
			return true;
	}
	
	return CAsset::SetValue<CAssetPath>(ValueType, Path, Value);
}

/* VALUE VEC4 *********************************************************/
	
template<>
vec4 CAsset_MapLayerTiles::GetValue<vec4>(int ValueType, int PathInt, vec4 DefaultValue)
{
	switch(ValueType)
	{
		case COLOR:
			return m_Color;
		default:
			return CAsset::GetValue<vec4>(ValueType, PathInt, DefaultValue);
	}
}
	
template<>
bool CAsset_MapLayerTiles::SetValue<vec4>(int ValueType, int PathInt, vec4 Value)
{
	switch(ValueType)
	{
		case COLOR:
			m_Color = Value;
			return true;
	}
	
	return CAsset::SetValue<vec4>(ValueType, PathInt, Value);
}

}
