#ifndef TU_CLIENT_ASSETS_MAPGROUP_H
#define TU_CLIENT_ASSETS_MAPGROUP_H

#include <tu/client/assets.h>

class CDataFileWriter;

namespace tu
{

class CAsset_MapGroup : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_MAPGROUP;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		int m_NumLayers;
		int m_LayersData;
		int m_PositionX;
		int m_PositionY;
		int m_HardParallaxX;
		int m_HardParallaxY;
	};
	
	void InitFromAssetsFile(class CAssetsManager* pAssetsManager, class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);
	
/* SUBITEMS ***********************************************************/
public:
	class CSubPath : public CGenericPath<2, 0, 0>
	{
	public:
		enum
		{
			TYPE_LAYER=0,
			NUM_TYPES,
		};

	public:
		CSubPath() : CGenericPath() { }
		CSubPath(int PathInt) : CGenericPath(PathInt) { }
		CSubPath(int Type, int Id) : CGenericPath(Type, 0, 0x0, Id) { }
		
		static inline CSubPath Null() { return CSubPath(CGenericPath::UNDEFINED); }
		static inline CSubPath Layer(int Id) { return CSubPath(TYPE_LAYER, Id); }
	};

/* MEMBERS ************************************************************/
public:	
	array<CAssetPath> m_Layers;
	vec2 m_Position;
	vec2 m_HardParallax;
	
/* FUNCTIONS **********************************************************/
public:
	CAsset_MapGroup();
	
/* GET/SET ************************************************************/
public:
	enum
	{
		LAYERPATH = CAsset::NUM_MEMBERS, //Path
		POSITION_X, //float
		POSITION_Y, //float
		HARDPARALLAX_X, //float
		HARDPARALLAX_Y, //float
	};
	
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		for(int i=0; i<m_Layers.size(); i++)
		{
			m_Layers[i].OnIdDeleted(Path);
		}
	}
};

}

#endif
