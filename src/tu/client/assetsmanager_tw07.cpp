#include <engine/storage.h>
#include <engine/shared/config.h>
#include <engine/shared/datafile.h>
#include <generated/client_data.h>
#include <game/mapitems.h>
#include <game/gamecore.h>
#include <game/client/render.h>
#include <tu/shared/assetsfile.h>

#include <engine/external/pnglite/pnglite.h>

#include <tu/client/assetsmanager.h>

namespace tu
{

void CAssetsManager::InitAssetsManager_TeeWorldsUniverse(CAssetsManager* pAssetsManager)
{
	//Images
	{
		CAssetPath ImagePath;
		CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_UNIVERSE, IStorage::TYPE_ALL, "game.png", -2);
		if(pImage)
		{			
			pImage->SetGridSize(32, 16);
		}
	}
	{
		CAssetPath ImagePath;
		CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_UNIVERSE, IStorage::TYPE_ALL, "particles.png", -2);
		if(pImage)
		{			
			pImage->SetGridSize(8, 8);
		}
	}
	{
		CAssetPath ImagePath;
		CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_UNIVERSE, IStorage::TYPE_ALL, "skins/body/standard.png", -2);
		if(pImage)
		{			
			pImage->SetGridSize(2, 2);
		}
	}
	{
		CAssetPath ImagePath;
		CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_UNIVERSE, IStorage::TYPE_ALL, "skins/feet/standard.png", -2);
		if(pImage)
		{			
			pImage->SetGridSize(2, 1);
		}
	}
	{
		CAssetPath ImagePath;
		CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_UNIVERSE, IStorage::TYPE_ALL, "skins/hands/standard.png", -2);
		if(pImage)
		{			
			pImage->SetGridSize(2, 1);
		}
	}
	{
		CAssetPath ImagePath;
		CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_UNIVERSE, IStorage::TYPE_ALL, "skins/eyes/standard.png", -2);
		if(pImage)
		{			
			pImage->SetGridSize(4, 4);
		}
	}
	{
		CAssetPath ImagePath;
		CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_UNIVERSE, IStorage::TYPE_ALL, "skins/hands/standard.png", -2);
		if(pImage)
		{			
			pImage->SetGridSize(2, 1);
		}
	}
	dbg_msg("assetsmanager", "Images initialised");
	
	#define CREATE_INTERNAL_SPRITE(id, name, image, x, y, w, h) {\
		CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(CAssetPath::Universe(CAssetPath::TYPE_SPRITE, id));\
		pSprite->SetName(name);\
		pSprite->Init(CAssetPath::Universe(CAssetPath::TYPE_IMAGE, image), x, y, w, h);\
	}
	
	{
		CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_WHITESQUARE));
		pSprite->SetName("whiteSquare");
		pSprite->Init(CAssetPath::Null(), 0, 0, 1, 1);
	}
	CREATE_INTERNAL_SPRITE(tu::SPRITE_HAMMER, "hammer", tu::IMAGE_GAME, 2, 1, 4, 3);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_GUN, "gun", tu::IMAGE_GAME, 2, 4, 4, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_SHOTGUN, "shotgun", tu::IMAGE_GAME, 2, 6, 8, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_GRENADE, "grenade", tu::IMAGE_GAME, 2, 8, 7, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_LASER, "laserRifle", tu::IMAGE_GAME, 2, 12, 7, 3);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_NINJA, "ninjaSword", tu::IMAGE_GAME, 2, 10, 8, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_GUN_MUZZLE1, "gunMuzzle1", tu::IMAGE_GAME, 8, 4, 3, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_GUN_MUZZLE2, "gunMuzzle2", tu::IMAGE_GAME, 12, 4, 3, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_GUN_MUZZLE3, "gunMuzzle3", tu::IMAGE_GAME, 16, 4, 3, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_SHOTGUN_MUZZLE1, "shotgunMuzzle1", tu::IMAGE_GAME, 12, 6, 3, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_SHOTGUN_MUZZLE2, "shotgunMuzzle2", tu::IMAGE_GAME, 16, 6, 3, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_SHOTGUN_MUZZLE3, "shotgunMuzzle3", tu::IMAGE_GAME, 20, 6, 3, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_NINJA_MUZZLE1, "ninjaMuzzle1", tu::IMAGE_GAME, 25, 0, 7, 4);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_NINJA_MUZZLE2, "ninjaMuzzle2", tu::IMAGE_GAME, 25, 4, 7, 4);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_NINJA_MUZZLE3, "ninjaMuzzle3", tu::IMAGE_GAME, 25, 8, 7, 4);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_HAMMER_CURSOR, "hammerCursor", tu::IMAGE_GAME, 0, 0, 2, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_GUN_CURSOR, "gunCursor", tu::IMAGE_GAME, 0, 4, 2, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_SHOTGUN_CURSOR, "shotgunCursor", tu::IMAGE_GAME, 0, 6, 2, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_GRENADE_CURSOR, "grenadeCursor", tu::IMAGE_GAME, 0, 8, 2, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_LASER_CURSOR, "laserCursor", tu::IMAGE_GAME, 0, 12, 2 , 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_NINJA_CURSOR, "ninjaCursor", tu::IMAGE_GAME, 0, 10 , 2, 2);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_HOOK_HEAD, "hookEndPoint", tu::IMAGE_GAME, 3, 0, 2, 1);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_HOOK_CHAIN, "hookChain", tu::IMAGE_GAME, 2, 0 , 1, 1);
	
	CREATE_INTERNAL_SPRITE(tu::SPRITE_PART_SPLAT1, "partSplat1", tu::IMAGE_PARTICLES, 2, 0 , 1, 1);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_PART_SPLAT2, "partSplat2", tu::IMAGE_PARTICLES, 3, 0 , 1, 1);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_PART_SPLAT3, "partSplat3", tu::IMAGE_PARTICLES, 4, 0 , 1, 1);
	
	CREATE_INTERNAL_SPRITE(tu::SPRITE_TEEBODYDEFAULT_SHADOW, "teeBodyDefaultShadow", tu::IMAGE_TEEBODYDEFAULT, 0, 0, 1, 1);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_TEEBODYDEFAULT_COLOR, "teeBodyDefaultColor", tu::IMAGE_TEEBODYDEFAULT, 1, 0, 1, 1);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_TEEBODYDEFAULT_SHADING, "teeBodyDefaultShading", tu::IMAGE_TEEBODYDEFAULT, 0, 1, 1, 1);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_TEEBODYDEFAULT_OUTLINE, "teeBodyDefaultOutline", tu::IMAGE_TEEBODYDEFAULT, 1, 1, 1, 1);
	
	CREATE_INTERNAL_SPRITE(tu::SPRITE_TEEFEETDEFAULT_SHADOW, "teeFeetDefaultShadow", tu::IMAGE_TEEFEETDEFAULT, 1, 0, 1, 1);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_TEEFEETDEFAULT_COLOR, "teeFeetDefaultColor", tu::IMAGE_TEEFEETDEFAULT, 0, 0, 1, 1);
	
	CREATE_INTERNAL_SPRITE(tu::SPRITE_TEEHANDSDEFAULT_SHADOW, "teeHandsDefaultShadow", tu::IMAGE_TEEHANDSDEFAULT, 1, 0, 1, 1);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_TEEHANDSDEFAULT_COLOR, "teeHandsDefaultColor", tu::IMAGE_TEEHANDSDEFAULT, 0, 0, 1, 1);
	
	CREATE_INTERNAL_SPRITE(tu::SPRITE_TEEEYESDEFAULT_NORMAL, "teeEyesDefaultNormal", tu::IMAGE_TEEEYESDEFAULT, 0, 0, 2, 1);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_TEEEYESDEFAULT_ANGRY, "teeEyesDefaultAngry", tu::IMAGE_TEEEYESDEFAULT, 2, 0, 2, 1);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_TEEEYESDEFAULT_PAIN, "teeEyesDefaultPain", tu::IMAGE_TEEEYESDEFAULT, 0, 1, 2, 1);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_TEEEYESDEFAULT_HAPPY, "teeEyesDefaultHappy", tu::IMAGE_TEEEYESDEFAULT, 2, 0, 2, 1);
	CREATE_INTERNAL_SPRITE(tu::SPRITE_TEEEYESDEFAULT_FEAR, "teeEyesDefaultFear", tu::IMAGE_TEEEYESDEFAULT, 0, 2, 2, 1);
	dbg_msg("assetsmanager", "Sprites initialised");
	
	//Skeletons
	{
		CAsset_Skeleton* pSkeleton = pAssetsManager->NewAsset<CAsset_Skeleton>(CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_PLACEHOLDER));
		pSkeleton->SetName("placeholder");
		pSkeleton->AddBone().Name("Position").Color(vec4(1.0f, 1.0f, 1.0f, 1.0f)).Length(32.0f);
	
		pSkeleton->AddLayer().Name("Color");
	}
	{
		CAsset_Skeleton* pSkeleton = pAssetsManager->NewAsset<CAsset_Skeleton>(CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE));
		pSkeleton->SetName("tee");
		pSkeleton->AddBone().Name("body").Color(vec4(1.0f, 0.0f, 1.0f, 1.0f))
			.Length(16.0f).Translation(vec2(0.0f, -4.0f));
		pSkeleton->AddBone().Name("eyes").Color(vec4(0.0f, 1.0f, 1.0f, 1.0f)).Parent(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY))
			.Length(8.0f).Translation(vec2(-16.0f, -3.2f)).Angle(-pi/4.0f);
		pSkeleton->AddBone().Name("backFoot").Color(vec4(0.5f, 0.0f, 0.0f, 1.0f))
			.Length(16.0f).Translation(vec2(0.0f, 10.0f));
		pSkeleton->AddBone().Name("frontFoot").Color(vec4(1.0f, 0.0f, 0.0f, 1.0f))
			.Length(16.0f).Translation(vec2(0.0f, 10.0f));
		pSkeleton->AddBone().Name("backArm").Color(vec4(0.0f, 0.5f, 0.0f, 1.0f))
			.Length(21.0f);
		pSkeleton->AddBone().Name("backHand").Color(vec4(0.0f, 0.5f, 0.0f, 1.0f)).Parent(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKARM))
			.Length(8.0f);
		pSkeleton->AddBone().Name("frontArm").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f))
			.Length(21.0f);
		pSkeleton->AddBone().Name("frontHand").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f)).Parent(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTARM))
			.Length(8.0f);
	
		pSkeleton->AddLayer().Name("attach");
		pSkeleton->AddLayer().Name("backHandShadow");
		pSkeleton->AddLayer().Name("frontHandShadow");
		pSkeleton->AddLayer().Name("backFootShadow");
		pSkeleton->AddLayer().Name("bodyShadow");
		pSkeleton->AddLayer().Name("frontFootShadow");
		pSkeleton->AddLayer().Name("backHand");
		pSkeleton->AddLayer().Name("frontHand");
		pSkeleton->AddLayer().Name("backFoot");
		pSkeleton->AddLayer().Name("decoration");
		pSkeleton->AddLayer().Name("body");
		pSkeleton->AddLayer().Name("marking");
		pSkeleton->AddLayer().Name("outline");
		pSkeleton->AddLayer().Name("eyesNormal");
		pSkeleton->AddLayer().Name("eyesAngry");
		pSkeleton->AddLayer().Name("eyesPain");
		pSkeleton->AddLayer().Name("eyesHappy");
		pSkeleton->AddLayer().Name("eyesFear");
		pSkeleton->AddLayer().Name("frontFoot");
	}
	{
		CAsset_Skeleton* pSkeleton = pAssetsManager->NewAsset<CAsset_Skeleton>(CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_HAMMER));
		pSkeleton->SetName("hammer");
		pSkeleton->m_DefaultSkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_HAMMER);
		pSkeleton->m_ParentPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeleton->AddBone().Name("attach").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f)).Parent(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_BODY))
			.Translation(vec2(-12.0f, -16.0f))
			.Length(48.0f);
	}
	{
		CAsset_Skeleton* pSkeleton = pAssetsManager->NewAsset<CAsset_Skeleton>(CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_GUN));
		pSkeleton->SetName("gun");
		pSkeleton->m_DefaultSkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_GUN);
		pSkeleton->m_ParentPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeleton->AddBone().Name("attach").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f)).Parent(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTARM))
			.Translation(vec2(0.0f, 0.0f))
			.Length(32.0f);
	}
	{
		CAsset_Skeleton* pSkeleton = pAssetsManager->NewAsset<CAsset_Skeleton>(CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_SHOTGUN));
		pSkeleton->SetName("shotgun");
		pSkeleton->m_DefaultSkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_SHOTGUN);
		pSkeleton->m_ParentPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeleton->AddBone().Name("attach").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f)).Parent(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTARM))
			.Translation(vec2(0.0f, 0.0f))
			.Length(32.0f);
	}
	dbg_msg("assetsmanager", "Skeletons initialised");
	
	//Skeleton Skins
	{
		CAsset_SkeletonSkin* pSkeletonSkin = pAssetsManager->NewAsset<CAsset_SkeletonSkin>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEBODYDEFAULT));
		pSkeletonSkin->SetName("teeBodyDefault");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEBODYDEFAULT_SHADOW),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BODYSHADOW))
				.Anchor(0.0f);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEBODYDEFAULT_COLOR),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BODY))
				.Anchor(0.0f);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEBODYDEFAULT_SHADING),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_OUTLINE))
				.Anchor(0.0f);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEBODYDEFAULT_OUTLINE),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_OUTLINE))
				.Anchor(0.0f);
	}
	{
		CAsset_SkeletonSkin* pSkeletonSkin = pAssetsManager->NewAsset<CAsset_SkeletonSkin>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEFEETDEFAULT));
		pSkeletonSkin->SetName("teeFeetDefault");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEFEETDEFAULT_SHADOW),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKFOOTSHADOW))
			.Anchor(0.0f)
			.Scale(vec2(30.476f, 30.476f));
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEFEETDEFAULT_SHADOW),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTFOOTSHADOW))
			.Anchor(0.0f)
			.Scale(vec2(30.476f, 30.476f));
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEFEETDEFAULT_COLOR),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKFOOT))
			.Anchor(0.0f)
			.Scale(vec2(30.476f, 30.476f));
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEFEETDEFAULT_COLOR),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTFOOT))
			.Anchor(0.0f)
			.Scale(vec2(30.476f, 30.476f));
	}
	{
		CAsset_SkeletonSkin* pSkeletonSkin = pAssetsManager->NewAsset<CAsset_SkeletonSkin>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEHANDSDEFAULT));
		pSkeletonSkin->SetName("teeHandsDefault");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEHANDSDEFAULT_SHADOW),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKHAND),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKHANDSHADOW))
			.Anchor(0.0f)
			.Scale(vec2(30.0f, 30.0f));
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEHANDSDEFAULT_SHADOW),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTHAND),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTHANDSHADOW))
			.Anchor(0.0f)
			.Scale(vec2(30.0f, 30.0f));
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEHANDSDEFAULT_COLOR),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKHAND),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKHAND))
			.Anchor(0.0f)
			.Scale(vec2(30.0f, 30.0f));
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEHANDSDEFAULT_COLOR),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTHAND),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTHAND))
			.Anchor(0.0f)
			.Scale(vec2(30.0f, 30.0f));
	}
	{
		CAsset_SkeletonSkin* pSkeletonSkin = pAssetsManager->NewAsset<CAsset_SkeletonSkin>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEEYESDEFAULT));
		pSkeletonSkin->SetName("teeEyesDefault");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEEYESDEFAULT_NORMAL),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_EYES),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_NORMAL))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEEYESDEFAULT_ANGRY),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_EYES),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_ANGRY))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEEYESDEFAULT_PAIN),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_EYES),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_PAIN))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEEYESDEFAULT_HAPPY),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_EYES),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_HAPPY))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEEYESDEFAULT_FEAR),
			CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_EYES),
			CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_FEAR))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
	}
	{
		CAsset_SkeletonSkin* pSkeletonSkin = pAssetsManager->NewAsset<CAsset_SkeletonSkin>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_HAMMER));
		pSkeletonSkin->SetName("hammer");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_HAMMER);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_HAMMER),
			CAsset_Skeleton::CSubPath::LocalBone(0),
			CAsset_Skeleton::CSubPath::ParentLayer(tu::TEELAYER_ATTACH))
			.Translation(vec2(-44.0f, 0.0f))
			.Scale(vec2(76.8f, 76.8f));
	}
	{
		CAsset_SkeletonSkin* pSkeletonSkin = pAssetsManager->NewAsset<CAsset_SkeletonSkin>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_GUN));
		pSkeletonSkin->SetName("gun");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_GUN);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_GUN),
			CAsset_Skeleton::CSubPath::LocalBone(0),
			CAsset_Skeleton::CSubPath::ParentLayer(tu::TEELAYER_ATTACH))
			.Translation(vec2(-21.0f, 0.0f))
			.Scale(vec2(57.24f, 57.24f));
	}
	{
		CAsset_SkeletonSkin* pSkeletonSkin = pAssetsManager->NewAsset<CAsset_SkeletonSkin>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_SHOTGUN));
		pSkeletonSkin->SetName("shotgun");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_SHOTGUN);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_SHOTGUN),
			CAsset_Skeleton::CSubPath::LocalBone(0),
			CAsset_Skeleton::CSubPath::ParentLayer(tu::TEELAYER_ATTACH))
			.Translation(vec2(-21.0f, 0.0f))
			.Scale(vec2(93.13f, 93.13f));
	}
	dbg_msg("assetsmanager", "Skeleton Skins initialised");
	
	//Skeleton Animations
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = pAssetsManager->NewAsset<CAsset_SkeletonAnimation>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEIDLE));
		pSkeletonAnimation->SetName("teeIdle");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT),  0).Translation(vec2(-7.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT), 0).Translation(vec2( 7.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKARM), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_HOOK);
		
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_ANGRY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_PAIN), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_HAPPY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_FEAR), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}
	
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = pAssetsManager->NewAsset<CAsset_SkeletonAnimation>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEAIR));
		pSkeletonAnimation->SetName("teeAir");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT),  0).Translation(vec2(-3.0f,  0.0f)).Angle(-0.1f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT), 0).Translation(vec2( 3.0f,  0.0f)).Angle(-0.1f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKARM), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_HOOK);
		
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_ANGRY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_PAIN), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_HAPPY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_FEAR), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}
	
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = pAssetsManager->NewAsset<CAsset_SkeletonAnimation>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEAIR2));
		pSkeletonAnimation->SetName("teeAir2");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT),  0).Translation(vec2(-3.0f,  0.0f)).Angle(-0.1f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT), 0).Translation(vec2( 3.0f,  0.0f)).Angle(-0.1f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKARM), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_HOOK);
		
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKFOOT), 0).Color(vec4(0.5f, 0.5f, 0.5f, 1.0f));
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTFOOT), 0).Color(vec4(0.5f, 0.5f, 0.5f, 1.0f));
		
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_ANGRY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_PAIN), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_HAPPY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_FEAR), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}
	
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = pAssetsManager->NewAsset<CAsset_SkeletonAnimation>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEWALK));
		pSkeletonAnimation->SetName("teeWalk");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKARM), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_HOOK);
		
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY), 0 ).Translation(vec2(0.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY), 6 ).Translation(vec2(0.0f, -1.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY), 12).Translation(vec2(0.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY), 18).Translation(vec2(0.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY), 24).Translation(vec2(0.0f, -1.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY), 30).Translation(vec2(0.0f,  0.0f));
		pSkeletonAnimation->SetCycle(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY), CAsset_SkeletonAnimation::CYCLETYPE_LOOP);
		
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT), 0 ).Translation(vec2(  8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT), 6 ).Translation(vec2( -8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT), 12).Translation(vec2(-10.0f, -4.0f)).Angle(0.2f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT), 18).Translation(vec2( -8.0f, -8.0f)).Angle(0.3f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT), 24).Translation(vec2(  4.0f, -4.0f)).Angle(-0.2f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT), 30).Translation(vec2(  8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->SetCycle(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT), CAsset_SkeletonAnimation::CYCLETYPE_LOOP);
		
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT), 0 ).Translation(vec2(-10.0f, -4.0f)).Angle(0.2f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT), 6 ).Translation(vec2( -8.0f, -8.0f)).Angle(0.3f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT), 12).Translation(vec2(  4.0f, -4.0f)).Angle(-0.2f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT), 18).Translation(vec2(  8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT), 24).Translation(vec2(  8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT), 30).Translation(vec2(-10.0f, -4.0f)).Angle(0.2f*pi*2.0f);
		pSkeletonAnimation->SetCycle(CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT), CAsset_SkeletonAnimation::CYCLETYPE_LOOP);
		
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_ANGRY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_PAIN), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_HAPPY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_FEAR), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = pAssetsManager->NewAsset<CAsset_SkeletonAnimation>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_HAMMERATTACK));
		pSkeletonAnimation->SetName("hammerAttack");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_HAMMER);
		
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(0), 0).Angle(-pi/2-0.10f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(0), 2).Angle(-pi/2+0.25f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(0), 4).Angle(-pi/2+0.30f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(0), 6).Angle(-pi/2+0.25f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(0), 9).Angle(-pi/2-0.10f*pi*2.0f);
		
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_EYES), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);
		
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::ParentLayer(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::ParentLayer(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = pAssetsManager->NewAsset<CAsset_SkeletonAnimation>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_GUNATTACK));
		pSkeletonAnimation->SetName("gunAttack");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_GUN);
		
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTARM), 0)
			.Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTARM), 2)
			.Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTARM), 4)
			.Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);
			
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTHAND), 0)
			.Angle(-3*pi/4)
			.Translation(vec2(0.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTHAND), 2)
			.Angle(-3*pi/4)
			.Translation(vec2(-10.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTHAND), 4)
			.Angle(-3*pi/4)
			.Translation(vec2(0.0f, 0.0f));
			
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(0), 0)
			.Translation(vec2(0.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(0), 2)
			.Translation(vec2(-10.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(0), 4)
			.Translation(vec2(0.0f, 0.0f));
		
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_EYES), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);
		
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::ParentLayer(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_VISIBLE);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::ParentLayer(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_VISIBLE);
	}
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = pAssetsManager->NewAsset<CAsset_SkeletonAnimation>(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_SHOTGUNATTACK));
		pSkeletonAnimation->SetName("shotgunAttack");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_SHOTGUN);
		
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTARM), 0)
			.Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTARM), 2)
			.Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTARM), 4)
			.Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);
			
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTHAND), 0)
			.Angle(-pi/2)
			.Translation(vec2(0.0f, 6.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTHAND), 2)
			.Angle(-pi/2)
			.Translation(vec2(-10.0f, 6.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_FRONTHAND), 4)
			.Angle(-pi/2)
			.Translation(vec2(0.0f, 6.0f));
			
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(0), 0)
			.Translation(vec2(0.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(0), 2)
			.Translation(vec2(-10.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::LocalBone(0), 4)
			.Translation(vec2(0.0f, 0.0f));
		
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CSubPath::ParentBone(tu::TEEBONE_EYES), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);
		
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::ParentLayer(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_VISIBLE);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CSubPath::ParentLayer(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_VISIBLE);
	}
	dbg_msg("assetsmanager", "Skeleton Animations initialised");
	
	//Character
	{
		CAsset_Character* pCharacter = pAssetsManager->NewAsset<CAsset_Character>(CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE));
		pCharacter->SetName("tee");
		
		pCharacter->AddPart().Name("body").Default(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEBODYDEFAULT));
		pCharacter->AddPart().Name("marking");
		pCharacter->AddPart().Name("decoration");
		pCharacter->AddPart().Name("hands").Default(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEHANDSDEFAULT));
		pCharacter->AddPart().Name("feet").Default(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEFEETDEFAULT));
		pCharacter->AddPart().Name("eyes").Default(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEEYESDEFAULT));
		
		pCharacter->SetIdlePath(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEIDLE));
		pCharacter->SetWalkPath(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEWALK));
		pCharacter->SetControlledJumpPath(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEAIR));
		pCharacter->SetUncontrolledJumpPath(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEAIR2));
	}
	dbg_msg("assetsmanager", "Characters initialised");
	
	//CharacterPart
	{
		CAsset_CharacterPart* pCharacterPart = pAssetsManager->NewAsset<CAsset_CharacterPart>(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEBODYDEFAULT));
		pCharacterPart->SetName("teeBodyDefault");
		pCharacterPart->SetCharacterPath(CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE));
		pCharacterPart->SetCharacterPart(CAsset_Character::CSubPath::Part(tu::SKINPART_BODY));
		pCharacterPart->SetSkeletonSkinPath(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEBODYDEFAULT));
	}
	{
		CAsset_CharacterPart* pCharacterPart = pAssetsManager->NewAsset<CAsset_CharacterPart>(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEFEETDEFAULT));
		pCharacterPart->SetName("teeFeetDefault");
		pCharacterPart->SetCharacterPath(CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE));
		pCharacterPart->SetCharacterPart(CAsset_Character::CSubPath::Part(tu::SKINPART_FEET));
		pCharacterPart->SetSkeletonSkinPath(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEFEETDEFAULT));
	}
	{
		CAsset_CharacterPart* pCharacterPart = pAssetsManager->NewAsset<CAsset_CharacterPart>(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEEYESDEFAULT));
		pCharacterPart->SetName("teeEyesDefault");
		pCharacterPart->SetCharacterPath(CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE));
		pCharacterPart->SetCharacterPart(CAsset_Character::CSubPath::Part(tu::SKINPART_EYES));
		pCharacterPart->SetSkeletonSkinPath(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEEYESDEFAULT));
	}
	dbg_msg("assetsmanager", "Character parts initialized");
	
	//Weapons
	{
		CAsset_Weapon* pWeapon = pAssetsManager->NewAsset<CAsset_Weapon>(CAssetPath::Universe(CAssetPath::TYPE_WEAPON, tu::WEAPON_HAMMER));
		pWeapon->SetName("hammer");
		pWeapon->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
		pWeapon->m_CursorPath = CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_HAMMER_CURSOR);
		pWeapon->m_SkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_HAMMER);
		pWeapon->m_AttackAnimationPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_HAMMERATTACK);
	}
	{
		CAsset_Weapon* pWeapon = pAssetsManager->NewAsset<CAsset_Weapon>(CAssetPath::Universe(CAssetPath::TYPE_WEAPON, tu::WEAPON_GUN));
		pWeapon->SetName("gun");
		pWeapon->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
		pWeapon->m_CursorPath = CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_GUN_CURSOR);
		pWeapon->m_SkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_GUN);
		pWeapon->m_AttackAnimationPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_GUNATTACK);
	}
	dbg_msg("assetsmanager", "Weapons initialised");
	
	//ZoneTypes
	{
		CAsset_ZoneType* pZoneType = pAssetsManager->NewAsset<CAsset_ZoneType>(CAssetPath::Universe(CAssetPath::TYPE_ZONETYPE, tu::ZONETYPE_PHYSICS));
		pZoneType->SetName("Physics");
		CAsset_ZoneType::CSubPath IndexPath;
		
		IndexPath = pZoneType->NewIndex();
		pZoneType->SetIndexName(IndexPath, "Air");
		pZoneType->SetIndexNumber(IndexPath, ZONETYPEINDEX_PHYSICS_AIR);
		IndexPath = pZoneType->NewIndex();
		pZoneType->SetIndexName(IndexPath, "HookableGround");
		pZoneType->SetIndexNumber(IndexPath, ZONETYPEINDEX_PHYSICS_GROUND);
		pZoneType->SetIndexColor(IndexPath, 1.0f);
		IndexPath = pZoneType->NewIndex();
		pZoneType->SetIndexName(IndexPath, "UnhookableGround");
		pZoneType->SetIndexNumber(IndexPath, ZONETYPEINDEX_PHYSICS_NOHOOK);
		pZoneType->SetIndexColor(IndexPath, vec4(228.0f/255.0f, 255.0f/255.0f, 0.0f/255.0f, 1.0f));
	}
	{
		CAsset_ZoneType* pZoneType = pAssetsManager->NewAsset<CAsset_ZoneType>(CAssetPath::Universe(CAssetPath::TYPE_ZONETYPE, tu::ZONETYPE_DEATH));
		pZoneType->SetName("DeathZone");
		CAsset_ZoneType::CSubPath IndexPath;
		
		IndexPath = pZoneType->NewIndex();
		pZoneType->SetIndexName(IndexPath, "Death");
		pZoneType->SetIndexNumber(IndexPath, ZONETYPEINDEX_DEATH_DEATH);
		pZoneType->SetIndexColor(IndexPath, vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	dbg_msg("assetsmanager", "Zone types initialised");
	
	LoadSkins(pAssetsManager);
}

int CAssetsManager::LoadSkinAssets_BodyScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CAssetsManager *pAssetsManager = (CAssetsManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;
	
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/body/%s", pFilename);
	
	CAssetPath ImagePath;
	CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_SKIN, IStorage::TYPE_ALL, aBuf, -2);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "body/%s.png", aName);
		pImage->SetName(aBuf);
		
		pImage->SetGridSize(2, 2);
	
		CAssetPath ShadowPath;
		CAssetPath ColorPath;
		CAssetPath ShadingPath;
		CAssetPath OutinePath;
		CAssetPath SkeletonSkinPath;
		{
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&ShadowPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "body/%s/shadow", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 1, 1);
		}
		{
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&ColorPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "body/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&ShadingPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "body/%s/shading", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 1, 1, 1);
		}
		{
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&OutinePath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "body/%s/outline", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 1, 1, 1);
		}
		{
			CAsset_SkeletonSkin* pSkeletonSkin = pAssetsManager->NewAsset<CAsset_SkeletonSkin>(&SkeletonSkinPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "body/%s", aName);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BODYSHADOW))
				.Anchor(0.0f);
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BODY))
				.Anchor(0.0f);
			pSkeletonSkin->AddSprite(
				ShadingPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_OUTLINE))
				.Anchor(0.0f);
			pSkeletonSkin->AddSprite(
				OutinePath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_OUTLINE))
				.Anchor(0.0f);
		}
		{
			CAssetPath CharacterPartPath;
			CAsset_CharacterPart* pCharacterPart = pAssetsManager->NewAsset<CAsset_CharacterPart>(&CharacterPartPath, CAssetPath::SRC_SKIN, -2);
			pCharacterPart->SetName(aName);
			pCharacterPart->SetCharacterPath(CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE));
			pCharacterPart->SetCharacterPart(CAsset_Character::CSubPath::Part(tu::SKINPART_BODY));
			pCharacterPart->SetSkeletonSkinPath(SkeletonSkinPath);
		}
	}
	
	return 0;
}

int CAssetsManager::LoadSkinAssets_FootScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CAssetsManager *pAssetsManager = (CAssetsManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;
	
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/feet/%s", pFilename);
	
	CAssetPath ImagePath;
	CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_SKIN, IStorage::TYPE_ALL, aBuf, -2);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "feet/%s.png", aName);
		pImage->SetName(aBuf);
		
		pImage->SetGridSize(2, 1);
	
		CAssetPath ShadowPath;
		CAssetPath ColorPath;
		CAssetPath SkeletonSkinPath;
		{
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&ShadowPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "feet/%s/shadow", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&ColorPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "feet/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 1, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "feet/%s", aName);
			CAsset_SkeletonSkin* pSkeletonSkin = pAssetsManager->NewAsset<CAsset_SkeletonSkin>(&SkeletonSkinPath, CAssetPath::SRC_SKIN, -2);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKFOOTSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(30.476f, 30.476f));
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTFOOTSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(30.476f, 30.476f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKFOOT),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKFOOT))
				.Anchor(0.0f)
				.Scale(vec2(30.476f, 30.476f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTFOOT),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTFOOT))
				.Anchor(0.0f)
				.Scale(vec2(30.476f, 30.476f));
		}
		{
			CAssetPath CharacterPartPath;
			CAsset_CharacterPart* pCharacterPart = pAssetsManager->NewAsset<CAsset_CharacterPart>(&CharacterPartPath, CAssetPath::SRC_SKIN, -2);
			pCharacterPart->SetName(aName);
			pCharacterPart->SetCharacterPath(CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE));
			pCharacterPart->SetCharacterPart(CAsset_Character::CSubPath::Part(tu::SKINPART_FEET));
			pCharacterPart->SetSkeletonSkinPath(SkeletonSkinPath);
		}
	}
	
	return 0;
}

int CAssetsManager::LoadSkinAssets_HandsScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CAssetsManager *pAssetsManager = (CAssetsManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;
	
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/hands/%s", pFilename);
	
	CAssetPath ImagePath;
	CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_SKIN, IStorage::TYPE_ALL, aBuf, -2);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "hands/%s.png", aName);
		pImage->SetName(aBuf);
		
		pImage->SetGridSize(2, 1);
	
		CAssetPath ShadowPath;
		CAssetPath ColorPath;
		CAssetPath SkeletonSkinPath;
		{
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&ShadowPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "hands/%s/shadow", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&ColorPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "hands/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 1, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "hands/%s", aName);
			CAsset_SkeletonSkin* pSkeletonSkin = pAssetsManager->NewAsset<CAsset_SkeletonSkin>(&SkeletonSkinPath, CAssetPath::SRC_SKIN, -2);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKHAND),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKHANDSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(30.0f, 30.0f));
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTHAND),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTHANDSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(30.0f, 30.0f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BACKHAND),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BACKHAND))
				.Anchor(0.0f)
				.Scale(vec2(30.0f, 30.0f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_FRONTHAND),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_FRONTHAND))
				.Anchor(0.0f)
				.Scale(vec2(30.0f, 30.0f));
		}
		{
			CAssetPath CharacterPartPath;
			CAsset_CharacterPart* pCharacterPart = pAssetsManager->NewAsset<CAsset_CharacterPart>(&CharacterPartPath, CAssetPath::SRC_SKIN, -2);
			pCharacterPart->SetName(aName);
			pCharacterPart->SetCharacterPath(CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE));
			pCharacterPart->SetCharacterPart(CAsset_Character::CSubPath::Part(tu::SKINPART_HANDS));
			pCharacterPart->SetSkeletonSkinPath(SkeletonSkinPath);
		}
	}
	
	return 0;
}

int CAssetsManager::LoadSkinAssets_MarkingScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CAssetsManager *pAssetsManager = (CAssetsManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;
	
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/marking/%s", pFilename);
	
	CAssetPath ImagePath;
	CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_SKIN, IStorage::TYPE_ALL, aBuf, -2);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "marking/%s.png", aName);
		pImage->SetName(aBuf);
		
		pImage->SetGridSize(1, 1);
	
		CAssetPath ColorPath;
		CAssetPath SkeletonSkinPath;
		{
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&ColorPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "marking/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "marking/%s", aName);
			CAsset_SkeletonSkin* pSkeletonSkin = pAssetsManager->NewAsset<CAsset_SkeletonSkin>(&SkeletonSkinPath, CAssetPath::SRC_SKIN, -2);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_MARKING))
				.Anchor(0.0f)
				.Scale(vec2(64.0f, 64.0f));
		}
		{
			CAssetPath CharacterPartPath;
			CAsset_CharacterPart* pCharacterPart = pAssetsManager->NewAsset<CAsset_CharacterPart>(&CharacterPartPath, CAssetPath::SRC_SKIN, -2);
			pCharacterPart->SetName(aName);
			pCharacterPart->SetCharacterPath(CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE));
			pCharacterPart->SetCharacterPart(CAsset_Character::CSubPath::Part(tu::SKINPART_MARKING));
			pCharacterPart->SetSkeletonSkinPath(SkeletonSkinPath);
		}
	}
	
	return 0;
}

int CAssetsManager::LoadSkinAssets_DecorationScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CAssetsManager *pAssetsManager = (CAssetsManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;
	
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/decoration/%s", pFilename);
	
	CAssetPath ImagePath;
	CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_SKIN, IStorage::TYPE_ALL, aBuf, -2);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "decoration/%s.png", aName);
		pImage->SetName(aBuf);
		
		pImage->SetGridSize(2, 1);
	
		CAssetPath ShadowPath;
		CAssetPath ColorPath;
		CAssetPath SkeletonSkinPath;
		{
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&ShadowPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "decoration/%s/shadow", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&ColorPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "decoration/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 1, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "decoration/%s", aName);
			CAsset_SkeletonSkin* pSkeletonSkin = pAssetsManager->NewAsset<CAsset_SkeletonSkin>(&SkeletonSkinPath, CAssetPath::SRC_SKIN, -2);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_BODYSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(64.0f, 64.0f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_BODY),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_DECORATION))
				.Anchor(0.0f)
				.Scale(vec2(64.0f, 64.0f));
		}
		{
			CAssetPath CharacterPartPath;
			CAsset_CharacterPart* pCharacterPart = pAssetsManager->NewAsset<CAsset_CharacterPart>(&CharacterPartPath, CAssetPath::SRC_SKIN, -2);
			pCharacterPart->SetName(aName);
			pCharacterPart->SetCharacterPath(CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE));
			pCharacterPart->SetCharacterPart(CAsset_Character::CSubPath::Part(tu::SKINPART_DECORATION));
			pCharacterPart->SetSkeletonSkinPath(SkeletonSkinPath);
		}
	}
	
	return 0;
}

int CAssetsManager::LoadSkinAssets_EyeScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CAssetsManager *pAssetsManager = (CAssetsManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;
	
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/eyes/%s", pFilename);
	
	CAssetPath ImagePath;
	CAsset_Image* pImage = pAssetsManager->NewImage(&ImagePath, CAssetPath::SRC_SKIN, IStorage::TYPE_ALL, aBuf, -2);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "eyes/%s.png", aName);
		pImage->SetName(aBuf);
		
		pImage->SetGridSize(1, 1);
	
		CAssetPath NormalPath;
		CAssetPath AngryPath;
		CAssetPath PainPath;
		CAssetPath HappyPath;
		CAssetPath FearPath;
		CAssetPath SkeletonSkinPath;
		{
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&NormalPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/normal", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 2, 1);
		}
		{
			CAssetPath SpritePath;
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&AngryPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/angry", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 2, 0, 2, 1);
		}
		{
			CAssetPath SpritePath;
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&PainPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/pain", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 1, 2, 1);
		}
		{
			CAssetPath SpritePath;
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&HappyPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/happy", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 2, 1, 2, 1);
		}
		{
			CAssetPath SpritePath;
			CAsset_Sprite* pSprite = pAssetsManager->NewAsset<CAsset_Sprite>(&FearPath, CAssetPath::SRC_SKIN, -2);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/fear", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 2, 2, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "eyes/%s", aName);
			CAsset_SkeletonSkin* pSkeletonSkin = pAssetsManager->NewAsset<CAsset_SkeletonSkin>(&SkeletonSkinPath, CAssetPath::SRC_SKIN, -2);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				NormalPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_EYES),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_NORMAL))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
			pSkeletonSkin->AddSprite(
				AngryPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_EYES),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_ANGRY))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
			pSkeletonSkin->AddSprite(
				PainPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_EYES),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_PAIN))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
			pSkeletonSkin->AddSprite(
				HappyPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_EYES),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_HAPPY))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
			pSkeletonSkin->AddSprite(
				FearPath,
				CAsset_Skeleton::CSubPath::LocalBone(tu::TEEBONE_EYES),
				CAsset_Skeleton::CSubPath::LocalLayer(tu::TEELAYER_EYES_FEAR))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
		}
		{
			CAssetPath CharacterPartPath;
			CAsset_CharacterPart* pCharacterPart = pAssetsManager->NewAsset<CAsset_CharacterPart>(&CharacterPartPath, CAssetPath::SRC_SKIN, -2);
			pCharacterPart->SetName(aName);
			pCharacterPart->SetCharacterPath(CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE));
			pCharacterPart->SetCharacterPart(CAsset_Character::CSubPath::Part(tu::SKINPART_EYES));
			pCharacterPart->SetSkeletonSkinPath(SkeletonSkinPath);
		}
	}
	
	return 0;
}

void CAssetsManager::LoadSkins(CAssetsManager* pAssetsManager)
{
	dbg_msg("assetsmanager", "load skin assets");
	pAssetsManager->Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/body", CAssetsManager::LoadSkinAssets_BodyScan, pAssetsManager);
	dbg_msg("assetsmanager", "body assets loaded");
	pAssetsManager->Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/feet", CAssetsManager::LoadSkinAssets_FootScan, pAssetsManager);
	dbg_msg("assetsmanager", "feet assets loaded");
	pAssetsManager->Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/eyes", CAssetsManager::LoadSkinAssets_EyeScan, pAssetsManager);
	dbg_msg("assetsmanager", "eyes assets loaded");
	pAssetsManager->Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/hands", CAssetsManager::LoadSkinAssets_HandsScan, pAssetsManager);
	dbg_msg("assetsmanager", "hands assets loaded");
	pAssetsManager->Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/marking", CAssetsManager::LoadSkinAssets_MarkingScan, pAssetsManager);
	dbg_msg("assetsmanager", "marking assets loaded");
	pAssetsManager->Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/decoration", CAssetsManager::LoadSkinAssets_DecorationScan, pAssetsManager);
	dbg_msg("assetsmanager", "decoration assets loaded");
}

}
