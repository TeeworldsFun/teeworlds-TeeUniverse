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
			int m_AnimationPath;
		};
		
		int m_ImagePath;
		int m_NumQuads;
		int m_QuadsData;
		
	};
	
	void InitFromAssetsFile(class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position);

/* SUBITEMS ***********************************************************/
public:
	class CSubPath : public CGenericPath<2, 0, 0, 4>
	{
	public:
		enum
		{
			TYPE_QUAD=0,
			NUM_TYPES,
		};
		
		enum
		{
			POINT_NONE=0,
			POINT_PIVOT,
			POINT_VERTEX0,
			POINT_VERTEX1,
			POINT_VERTEX2,
			POINT_VERTEX3,
		};

	public:
		CSubPath() : CGenericPath() { }
		CSubPath(int PathInt) : CGenericPath(PathInt) { }
		CSubPath(int Type, int Id) : CGenericPath(Type, 0, 0x0, Id, POINT_NONE) { }
		CSubPath(int Type, int Id, int Id2) : CGenericPath(Type, 0, 0x0, Id, Id2) { }
		
		CSubPath Point(int Point) { CSubPath Path = *this; Path.SetId2(Point); return Path; }
		
		static inline CSubPath Null() { return CSubPath(CGenericPath::UNDEFINED); }
		static inline CSubPath Quad(int Id, int Id2 = POINT_NONE) { return CSubPath(TYPE_QUAD, Id, Id2); }
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
		CAssetPath m_AnimationPath;
		
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
		
		void GetTransform(CAssetsManager* pAssetsManager, float Time, matrix2x2* pMatrix, vec2* pPosition) const;
		void GetDrawState(CAssetsManager* pAssetsManager, float Time, vec4* pColor, int* pState) const;
	};

/* MEMBERS ************************************************************/
private:	
	CAssetPath m_ImagePath;
	array<CQuad> m_Quads;

/* FUNCTIONS **********************************************************/
public:
	CAsset_MapLayerQuads();
	
	inline bool IsValidQuad(CSubPath SubPath) const
	{
		return (SubPath.GetType() == CSubPath::TYPE_QUAD && SubPath.GetId() >= 0 && SubPath.GetId() < m_Quads.size());
	}
	inline bool IsValidQuadVertex(CSubPath SubPath) const
	{
		return (
			SubPath.GetType() == CSubPath::TYPE_QUAD &&
			SubPath.GetId() >= 0 &&
			SubPath.GetId() < m_Quads.size() &&
			SubPath.GetId2() >= CSubPath::POINT_VERTEX0 &&
			SubPath.GetId2() <= CSubPath::POINT_VERTEX3
		);
	}
	
	inline CAssetPath GetImagePath() const { return m_ImagePath; }
	inline vec2 GetQuadPivot(CSubPath SubPath) const { return m_Quads[SubPath.GetId()].m_Pivot; }
	inline vec2 GetQuadSize(CSubPath SubPath) const { return m_Quads[SubPath.GetId()].m_Size; }
	inline float GetQuadAngle(CSubPath SubPath) const { return m_Quads[SubPath.GetId()].m_Angle; }
	inline CAssetPath GetQuadAnimationPath(CSubPath SubPath) const { return m_Quads[SubPath.GetId()].m_AnimationPath; }
	inline vec2 GetVertexPosition(CSubPath SubPath) const { return m_Quads[SubPath.GetId()].m_Vertices[SubPath.GetId2()-CSubPath::POINT_VERTEX0]; }
	inline vec2 GetVertexTexture(CSubPath SubPath) const { return m_Quads[SubPath.GetId()].m_UVs[SubPath.GetId2()-CSubPath::POINT_VERTEX0]; }
	inline vec4 GetVertexColor(CSubPath SubPath) const { return m_Quads[SubPath.GetId()].m_Colors[SubPath.GetId2()-CSubPath::POINT_VERTEX0]; }
	
	inline void CopyQuad(CSubPath SubPath, CQuad* pQuad) const { *pQuad = m_Quads[SubPath.GetId()]; }
	inline const CQuad* GetQuadsPointer() const { return &m_Quads[0]; }
	inline const CQuad* GetQuadPointer(CSubPath SubPath) const { return &m_Quads[SubPath.GetId()]; }
	inline int GetNumQuads() const { return m_Quads.size(); }
	
	inline void SetImagePath(CAssetPath Value) { m_ImagePath = Value; }
	inline void SetQuadPivot(CSubPath SubPath, vec2 Value) { if(IsValidQuad(SubPath)) m_Quads[SubPath.GetId()].m_Pivot = Value; }
	inline void SetQuadSize(CSubPath SubPath, vec2 Value) { if(IsValidQuad(SubPath)) m_Quads[SubPath.GetId()].m_Size = Value; }
	inline void SetQuadAngle(CSubPath SubPath, float Value) { if(IsValidQuad(SubPath)) m_Quads[SubPath.GetId()].m_Angle = Value; }
	inline void SetQuadAnimationPath(CSubPath SubPath, CAssetPath Value) { if(IsValidQuad(SubPath)) m_Quads[SubPath.GetId()].m_AnimationPath = Value; }
	inline void SetVertexPosition(CSubPath SubPath, vec2 Value)
	{
		if(IsValidQuadVertex(SubPath))
			m_Quads[SubPath.GetId()].m_Vertices[SubPath.GetId2()-CSubPath::POINT_VERTEX0] = Value;
	}
	inline void SetVertexTexture(CSubPath SubPath, vec2 Value)
	{
		if(IsValidQuadVertex(SubPath))
			m_Quads[SubPath.GetId()].m_UVs[SubPath.GetId2()-CSubPath::POINT_VERTEX0] = Value;
	}
	inline void SetVertexColor(CSubPath SubPath, vec4 Value)
	{
		if(IsValidQuadVertex(SubPath))
			m_Quads[SubPath.GetId()].m_Colors[SubPath.GetId2()-CSubPath::POINT_VERTEX0] = Value;
	}
	
	inline CSubPath NewQuad() { m_Quads.add(CQuad()); return CSubPath::Quad(m_Quads.size()-1); }
	
	void GetQuadTransform(CSubPath SubPath, float Time, matrix2x2* pMatrix, vec2* pPosition);
	void GetQuadDrawState(CSubPath SubPath, float Time, vec4* pColor, int* pState);
	
/* GET/SET ************************************************************/
public:
	enum
	{
		IMAGEPATH = CAsset::NUM_MEMBERS, //Path
		QUAD_POSITION_X, //Float
		QUAD_POSITION_Y, //Float
		QUAD_SIZE_X, //Float
		QUAD_SIZE_Y, //Float
		QUAD_ANGLE, //Float
		QUAD_POINT_POSITION_X, //Float
		QUAD_POINT_POSITION_Y, //Float
		QUAD_POINT_UV_X, //Float
		QUAD_POINT_UV_Y, //Float
		QUAD_POINT_COLOR, //Vec4
		QUAD_ANIMATIONPATH, //Asset
	};
	
	TU_ASSET_ARRAY_ITERATOR(Quad, m_Quads, Quad)
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		m_ImagePath.OnIdDeleted(Path);
		
		for(int i=0; i<m_Quads.size(); i++)
			m_Quads[i].m_AnimationPath.OnIdDeleted(Path);
	}
	
	int AddSubItem(int SubItemType)
	{
		switch(SubItemType)
		{
			case CSubPath::TYPE_QUAD:
				return NewQuad();
		}
		
		return CSubPath::Null();
	}
};

}

#endif
