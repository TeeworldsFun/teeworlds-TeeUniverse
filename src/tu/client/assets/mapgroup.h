#ifndef TU_CLIENT_ASSETS_MAPGROUP_H
#define TU_CLIENT_ASSETS_MAPGROUP_H

#include <tu/client/assets.h>
#include <engine/shared/datafile.h>

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
	
	void InitFromAssetsFile(CDataFileReader* pFileReader, const CStorageType* pItem);
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
private:	
	array<CAssetPath> m_Layers;
	vec2 m_Position;
	vec2 m_HardParallax;
	
/* FUNCTIONS **********************************************************/
public:
	CAsset_MapGroup();
	
	inline vec2 GetPosition() const { return m_Position; }
	inline vec2 GetHardParallax() const { return m_HardParallax; }
	inline CAssetPath GetLayer(CSubPath SubPath) const { return m_Layers[SubPath.GetId()]; }
	
	inline void SetPosition(vec2 Value) { m_Position = Value; }
	inline void SetHardParallax(vec2 Value) { m_HardParallax = Value; }
	
	inline void AddLayer(CAssetPath Path) { m_Layers.add(Path); }
	
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
	
	TU_ASSET_ARRAY_ITERATOR(Layer, m_Layers, Layer)
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		for(int i=0; i<m_Layers.size(); i++)
			m_Layers[i].OnIdDeleted(Path);
	}
};

}

#endif
