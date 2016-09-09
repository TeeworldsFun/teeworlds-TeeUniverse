#include "skeleton.h"

#include <engine/shared/datafile.h>
#include <tu/client/graphics.h>

namespace tu
{

CAsset_SkeletonAnimation::CAsset_SkeletonAnimation()
{
	
}
	
CAsset_SkeletonAnimation::CBoneAnimation::CKeyFrame& CAsset_SkeletonAnimation::AddBoneKeyFrame(CAsset_Skeleton::CSubPath BonePath, int Time)
{
	//Find BoneAnimation
	int BoneAnimationId = -1;
	for(int i=0; i<m_BoneAnimations.size(); i++)
	{
		if(m_BoneAnimations[i].m_BonePath == BonePath)
		{
			BoneAnimationId = i;
			break;
		}
	}
	if(BoneAnimationId < 0)
	{
		BoneAnimationId = m_BoneAnimations.size();
		m_BoneAnimations.set_size(m_BoneAnimations.size()+1);
		m_BoneAnimations[BoneAnimationId].m_BonePath = BonePath;
	}
	
	CAsset_SkeletonAnimation::CBoneAnimation::CKeyFrame KeyFrame;
	m_BoneAnimations[BoneAnimationId].GetFrame(Time/static_cast<float>(TU_SKELETONANIMATION_TIMESTEP), &KeyFrame);
	KeyFrame.m_Time = Time;
	
	int KeyFrameId = m_BoneAnimations[BoneAnimationId].IntTimeToKeyFrame(Time);
	if(KeyFrameId == m_BoneAnimations[BoneAnimationId].m_KeyFrames.size())
	{
		m_BoneAnimations[BoneAnimationId].m_KeyFrames.add(KeyFrame);
	}
	else
	{
		m_BoneAnimations[BoneAnimationId].m_KeyFrames.insertat(KeyFrame, KeyFrameId);
	}
	return m_BoneAnimations[BoneAnimationId].m_KeyFrames[KeyFrameId];
}

CAsset_SkeletonAnimation::CLayerAnimation::CKeyFrame& CAsset_SkeletonAnimation::AddLayerKeyFrame(CAsset_Skeleton::CSubPath LayerPath, int Time)
{
	//Find BoneAnimation
	int LayerAnimationId = -1;
	for(int i=0; i<m_LayerAnimations.size(); i++)
	{
		if(m_LayerAnimations[i].m_LayerPath == LayerPath)
		{
			LayerAnimationId = i;
			break;
		}
	}
	if(LayerAnimationId < 0)
	{
		LayerAnimationId = m_LayerAnimations.size();
		m_LayerAnimations.set_size(m_LayerAnimations.size()+1);
		m_LayerAnimations[LayerAnimationId].m_LayerPath = LayerPath;
	}
	
	CAsset_SkeletonAnimation::CLayerAnimation::CKeyFrame KeyFrame;
	m_LayerAnimations[LayerAnimationId].GetFrame(Time/static_cast<float>(TU_SKELETONANIMATION_TIMESTEP), &KeyFrame);
	KeyFrame.m_Time = Time;
	
	int KeyFrameId = m_LayerAnimations[LayerAnimationId].IntTimeToKeyFrame(Time);
	if(KeyFrameId == m_LayerAnimations[LayerAnimationId].m_KeyFrames.size())
	{
		m_LayerAnimations[LayerAnimationId].m_KeyFrames.add(KeyFrame);
	}
	else
	{
		m_LayerAnimations[LayerAnimationId].m_KeyFrames.insertat(KeyFrame, KeyFrameId);
	}
	
	return m_LayerAnimations[LayerAnimationId].m_KeyFrames[KeyFrameId];
}

void CAsset_SkeletonAnimation::SetCycle(CAsset_Skeleton::CSubPath BonePath, int CycleType)
{
	//Find BoneAnimation
	for(int i=0; i<m_BoneAnimations.size(); i++)
	{
		if(m_BoneAnimations[i].m_BonePath == BonePath)
		{
			m_BoneAnimations[i].m_CycleType = CycleType;
			return;
		}
	}
}

CAsset_SkeletonAnimation::CSubPath CAsset_SkeletonAnimation::GetBoneKeyFramePath(CAsset_Skeleton::CSubPath BonePath, int IntTime)
{
	//Find BoneAnimation
	for(int i=0; i<m_BoneAnimations.size(); i++)
	{
		if(m_BoneAnimations[i].m_BonePath == BonePath)
		{
			for(int j=0; j<m_BoneAnimations[i].m_KeyFrames.size(); j++)
			{
				if(m_BoneAnimations[i].m_KeyFrames[j].m_Time == IntTime)
				{
					return CSubPath::BoneKeyFrame(i, j);
				}
			}
			return CSubPath::Null();
		}
	}
	return CSubPath::Null();
}

CAsset_SkeletonAnimation::CSubPath CAsset_SkeletonAnimation::GetBoneKeyFramePath(CAsset_SkeletonAnimation::CSubPath SubPath, int IntTime)
{
	if(SubPath.GetType() != CAsset_SkeletonAnimation::CSubPath::TYPE_BONEANIMATION)
		return CSubPath::Null();
		
	int animId = SubPath.GetId();
	if(animId < 0 || animId >= m_BoneAnimations.size())
		return CSubPath::Null();
	
	for(int i=0; i<m_BoneAnimations[animId].m_KeyFrames.size(); i++)
	{
		if(m_BoneAnimations[animId].m_KeyFrames[i].m_Time == IntTime)
		{
			return CSubPath::BoneKeyFrame(animId, i);
		}
	}
	return CSubPath::Null();
}

CAsset_SkeletonAnimation::CSubPath CAsset_SkeletonAnimation::GetLayerKeyFramePath(CAsset_Skeleton::CSubPath LayerPath, int IntTime)
{
	//Find BoneAnimation
	for(int i=0; i<m_LayerAnimations.size(); i++)
	{
		if(m_LayerAnimations[i].m_LayerPath == LayerPath)
		{
			for(int j=0; j<m_LayerAnimations[i].m_KeyFrames.size(); j++)
			{
				if(m_LayerAnimations[i].m_KeyFrames[j].m_Time == IntTime)
				{
					return CSubPath::LayerKeyFrame(i, j);
				}
			}
			return CSubPath::Null();
		}
	}
	return CSubPath::Null();
}

CAsset_SkeletonAnimation::CSubPath CAsset_SkeletonAnimation::GetLayerKeyFramePath(CAsset_SkeletonAnimation::CSubPath SubPath, int IntTime)
{
	if(SubPath.GetType() != CAsset_SkeletonAnimation::CSubPath::TYPE_LAYERANIMATION)
		return CSubPath::Null();
		
	int animId = SubPath.GetId();
	if(animId < 0 || animId >= m_LayerAnimations.size())
		return CSubPath::Null();
	
	for(int i=0; i<m_LayerAnimations[animId].m_KeyFrames.size(); i++)
	{
		if(m_LayerAnimations[animId].m_KeyFrames[i].m_Time == IntTime)
		{
			return CSubPath::LayerKeyFrame(animId, i);
		}
	}
	return CSubPath::Null();
}

/* IO *****************************************************************/

void CAsset_SkeletonAnimation::InitFromAssetsFile(tu::IAssetsFile* pAssetsFile, const CStorageType* pItem)
{
	// copy name
	SetName((char *)pAssetsFile->GetData(pItem->m_Name));
				
	// copy info
	m_SkeletonPath = pItem->m_SkeletonPath;
	
	// load bone animations
	const CStorageType::CBoneKeyFrame* pBoneKeyFrames = static_cast<CStorageType::CBoneKeyFrame*>(pAssetsFile->GetData(pItem->m_BoneKeyFramesData));
	const CStorageType::CBoneAnimation* pBoneAnimations = static_cast<CStorageType::CBoneAnimation*>(pAssetsFile->GetData(pItem->m_BoneAnimationsData));
	
	for(int i=0; i<pItem->m_NumBoneAnimations; i++)
	{
		m_BoneAnimations.add(CBoneAnimation());
		CBoneAnimation& Animation = m_BoneAnimations[m_BoneAnimations.size()-1];
		Animation.m_BonePath = pBoneAnimations[i].m_BonePath;
		Animation.m_CycleType = pBoneAnimations[i].m_CycleType;
		
		if(pBoneAnimations[i].m_FirstKeyFrame < pItem->m_NumBoneKeyFrames)
		{
			int NumKeyFrames = min(pItem->m_NumBoneKeyFrames - pBoneAnimations[i].m_FirstKeyFrame, pBoneAnimations[i].m_NumKeyFrames);
			for(int j=0; j<NumKeyFrames; j++)
			{
				Animation.m_KeyFrames.add(CBoneAnimation::CKeyFrame());
				CBoneAnimation::CKeyFrame& KeyFrame = Animation.m_KeyFrames[Animation.m_KeyFrames.size()-1];
				
				int KeyId = pBoneAnimations[i].m_FirstKeyFrame+j;
				KeyFrame.m_Time = pBoneKeyFrames[KeyId].m_Time;
				KeyFrame.m_Translation = vec2(pBoneKeyFrames[KeyId].m_TranslationX, pBoneKeyFrames[KeyId].m_TranslationY);
				KeyFrame.m_Scale = vec2(pBoneKeyFrames[KeyId].m_ScaleX, pBoneKeyFrames[KeyId].m_ScaleY);
				KeyFrame.m_Angle = pBoneKeyFrames[KeyId].m_Angle;
				KeyFrame.m_Alignment = pBoneKeyFrames[KeyId].m_Alignment;
			}
		}
	}
	
	// load layer animations
	const CStorageType::CLayerKeyFrame* pLayerKeyFrames = static_cast<CStorageType::CLayerKeyFrame*>(pAssetsFile->GetData(pItem->m_LayerKeyFramesData));
	const CStorageType::CLayerAnimation* pLayerAnimations = static_cast<CStorageType::CLayerAnimation*>(pAssetsFile->GetData(pItem->m_LayerAnimationsData));
	for(int i=0; i<pItem->m_NumLayerAnimations; i++)
	{
		m_LayerAnimations.add(CLayerAnimation());
		CLayerAnimation& Animation = m_LayerAnimations[m_LayerAnimations.size()-1];
		
		Animation.m_LayerPath = pLayerAnimations[i].m_LayerPath;
		Animation.m_CycleType = pLayerAnimations[i].m_CycleType;
		
		if(pLayerAnimations[i].m_FirstKeyFrame < pItem->m_NumLayerKeyFrames)
		{
			int NumKeyFrames = min(pItem->m_NumLayerKeyFrames - pLayerAnimations[i].m_FirstKeyFrame, pLayerAnimations[i].m_NumKeyFrames);
			for(int j=0; j<NumKeyFrames; j++)
			{
				Animation.m_KeyFrames.add(CLayerAnimation::CKeyFrame());
				CLayerAnimation::CKeyFrame& KeyFrame = Animation.m_KeyFrames[Animation.m_KeyFrames.size()-1];
				
				int KeyId = pLayerAnimations[i].m_FirstKeyFrame+j;
				KeyFrame.m_Time = pBoneKeyFrames[KeyId].m_Time;
				KeyFrame.m_Color = tu::IntToColor(pLayerKeyFrames[KeyId].m_Color);
				KeyFrame.m_State = pLayerKeyFrames[KeyId].m_State;
			}
		}
	}
}

void CAsset_SkeletonAnimation::SaveInAssetsFile(CDataFileWriter* pFileWriter, int Position)
{
	CStorageType Item;
	Item.m_Name = pFileWriter->AddData(str_length(m_aName)+1, m_aName);

	Item.m_SkeletonPath = m_SkeletonPath;
	
	{
		int KeyFramesIter = 0;
		for(int i=0; i<m_BoneAnimations.size(); i++)
		{
			KeyFramesIter += m_BoneAnimations[i].m_KeyFrames.size();
		}
		
		Item.m_NumBoneKeyFrames = KeyFramesIter;
		Item.m_NumBoneAnimations = m_BoneAnimations.size();
		
		CStorageType::CBoneKeyFrame* pKeyFrames = new CStorageType::CBoneKeyFrame[Item.m_NumBoneKeyFrames];
		CStorageType::CBoneAnimation* pAnimations = new CStorageType::CBoneAnimation[Item.m_NumBoneAnimations];
		
		KeyFramesIter = 0;
		for(int i=0; i<m_BoneAnimations.size(); i++)
		{
			pAnimations[i].m_BonePath = m_BoneAnimations[i].m_BonePath;
			pAnimations[i].m_CycleType = m_BoneAnimations[i].m_CycleType;
			
			pAnimations[i].m_FirstKeyFrame = KeyFramesIter;
			pAnimations[i].m_NumKeyFrames = m_BoneAnimations[i].m_KeyFrames.size();
			
			for(int j=0; j<m_BoneAnimations[i].m_KeyFrames.size(); j++)
			{
				pKeyFrames[KeyFramesIter].m_Time = m_BoneAnimations[i].m_KeyFrames[j].m_Time;
				pKeyFrames[KeyFramesIter].m_TranslationX = m_BoneAnimations[i].m_KeyFrames[j].m_Translation.x;
				pKeyFrames[KeyFramesIter].m_TranslationY = m_BoneAnimations[i].m_KeyFrames[j].m_Translation.y;
				pKeyFrames[KeyFramesIter].m_ScaleX = m_BoneAnimations[i].m_KeyFrames[j].m_Scale.x;
				pKeyFrames[KeyFramesIter].m_ScaleY = m_BoneAnimations[i].m_KeyFrames[j].m_Scale.y;
				pKeyFrames[KeyFramesIter].m_Angle = m_BoneAnimations[i].m_KeyFrames[j].m_Angle;
				pKeyFrames[KeyFramesIter].m_Alignment = m_BoneAnimations[i].m_KeyFrames[j].m_Alignment;
				
				KeyFramesIter++;
			}
		}
		
		Item.m_BoneKeyFramesData = pFileWriter->AddData(Item.m_NumBoneKeyFrames * sizeof(CStorageType::CBoneKeyFrame), pKeyFrames);
		Item.m_BoneAnimationsData = pFileWriter->AddData(Item.m_NumBoneAnimations * sizeof(CStorageType::CBoneAnimation), pAnimations);
		
		delete[] pKeyFrames;
		delete[] pAnimations;
	}
	
	{
		int KeyFramesIter = 0;
		for(int i=0; i<m_LayerAnimations.size(); i++)
		{
			KeyFramesIter += m_LayerAnimations[i].m_KeyFrames.size();
		}
		
		Item.m_NumLayerKeyFrames = KeyFramesIter;
		Item.m_NumLayerAnimations = m_LayerAnimations.size();
		
		CStorageType::CLayerKeyFrame* pKeyFrames = new CStorageType::CLayerKeyFrame[Item.m_NumLayerKeyFrames];
		CStorageType::CLayerAnimation* pAnimations = new CStorageType::CLayerAnimation[Item.m_NumLayerAnimations];
		
		KeyFramesIter = 0;
		for(int i=0; i<m_LayerAnimations.size(); i++)
		{
			pAnimations[i].m_LayerPath = m_LayerAnimations[i].m_LayerPath;
			pAnimations[i].m_CycleType = m_LayerAnimations[i].m_CycleType;
			
			pAnimations[i].m_FirstKeyFrame = KeyFramesIter;
			pAnimations[i].m_NumKeyFrames = m_LayerAnimations[i].m_KeyFrames.size();
			
			for(int j=0; j<m_LayerAnimations[i].m_KeyFrames.size(); j++)
			{
				pKeyFrames[KeyFramesIter].m_Time = m_LayerAnimations[i].m_KeyFrames[j].m_Time;
				pKeyFrames[KeyFramesIter].m_Color = ColorToInt(m_LayerAnimations[i].m_KeyFrames[j].m_Color);
				pKeyFrames[KeyFramesIter].m_State = m_LayerAnimations[i].m_KeyFrames[j].m_State;
				
				KeyFramesIter++;
			}
		}
		
		Item.m_LayerKeyFramesData = pFileWriter->AddData(Item.m_NumLayerKeyFrames * sizeof(CStorageType::CLayerKeyFrame), pKeyFrames);
		Item.m_LayerAnimationsData = pFileWriter->AddData(Item.m_NumLayerAnimations * sizeof(CStorageType::CLayerAnimation), pAnimations);
		
		delete[] pKeyFrames;
		delete[] pAnimations;
	}
	
	pFileWriter->AddItem(CAssetPath::TypeToStoredType(TypeId), Position, sizeof(CStorageType), &Item);
}

/* VALUE INT **********************************************************/

template<>
int CAsset_SkeletonAnimation::GetValue<int>(int ValueType, int Path, int DefaultValue)
{
	CAsset_SkeletonAnimation::CSubPath SubPath(Path);
	
	switch(ValueType)
	{
		case BONEANIMATION_CYCLETYPE:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_BONEANIMATION)
			{
				int animId = SubPath.GetId();
				if(animId >= 0 && animId < m_BoneAnimations.size())
				{
					return m_BoneAnimations[animId].m_CycleType;
				}
			}
			break;
		case BONEKEYFRAME_TIME:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_BONEKEYFRAME)
			{
				int animId = SubPath.GetId();
				if(animId >= 0 && animId < m_BoneAnimations.size())
				{
					int keyId = SubPath.GetId2();
					if(keyId >= 0 && keyId < m_BoneAnimations[animId].m_KeyFrames.size())
					{
						return m_BoneAnimations[animId].m_KeyFrames[keyId].m_Time;
					}
				}
			}
			break;
		case BONEKEYFRAME_ALIGNMENT:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_BONEKEYFRAME)
			{
				int animId = SubPath.GetId();
				if(animId >= 0 && animId < m_BoneAnimations.size())
				{
					int keyId = SubPath.GetId2();
					if(keyId >= 0 && keyId < m_BoneAnimations[animId].m_KeyFrames.size())
					{
						return m_BoneAnimations[animId].m_KeyFrames[keyId].m_Alignment;
					}
				}
			}
			break;
		case LAYERKEYFRAME_TIME:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_LAYERKEYFRAME)
			{
				int animId = SubPath.GetId();
				if(animId >= 0 && animId < m_LayerAnimations.size())
				{
					int keyId = SubPath.GetId2();
					if(keyId >= 0 && keyId < m_LayerAnimations[animId].m_KeyFrames.size())
					{
						return m_LayerAnimations[animId].m_KeyFrames[keyId].m_Time;
					}
				}
			}
			break;
		case LAYERKEYFRAME_STATE:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_LAYERKEYFRAME)
			{
				int animId = SubPath.GetId();
				if(animId >= 0 && animId < m_LayerAnimations.size())
				{
					int keyId = SubPath.GetId2();
					if(keyId >= 0 && keyId < m_LayerAnimations[animId].m_KeyFrames.size())
					{
						return m_LayerAnimations[animId].m_KeyFrames[keyId].m_State;
					}
				}
			}
			break;
		default:
			return CAsset::GetValue<int>(ValueType, Path, DefaultValue);
	}
	
	return DefaultValue;
}

template<>
bool CAsset_SkeletonAnimation::SetValue<int>(int ValueType, int Path, int Value)
{
	CAsset_SkeletonAnimation::CSubPath SubPath(Path);
	
	switch(ValueType)
	{
		case BONEANIMATION_CYCLETYPE:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_BONEANIMATION)
			{
				int animId = SubPath.GetId();
				if(animId >= 0 && animId < m_BoneAnimations.size())
				{
					m_BoneAnimations[animId].m_CycleType = Value;
					return true;
				}
				return false;
			}
			else return false;
		case BONEKEYFRAME_TIME:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_BONEKEYFRAME)
			{
				int animId = SubPath.GetId();
				if(animId < 0 || animId >= m_BoneAnimations.size())
					return false;
					
				int keyId = SubPath.GetId2();
				if(keyId < 0 || keyId >= m_BoneAnimations[animId].m_KeyFrames.size())
					return false;
				
				if(
					(keyId > 0 && Value > m_BoneAnimations[animId].m_KeyFrames[keyId-1].m_Time) &&
					(keyId+1 < m_BoneAnimations[animId].m_KeyFrames.size() && Value < m_BoneAnimations[animId].m_KeyFrames[keyId+1].m_Time)
				)
				{
					m_BoneAnimations[animId].m_KeyFrames[keyId].m_Time = Value;
					return true;
				}
				else
				{
					for(int i=0; i<m_BoneAnimations[animId].m_KeyFrames.size(); i++)
					{
						if(m_BoneAnimations[animId].m_KeyFrames[i].m_Time == Value)
							return false;
					}
					
					CAsset_SkeletonAnimation::CBoneAnimation::CKeyFrame KeyFrame = m_BoneAnimations[animId].m_KeyFrames[keyId];
					KeyFrame.m_Time = Value;
					m_BoneAnimations[animId].m_KeyFrames.remove_index(keyId);
					
					keyId = m_BoneAnimations[animId].IntTimeToKeyFrame(Value);
					if(keyId == m_BoneAnimations[animId].m_KeyFrames.size())
					{
						m_BoneAnimations[animId].m_KeyFrames.add(KeyFrame);
					}
					else
					{
						m_BoneAnimations[animId].m_KeyFrames.insertat(KeyFrame, keyId);
					}
					
					return true;
				}
			}
			else return false;
		case BONEKEYFRAME_ALIGNMENT:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_BONEKEYFRAME)
			{
				if(Value >= NUM_BONEALIGNS)
					return false;
					
				int animId = SubPath.GetId();
				if(animId < 0 || animId >= m_BoneAnimations.size())
					return false;
					
				int keyId = SubPath.GetId2();
				if(keyId < 0 || keyId >= m_BoneAnimations[animId].m_KeyFrames.size())
					return false;
				
				m_BoneAnimations[animId].m_KeyFrames[keyId].m_Alignment = Value;
			}
			else return false;
		case LAYERKEYFRAME_TIME:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_LAYERKEYFRAME)
			{
				int animId = SubPath.GetId();
				if(animId < 0 || animId >= m_LayerAnimations.size())
					return false;
					
				int keyId = SubPath.GetId2();
				if(keyId < 0 || keyId >= m_LayerAnimations[animId].m_KeyFrames.size())
					return false;
				
				if(
					(keyId > 0 && Value > m_LayerAnimations[animId].m_KeyFrames[keyId-1].m_Time) &&
					(keyId+1 < m_LayerAnimations[animId].m_KeyFrames.size() && Value < m_LayerAnimations[animId].m_KeyFrames[keyId+1].m_Time)
				)
				{
					m_LayerAnimations[animId].m_KeyFrames[keyId].m_Time = Value;
					return true;
				}
				else
				{
					for(int i=0; i<m_LayerAnimations[animId].m_KeyFrames.size(); i++)
					{
						if(m_LayerAnimations[animId].m_KeyFrames[i].m_Time == Value)
							return false;
					}
					
					CAsset_SkeletonAnimation::CLayerAnimation::CKeyFrame KeyFrame = m_LayerAnimations[animId].m_KeyFrames[keyId];
					KeyFrame.m_Time = Value;
					m_LayerAnimations[animId].m_KeyFrames.remove_index(keyId);
					
					keyId = m_LayerAnimations[animId].IntTimeToKeyFrame(Value);
					if(keyId == m_LayerAnimations[animId].m_KeyFrames.size())
					{
						m_LayerAnimations[animId].m_KeyFrames.add(KeyFrame);
					}
					else
					{
						m_LayerAnimations[animId].m_KeyFrames.insertat(KeyFrame, keyId);
					}
					
					return true;
				}
			}
			else return false;
		case LAYERKEYFRAME_STATE:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_LAYERKEYFRAME)
			{
				if(Value >= NUM_LAYERSTATES)
					return false;
					
				int animId = SubPath.GetId();
				if(animId < 0 || animId >= m_LayerAnimations.size())
					return false;
					
				int keyId = SubPath.GetId2();
				if(keyId < 0 || keyId >= m_LayerAnimations[animId].m_KeyFrames.size())
					return false;
				
				m_LayerAnimations[animId].m_KeyFrames[keyId].m_State = Value;
			}
			else return false;
	}
	
	return CAsset::SetValue<int>(ValueType, Path, Value);
}

/* VALUE FLOAT ********************************************************/

template<>
float CAsset_SkeletonAnimation::GetValue<float>(int ValueType, int Path, float DefaultValue)
{
	CAsset_SkeletonAnimation::CSubPath SubPath(Path);
	
	switch(ValueType)
	{
		case BONEKEYFRAME_ANGLE:
		case BONEKEYFRAME_TRANSLATION_X:
		case BONEKEYFRAME_TRANSLATION_Y:
		case BONEKEYFRAME_SCALE_X:
		case BONEKEYFRAME_SCALE_Y:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_BONEKEYFRAME)
			{
				int animId = SubPath.GetId();
				if(animId >= 0 && animId < m_BoneAnimations.size())
				{
					int keyId = SubPath.GetId2();
					if(keyId >= 0 && keyId < m_BoneAnimations[animId].m_KeyFrames.size())
					{
						switch(ValueType)
						{
							case BONEKEYFRAME_ANGLE:
								return m_BoneAnimations[animId].m_KeyFrames[keyId].m_Angle;
							case BONEKEYFRAME_TRANSLATION_X:
								return m_BoneAnimations[animId].m_KeyFrames[keyId].m_Translation.x;
							case BONEKEYFRAME_TRANSLATION_Y:
								return m_BoneAnimations[animId].m_KeyFrames[keyId].m_Translation.y;
							case BONEKEYFRAME_SCALE_X:
								return m_BoneAnimations[animId].m_KeyFrames[keyId].m_Scale.x;
							case BONEKEYFRAME_SCALE_Y:
								return m_BoneAnimations[animId].m_KeyFrames[keyId].m_Scale.y;
						}
					}
				}
			}
			break;
		default:
			return CAsset::GetValue<float>(ValueType, Path, DefaultValue);
	}
	
	return DefaultValue;
}

template<>
bool CAsset_SkeletonAnimation::SetValue<float>(int ValueType, int Path, float Value)
{
	CAsset_SkeletonAnimation::CSubPath SubPath(Path);
	
	switch(ValueType)
	{
		case BONEKEYFRAME_ANGLE:
		case BONEKEYFRAME_TRANSLATION_X:
		case BONEKEYFRAME_TRANSLATION_Y:
		case BONEKEYFRAME_SCALE_X:
		case BONEKEYFRAME_SCALE_Y:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_BONEKEYFRAME)
			{
				int animId = SubPath.GetId();
				if(animId >= 0 && animId < m_BoneAnimations.size())
				{
					int keyId = SubPath.GetId2();
					if(keyId >= 0 && keyId < m_BoneAnimations[animId].m_KeyFrames.size())
					{
						switch(ValueType)
						{
							case BONEKEYFRAME_ANGLE:
								m_BoneAnimations[animId].m_KeyFrames[keyId].m_Angle = Value;
								return true;
							case BONEKEYFRAME_TRANSLATION_X:
								m_BoneAnimations[animId].m_KeyFrames[keyId].m_Translation.x = Value;
								return true;
							case BONEKEYFRAME_TRANSLATION_Y:
								m_BoneAnimations[animId].m_KeyFrames[keyId].m_Translation.y = Value;
								return true;
							case BONEKEYFRAME_SCALE_X:
								m_BoneAnimations[animId].m_KeyFrames[keyId].m_Scale.x = Value;
								return true;
							case BONEKEYFRAME_SCALE_Y:
								m_BoneAnimations[animId].m_KeyFrames[keyId].m_Scale.y = Value;
								return true;
						}
					}
					else return false;
				}
				else return false;
			}
			break;
	}
	
	return CAsset::SetValue<int>(ValueType, Path, Value);
}

/* VALUE VEC4 *********************************************************/
	
template<>
vec4 CAsset_SkeletonAnimation::GetValue<vec4>(int ValueType, int PathInt, vec4 DefaultValue)
{
	CAsset_SkeletonAnimation::CSubPath SubPath(PathInt);
	switch(ValueType)
	{
		case LAYERKEYFRAME_COLOR:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_LAYERKEYFRAME)
			{
				int animId = SubPath.GetId();
				if(animId >= 0 && animId < m_LayerAnimations.size())
				{
					int keyId = SubPath.GetId2();
					if(keyId >= 0 && keyId < m_LayerAnimations[animId].m_KeyFrames.size())
					{
						return m_LayerAnimations[animId].m_KeyFrames[keyId].m_Color;
					}
				}
			}
		default:
			return CAsset::GetValue<vec4>(ValueType, PathInt, DefaultValue);
	}
	
	return DefaultValue;
}
	
template<>
bool CAsset_SkeletonAnimation::SetValue<vec4>(int ValueType, int PathInt, vec4 Value)
{
	CAsset_SkeletonAnimation::CSubPath SubPath(PathInt);
	switch(ValueType)
	{
		case LAYERKEYFRAME_COLOR:
			if(SubPath.GetType() == CAsset_SkeletonAnimation::CSubPath::TYPE_LAYERKEYFRAME)
			{
				int animId = SubPath.GetId();
				if(animId >= 0 && animId < m_LayerAnimations.size())
				{
					int keyId = SubPath.GetId2();
					if(keyId >= 0 && keyId < m_LayerAnimations[animId].m_KeyFrames.size())
					{
						m_LayerAnimations[animId].m_KeyFrames[keyId].m_Color = Value;
					}
					else return false;
				}
				else return false;
			}
			else return false;
	}
	
	return CAsset::SetValue<vec4>(ValueType, PathInt, Value);
}

/* VALUE ASSETPATH ****************************************************/

template<>
CAssetPath CAsset_SkeletonAnimation::GetValue<CAssetPath>(int ValueType, int Path, CAssetPath DefaultValue)
{
	switch(ValueType)
	{
		case SKELETONPATH:
			return m_SkeletonPath;
	}
	
	return CAsset::GetValue<CAssetPath>(ValueType, Path, DefaultValue);
}

template<>
bool CAsset_SkeletonAnimation::SetValue<CAssetPath>(int ValueType, int Path, CAssetPath Value)
{
	switch(ValueType)
	{
		case SKELETONPATH:
			m_SkeletonPath = Value;
			return true;
	}
	
	return CAsset::SetValue<CAssetPath>(ValueType, Path, Value);
}

}
