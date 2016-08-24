#ifndef TU_CLIENT_ASSETS_MAPLAYERQUADS_H
#define TU_CLIENT_ASSETS_MAPLAYERQUADS_H

#include <tu/client/assets.h>

class CDataFileWriter;

namespace tu
{

class CAsset_MapLayerQuads : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_MAPLAYERQUADS;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		struct CQuad
		{
			float m_PivotX;
			float m_PivotY;
			float m_SizeX;
			float m_SizeY;
			float m_Angle;
			float m_Vertices[4*2];
			float m_UVs[4*2];
			int m_Colors[4];
		};
		
		int m_NumQuads;
		int m_QuadsData;
		
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
			TYPE_QUAD=0,
			NUM_TYPES,
		};

	public:
		CSubPath() : CGenericPath() { }
		CSubPath(int PathInt) : CGenericPath(PathInt) { }
		CSubPath(int Type, int Id) : CGenericPath(Type, 0, 0x0, Id) { }
		
		static inline CSubPath Null() { return CSubPath(CGenericPath::UNDEFINED); }
		static inline CSubPath Quad(int Id) { return CSubPath(TYPE_QUAD, Id); }
	};
	
	class CQuad
	{
	public:
		vec2 m_Pivot;
		vec2 m_Size;
		float m_Angle;
		vec2 m_Vertices[4];
		vec2 m_UVs[4];
		vec4 m_Colors[4];
		
	public:
		CQuad() :
			m_Size(1.0f, 1.0f),
			m_Angle(0.0f)
		{
			for(int p=0; p<4; p++)
			{
				m_Colors[p] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	};

/* MEMBERS ************************************************************/
public:	
	CAssetPath m_ImagePath;
	array<CQuad> m_Quads;

/* FUNCTIONS **********************************************************/
public:
	CAsset_MapLayerQuads();
	
/* GET/SET ************************************************************/
public:
	enum
	{
		IMAGEPATH = CAsset::NUM_MEMBERS, //Path
		QUAD_PIVOT_POSITION_X, //Float
		QUAD_PIVOT_POSITION_Y, //Float
		QUAD_SIZE_X, //Float
		QUAD_SIZE_Y, //Float
		QUAD_ANGLE, //Float
		QUAD_POINT0_POSITION_X, //Float
		QUAD_POINT1_POSITION_X, //Float
		QUAD_POINT2_POSITION_X, //Float
		QUAD_POINT3_POSITION_X, //Float
		QUAD_POINT0_POSITION_Y, //Float
		QUAD_POINT1_POSITION_Y, //Float
		QUAD_POINT2_POSITION_Y, //Float
		QUAD_POINT3_POSITION_Y, //Float
		QUAD_POINT0_UV_X, //Float
		QUAD_POINT1_UV_X, //Float
		QUAD_POINT2_UV_X, //Float
		QUAD_POINT3_UV_X, //Float
		QUAD_POINT0_UV_Y, //Float
		QUAD_POINT1_UV_Y, //Float
		QUAD_POINT2_UV_Y, //Float
		QUAD_POINT3_UV_Y, //Float
		QUAD_POINT0_COLOR, //Vec4
		QUAD_POINT1_COLOR, //Vec4
		QUAD_POINT2_COLOR, //Vec4
		QUAD_POINT3_COLOR, //Vec4
	};
	
	TU_ASSET_GETSET_FUNC()
};

}

#endif
