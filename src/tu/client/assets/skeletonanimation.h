#ifndef TU_CLIENT_ASSETS_SKELETONANIMATION_H
#define TU_CLIENT_ASSETS_SKELETONANIMATION_H

#include <tu/client/assets.h>

#define TU_SKELETONANIMATION_TIMESTEP 30

namespace tu
{

class CAsset_SkeletonAnimation : public CAsset
{
public:
	static const int TypeId = CAssetPath::TYPE_SKELETONANIMATION;

/* IO *****************************************************************/
public:
	struct CStorageType : public CAsset::CStorageType
	{
		struct CBoneKeyFrame
		{
			int m_Time;
			float m_TranslationX;
			float m_TranslationY;
			float m_ScaleX;
			float m_ScaleY;
			float m_Angle;
			int m_Alignment;
		};
		
		struct CBoneAnimation
		{
			int m_BonePath;
			int m_CycleType;
			int m_FirstKeyFrame;
			int m_NumKeyFrames;
		};
		
		struct CLayerKeyFrame
		{
			int m_Time;
			int m_Color;
			int m_State;
		};
		
		struct CLayerAnimation
		{
			int m_LayerPath;
			int m_CycleType;
			int m_FirstKeyFrame;
			int m_NumKeyFrames;
		};
		
		int m_SkeletonPath;
		
		int m_NumBoneKeyFrames;
		int m_NumBoneAnimations;
		int m_NumLayerKeyFrames;
		int m_NumLayerAnimations;
		
		int m_BoneKeyFramesData;
		int m_BoneAnimationsData;
		int m_LayerKeyFramesData;
		int m_LayerAnimationsData;
		
	};
	
	void InitFromAssetsFile(class CAssetsManager* pAssetsManager, class tu::IAssetsFile* pAssetsFile, const CStorageType* pItem);
	void SaveInAssetsFile(class CDataFileWriter* pFileWriter, int Position);
	
/* SUBITEMS ***********************************************************/
public:
	class CSubPath : public CGenericPath<2, 0, 0, 16>
	{
	public:
		enum
		{
			TYPE_BONEANIMATION=0,
			TYPE_BONEKEYFRAME,
			TYPE_LAYERANIMATION,
			TYPE_LAYERKEYFRAME,
			NUM_TYPES,
		};
		
	public:
		CSubPath() : CGenericPath() { }
		CSubPath(int PathInt) : CGenericPath(PathInt) { }
		CSubPath(int Type, int Id, int Id2) : CGenericPath(Type, 0, 0x0, Id, Id2) { }
		
		static inline CSubPath Null() { return CSubPath(CGenericPath::UNDEFINED); }
		static inline CSubPath BoneAnimation(int Id) { return CSubPath(TYPE_BONEANIMATION, Id, 0); }
		static inline CSubPath BoneKeyFrame(int Id, int Id2) { return CSubPath(TYPE_BONEKEYFRAME, Id, Id2); }
		static inline CSubPath LayerAnimation(int Id) { return CSubPath(TYPE_LAYERANIMATION, Id, 0); }
		static inline CSubPath LayerKeyFrame(int Id, int Id2) { return CSubPath(TYPE_LAYERKEYFRAME, Id, Id2); }
	};

	enum
	{
		CYCLETYPE_CLAMP = 0,
		CYCLETYPE_LOOP,
		NUM_CYCLETYPES,
	};
	
	enum
	{
		LAYERSTATE_VISIBLE = 0,
		LAYERSTATE_HIDDEN,
		NUM_LAYERSTATES,
	};
	
	enum
	{
		BONEALIGN_PARENTBONE = 0,
		BONEALIGN_WORLD,
		BONEALIGN_AIM,
		BONEALIGN_MOTION,
		BONEALIGN_HOOK,
		NUM_BONEALIGNS,
	};

	class CBoneAnimation
	{
	public:
		class CFrame
		{
		public:
			vec2 m_Translation;
			vec2 m_Scale;
			float m_Angle;
			int m_Alignment;
			
			CFrame() :
				m_Translation(vec2(0.0f, 0.0f)),
				m_Scale(vec2(1.0f, 1.0f)),
				m_Angle(0),
				m_Alignment(BONEALIGN_PARENTBONE)
			{
				
			}
		};
		
		class CKeyFrame : public CFrame
		{
		public:
			int m_Time; // 1 = 1/50 sec
			
			CKeyFrame& Angle(float v)
			{
				m_Angle = v;
				return *this;
			}
			
			CKeyFrame& Translation(vec2 v)
			{
				m_Translation = v;
				return *this;
			}
			
			CKeyFrame& Alignment(int v)
			{
				m_Alignment = v;
				return *this;
			}
		};
		
		CAsset_Skeleton::CBonePath m_BonePath;
		array<CKeyFrame> m_KeyFrames;
		int m_CycleType;
		
		CBoneAnimation() :
			m_CycleType(CYCLETYPE_CLAMP),
			m_BonePath(CAsset_Skeleton::CBonePath::Null())
		{
			
		}
		
		inline int IntTimeToKeyFrame(int IntTime) const
		{
			if(m_KeyFrames.size() == 0)
				return 0;
			
			int i;
			for(i=0; i<m_KeyFrames.size(); i++)
			{
				if(m_KeyFrames[i].m_Time > IntTime)
					break;
			}
			
			return i;
		}
		
		inline int TimeToKeyFrame(float Time) const
		{
			return IntTimeToKeyFrame(Time*TU_SKELETONANIMATION_TIMESTEP);
		}

		float GetDuration() const
		{
			if(m_KeyFrames.size())
				return m_KeyFrames[m_KeyFrames.size()-1].m_Time/static_cast<float>(TU_SKELETONANIMATION_TIMESTEP);
			else
				return 1.0f;
		}

		inline bool GetFrame(float Time, CFrame* pFrame) const
		{
			float CycleTime = Time;
			if(m_CycleType == CYCLETYPE_LOOP)
			{
				CycleTime = fmod(Time, GetDuration());
			}
			
			int i = TimeToKeyFrame(CycleTime);
			
			if(i == m_KeyFrames.size())
			{
				if(i == 0)
					return false;
				else
					*pFrame = m_KeyFrames[m_KeyFrames.size()-1];
			}
			else if(i == 0)
			{
				*pFrame = m_KeyFrames[0];
			}
			else
			{
				float alpha = (TU_SKELETONANIMATION_TIMESTEP*CycleTime - m_KeyFrames[i-1].m_Time) / (m_KeyFrames[i].m_Time - m_KeyFrames[i-1].m_Time);
				pFrame->m_Translation = mix(m_KeyFrames[i-1].m_Translation, m_KeyFrames[i].m_Translation, alpha);
				pFrame->m_Scale = mix(m_KeyFrames[i-1].m_Scale, m_KeyFrames[i].m_Scale, alpha);
				pFrame->m_Angle = mix(m_KeyFrames[i-1].m_Angle, m_KeyFrames[i].m_Angle, alpha); //Need better interpolation
				pFrame->m_Alignment = m_KeyFrames[i-1].m_Alignment;
			}
			
			return true;
		}
	};

	class CLayerAnimation
	{
	public:
		class CFrame
		{
		public:
			vec4 m_Color;
			int m_State;
			
			CFrame() :
				m_Color(vec4(1.0f, 1.0f, 1.0f, 1.0f)),
				m_State(0)
			{
				
			}
		};
		
		class CKeyFrame : public CFrame
		{
		public:
			int m_Time; // 1 = 1/50 sec
			
			CKeyFrame& State(int v)
			{
				m_State = v;
				return *this;
			}
			
			CKeyFrame& Color(vec4 v)
			{
				m_Color = v;
				return *this;
			}
		};
		
		CAsset_Skeleton::CBonePath m_LayerPath;
		array<CKeyFrame> m_KeyFrames;
		int m_CycleType;
		
		CLayerAnimation() :
			m_CycleType(CYCLETYPE_CLAMP),
			m_LayerPath(CAsset_Skeleton::CBonePath::Null())
		{
			
		}
		
		inline int IntTimeToKeyFrame(int IntTime) const
		{
			if(m_KeyFrames.size() == 0)
				return 0;
			
			int i;
			for(i=0; i<m_KeyFrames.size(); i++)
			{
				if(m_KeyFrames[i].m_Time > IntTime)
					break;
			}
			
			return i;
		}
		
		inline int TimeToKeyFrame(float Time) const
		{
			return IntTimeToKeyFrame(Time*TU_SKELETONANIMATION_TIMESTEP);
		}

		float GetDuration() const
		{
			if(m_KeyFrames.size())
				return m_KeyFrames[m_KeyFrames.size()-1].m_Time/static_cast<float>(TU_SKELETONANIMATION_TIMESTEP);
			else
				return 1.0f;
		}

		inline bool GetFrame(float Time, CFrame* pFrame) const
		{
			float CycleTime = Time;
			if(m_CycleType == CYCLETYPE_LOOP)
			{
				CycleTime = fmod(Time, GetDuration());
			}
			
			int i = TimeToKeyFrame(CycleTime);
			
			if(i == m_KeyFrames.size())
			{
				if(i == 0)
					return false;
				else
					*pFrame = m_KeyFrames[m_KeyFrames.size()-1];
			}
			else if(i == 0)
			{
				*pFrame = m_KeyFrames[0];
			}
			else
			{
				float alpha = (TU_SKELETONANIMATION_TIMESTEP*CycleTime - m_KeyFrames[i-1].m_Time) / (m_KeyFrames[i].m_Time - m_KeyFrames[i-1].m_Time);
				pFrame->m_Color = mix(m_KeyFrames[i-1].m_Color, m_KeyFrames[i].m_Color, alpha); //Need better interpolation
				pFrame->m_State = m_KeyFrames[i].m_State;
			}
			
			return true;
		}
	};
	
	inline int GetKeyFramePath(int i)
	{
		return i;
	}
	
/* MEMBERS ************************************************************/
public:	
	CAssetPath m_SkeletonPath;
	array<CBoneAnimation> m_BoneAnimations;
	array<CLayerAnimation> m_LayerAnimations;

/* FUNCTIONS **********************************************************/
public:
	CAsset_SkeletonAnimation();
	
	CBoneAnimation::CKeyFrame& AddBoneKeyFrame(CAsset_Skeleton::CBonePath BonePath, int Time);
	CLayerAnimation::CKeyFrame& AddLayerKeyFrame(CAsset_Skeleton::CBonePath LayerPath, int Time);
	
	void SetCycle(CAsset_Skeleton::CBonePath BonePath, int CycleType);
	
	CSubPath GetBoneKeyFramePath(CAsset_Skeleton::CBonePath BonePath, int IntTime);
	CSubPath GetBoneKeyFramePath(CAsset_SkeletonAnimation::CSubPath SubPath, int IntTime);
	CSubPath GetLayerKeyFramePath(CAsset_Skeleton::CBonePath LayerPath, int IntTime);
	CSubPath GetLayerKeyFramePath(CAsset_SkeletonAnimation::CSubPath SubPath, int IntTime);

/* GET/SET ************************************************************/
public:
	enum
	{
		SKELETONPATH = CAsset::NUM_MEMBERS, //AssetPath
		BONEANIMATION_CYCLETYPE, //Int
		BONEKEYFRAME_TIME, //Int
		BONEKEYFRAME_TRANSLATION_X, //Float
		BONEKEYFRAME_TRANSLATION_Y, //Float
		BONEKEYFRAME_SCALE_X, //Float
		BONEKEYFRAME_SCALE_Y, //Float
		BONEKEYFRAME_ANGLE, //Float
		BONEKEYFRAME_ALIGNMENT, //Int
		LAYERKEYFRAME_TIME, //Int
		LAYERKEYFRAME_COLOR, //Color
		LAYERKEYFRAME_STATE, //Int
	};
	
	TU_ASSET_GETSET_FUNC()
	
/* EDITOR *************************************************************/
public:
	void OnAssetDeleted(const CAssetPath& Path)
	{
		m_SkeletonPath.OnIdDeleted(Path);
	}
	
	bool DeleteSubItem(CSubPath SubItemPath)
	{
		switch(SubItemPath.GetType())
		{
			case CSubPath::TYPE_BONEKEYFRAME:
			{
				int animId = SubItemPath.GetId();
				if(animId >= 0 && animId < m_BoneAnimations.size())
				{
					int keyId = SubItemPath.GetId2();
					if(keyId >= 0 && keyId < m_BoneAnimations[animId].m_KeyFrames.size())
					{
						m_BoneAnimations[animId].m_KeyFrames.remove_index(keyId);
						if(m_BoneAnimations.size() == 0)
						{
							m_BoneAnimations.remove_index(animId);
						}
					}
				}
				return true;
			}
			case CSubPath::TYPE_LAYERKEYFRAME:
			{
				int animId = SubItemPath.GetId();
				if(animId >= 0 && animId < m_LayerAnimations.size())
				{
					int keyId = SubItemPath.GetId2();
					if(keyId >= 0 && keyId < m_LayerAnimations[animId].m_KeyFrames.size())
					{
						m_LayerAnimations[animId].m_KeyFrames.remove_index(keyId);
						if(m_LayerAnimations.size() == 0)
						{
							m_LayerAnimations.remove_index(animId);
						}
					}
				}
				return true;
			}
		}
		
		return false;
	}
	
	void OnSubItemDeleted(const CAssetPath& Path, int SubPathInt)
	{
		//TODO Check for bones and layers
	}
	
/* EDITOR *************************************************************/
public:
};

}

#endif
