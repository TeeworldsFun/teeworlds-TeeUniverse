#include <modapi/client/assetsmanager.h>
#include <engine/storage.h>
#include <modapi/shared/assetsfile.h>
#include <generated/client_data.h>
#include <game/client/render.h>
#include <engine/shared/datafile.h>
#include <engine/shared/config.h>

#include <game/gamecore.h>

#include <engine/external/pnglite/pnglite.h>
	
CModAPI_AssetManager::CModAPI_AssetManager(IGraphics* pGraphics, IStorage* pStorage)
{
	m_pGraphics = pGraphics;
	m_pStorage = pStorage;
}

#define MODAPI_MACRO_ASSETTYPE(ClassName, CatalogName) template<>\
CModAPI_AssetCatalog<ClassName>* CModAPI_AssetManager::GetAssetCatalog<ClassName>()\
{\
	return &CatalogName;\
}

#include <modapi/client/assetsmacro.h>

#undef MODAPI_MACRO_ASSETTYPE

void CModAPI_AssetManager::Init(IStorage* pStorage)
{
	LoadInteralAssets();
	LoadSkinAssets(pStorage);
}

void CModAPI_AssetManager::LoadInteralAssets()
{
	//Images
	{
		CModAPI_AssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "game.png", CModAPI_AssetPath::SRC_INTERNAL);
		CModAPI_Asset_Image* pImage = GetAsset<CModAPI_Asset_Image>(ImagePath);
		if(pImage)
		{			
			pImage->m_GridWidth = 32;
			pImage->m_GridHeight = 16;
		}
	}
	{
		CModAPI_AssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "particles.png", CModAPI_AssetPath::SRC_INTERNAL);
		CModAPI_Asset_Image* pImage = GetAsset<CModAPI_Asset_Image>(ImagePath);
		if(pImage)
		{			
			pImage->m_GridWidth = 8;
			pImage->m_GridHeight = 8;
		}
	}
	{
		CModAPI_AssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "skins/body/standard.png", CModAPI_AssetPath::SRC_INTERNAL);
		CModAPI_Asset_Image* pImage = GetAsset<CModAPI_Asset_Image>(ImagePath);
		if(pImage)
		{			
			pImage->m_GridWidth = 2;
			pImage->m_GridHeight = 2;
		}
	}
	{
		CModAPI_AssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "skins/feet/standard.png", CModAPI_AssetPath::SRC_INTERNAL);
		CModAPI_Asset_Image* pImage = GetAsset<CModAPI_Asset_Image>(ImagePath);
		if(pImage)
		{			
			pImage->m_GridWidth = 2;
			pImage->m_GridHeight = 1;
		}
	}
	{
		CModAPI_AssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "skins/hands/standard.png", CModAPI_AssetPath::SRC_INTERNAL);
		CModAPI_Asset_Image* pImage = GetAsset<CModAPI_Asset_Image>(ImagePath);
		if(pImage)
		{			
			pImage->m_GridWidth = 2;
			pImage->m_GridHeight = 1;
		}
	}
	{
		CModAPI_AssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "skins/eyes/standard.png", CModAPI_AssetPath::SRC_INTERNAL);
		CModAPI_Asset_Image* pImage = GetAsset<CModAPI_Asset_Image>(ImagePath);
		if(pImage)
		{			
			pImage->m_GridWidth = 4;
			pImage->m_GridHeight = 4;
		}
	}
	{
		CModAPI_AssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "skins/hands/standard.png", CModAPI_AssetPath::SRC_INTERNAL);
		CModAPI_Asset_Image* pImage = GetAsset<CModAPI_Asset_Image>(ImagePath);
		if(pImage)
		{			
			pImage->m_GridWidth = 2;
			pImage->m_GridHeight = 1;
		}
	}
	dbg_msg("assetsmanager", "Images initialised");
	
	#define CREATE_INTERNAL_SPRITE(id, name, image, x, y, w, h) {\
		CModAPI_Asset_Sprite* pSprite = m_SpritesCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, id));\
		pSprite->SetName(name);\
		pSprite->Init(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_IMAGE, image), x, y, w, h);\
	}
	
	{
		CModAPI_Asset_Sprite* pSprite = m_SpritesCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_WHITESQUARE));
		pSprite->SetName("whiteSquare");
		pSprite->Init(CModAPI_AssetPath::Null(), 0, 0, 1, 1);
	}
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_HAMMER, "hammer", MODAPI_IMAGE_GAME, 2, 1, 4, 3);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_GUN, "gun", MODAPI_IMAGE_GAME, 2, 4, 4, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_SHOTGUN, "shotgun", MODAPI_IMAGE_GAME, 2, 6, 8, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_GRENADE, "grenade", MODAPI_IMAGE_GAME, 2, 8, 7, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_LASER, "laserRifle", MODAPI_IMAGE_GAME, 2, 12, 7, 3);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_NINJA, "ninjaSword", MODAPI_IMAGE_GAME, 2, 10, 8, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_GUN_MUZZLE1, "gunMuzzle1", MODAPI_IMAGE_GAME, 8, 4, 3, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_GUN_MUZZLE2, "gunMuzzle2", MODAPI_IMAGE_GAME, 12, 4, 3, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_GUN_MUZZLE3, "gunMuzzle3", MODAPI_IMAGE_GAME, 16, 4, 3, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_SHOTGUN_MUZZLE1, "shotgunMuzzle1", MODAPI_IMAGE_GAME, 12, 6, 3, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_SHOTGUN_MUZZLE2, "shotgunMuzzle2", MODAPI_IMAGE_GAME, 16, 6, 3, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_SHOTGUN_MUZZLE3, "shotgunMuzzle3", MODAPI_IMAGE_GAME, 20, 6, 3, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_NINJA_MUZZLE1, "ninjaMuzzle1", MODAPI_IMAGE_GAME, 25, 0, 7, 4);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_NINJA_MUZZLE2, "ninjaMuzzle2", MODAPI_IMAGE_GAME, 25, 4, 7, 4);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_NINJA_MUZZLE3, "ninjaMuzzle3", MODAPI_IMAGE_GAME, 25, 8, 7, 4);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_HAMMER_CURSOR, "hammerCursor", MODAPI_IMAGE_GAME, 0, 0, 2, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_GUN_CURSOR, "gunCursor", MODAPI_IMAGE_GAME, 0, 4, 2, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_SHOTGUN_CURSOR, "shotgunCursor", MODAPI_IMAGE_GAME, 0, 6, 2, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_GRENADE_CURSOR, "grenadeCursor", MODAPI_IMAGE_GAME, 0, 8, 2, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_LASER_CURSOR, "laserCursor", MODAPI_IMAGE_GAME, 0, 12, 2 , 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_NINJA_CURSOR, "ninjaCursor", MODAPI_IMAGE_GAME, 0, 10 , 2, 2);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_HOOK_HEAD, "hookEndPoint", MODAPI_IMAGE_GAME, 3, 0, 2, 1);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_HOOK_CHAIN, "hookChain", MODAPI_IMAGE_GAME, 2, 0 , 1, 1);
	
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_PART_SPLAT1, "partSplat1", MODAPI_IMAGE_PARTICLES, 2, 0 , 1, 1);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_PART_SPLAT2, "partSplat2", MODAPI_IMAGE_PARTICLES, 3, 0 , 1, 1);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_PART_SPLAT3, "partSplat3", MODAPI_IMAGE_PARTICLES, 4, 0 , 1, 1);
	
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_TEEBODYDEFAULT_SHADOW, "teeBodyDefaultShadow", MODAPI_IMAGE_TEEBODYDEFAULT, 0, 0, 1, 1);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_TEEBODYDEFAULT_COLOR, "teeBodyDefaultColor", MODAPI_IMAGE_TEEBODYDEFAULT, 1, 0, 1, 1);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_TEEBODYDEFAULT_SHADING, "teeBodyDefaultShading", MODAPI_IMAGE_TEEBODYDEFAULT, 0, 1, 1, 1);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_TEEBODYDEFAULT_OUTLINE, "teeBodyDefaultOutline", MODAPI_IMAGE_TEEBODYDEFAULT, 1, 1, 1, 1);
	
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_TEEFEETDEFAULT_SHADOW, "teeFeetDefaultShadow", MODAPI_IMAGE_TEEFEETDEFAULT, 1, 0, 1, 1);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_TEEFEETDEFAULT_COLOR, "teeFeetDefaultColor", MODAPI_IMAGE_TEEFEETDEFAULT, 0, 0, 1, 1);
	
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_TEEHANDSDEFAULT_SHADOW, "teeHandsDefaultShadow", MODAPI_IMAGE_TEEHANDSDEFAULT, 1, 0, 1, 1);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_TEEHANDSDEFAULT_COLOR, "teeHandsDefaultColor", MODAPI_IMAGE_TEEHANDSDEFAULT, 0, 0, 1, 1);
	
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_TEEEYESDEFAULT_NORMAL, "teeEyesDefaultNormal", MODAPI_IMAGE_TEEEYESDEFAULT, 0, 0, 2, 1);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_TEEEYESDEFAULT_ANGRY, "teeEyesDefaultAngry", MODAPI_IMAGE_TEEEYESDEFAULT, 2, 0, 2, 1);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_TEEEYESDEFAULT_PAIN, "teeEyesDefaultPain", MODAPI_IMAGE_TEEEYESDEFAULT, 0, 1, 2, 1);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_TEEEYESDEFAULT_HAPPY, "teeEyesDefaultHappy", MODAPI_IMAGE_TEEEYESDEFAULT, 2, 0, 2, 1);
	CREATE_INTERNAL_SPRITE(MODAPI_SPRITE_TEEEYESDEFAULT_FEAR, "teeEyesDefaultFear", MODAPI_IMAGE_TEEEYESDEFAULT, 0, 2, 2, 1);
	dbg_msg("assetsmanager", "Sprites initialised");
	
	//Skeletons
	{
		CModAPI_Asset_Skeleton* pSkeleton = m_SkeletonsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE));
		pSkeleton->SetName("tee");
		pSkeleton->AddBone().Name("body").Color(vec4(1.0f, 0.0f, 1.0f, 1.0f))
			.Length(16.0f).Translation(vec2(0.0f, -4.0f));
		pSkeleton->AddBone().Name("eyes").Color(vec4(0.0f, 1.0f, 1.0f, 1.0f)).Parent(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY))
			.Length(8.0f).Translation(vec2(-16.0f, -3.2f)).Angle(-pi/4.0f);
		pSkeleton->AddBone().Name("backFoot").Color(vec4(0.5f, 0.0f, 0.0f, 1.0f))
			.Length(16.0f).Translation(vec2(0.0f, 10.0f));
		pSkeleton->AddBone().Name("frontFoot").Color(vec4(1.0f, 0.0f, 0.0f, 1.0f))
			.Length(16.0f).Translation(vec2(0.0f, 10.0f));
		pSkeleton->AddBone().Name("backArm").Color(vec4(0.0f, 0.5f, 0.0f, 1.0f))
			.Length(21.0f);
		pSkeleton->AddBone().Name("backHand").Color(vec4(0.0f, 0.5f, 0.0f, 1.0f)).Parent(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKARM))
			.Length(8.0f);
		pSkeleton->AddBone().Name("frontArm").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f))
			.Length(21.0f);
		pSkeleton->AddBone().Name("frontHand").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f)).Parent(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTARM))
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
		CModAPI_Asset_Skeleton* pSkeleton = m_SkeletonsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_HAMMER));
		pSkeleton->SetName("hammer");
		pSkeleton->m_DefaultSkinPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_HAMMER);
		pSkeleton->m_ParentPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
		pSkeleton->AddBone().Name("attach").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f)).Parent(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_BODY))
			.Translation(vec2(-12.0f, -16.0f))
			.Length(48.0f);
	}
	{
		CModAPI_Asset_Skeleton* pSkeleton = m_SkeletonsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_GUN));
		pSkeleton->SetName("gun");
		pSkeleton->m_DefaultSkinPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_GUN);
		pSkeleton->m_ParentPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
		pSkeleton->AddBone().Name("attach").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f)).Parent(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTARM))
			.Translation(vec2(0.0f, 0.0f))
			.Length(32.0f);
	}
	{
		CModAPI_Asset_Skeleton* pSkeleton = m_SkeletonsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_SHOTGUN));
		pSkeleton->SetName("shotgun");
		pSkeleton->m_DefaultSkinPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_SHOTGUN);
		pSkeleton->m_ParentPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
		pSkeleton->AddBone().Name("attach").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f)).Parent(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTARM))
			.Translation(vec2(0.0f, 0.0f))
			.Length(32.0f);
	}
	dbg_msg("assetsmanager", "Skeletons initialised");
	
	//Skeleton Skins
	{
		CModAPI_Asset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_TEEBODYDEFAULT));
		pSkeletonSkin->SetName("teeBodyDefault");
		pSkeletonSkin->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEBODYDEFAULT_SHADOW),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BODYSHADOW))
				.Anchor(0.0f);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEBODYDEFAULT_COLOR),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BODY))
				.Anchor(0.0f);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEBODYDEFAULT_SHADING),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_OUTLINE))
				.Anchor(0.0f);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEBODYDEFAULT_OUTLINE),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_OUTLINE))
				.Anchor(0.0f);
	}
	{
		CModAPI_Asset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_TEEFEETDEFAULT));
		pSkeletonSkin->SetName("teeFeetDefault");
		pSkeletonSkin->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEFEETDEFAULT_SHADOW),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKFOOTSHADOW))
			.Anchor(0.0f)
			.Scale(vec2(30.476f, 30.476f));
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEFEETDEFAULT_SHADOW),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTFOOTSHADOW))
			.Anchor(0.0f)
			.Scale(vec2(30.476f, 30.476f));
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEFEETDEFAULT_COLOR),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKFOOT))
			.Anchor(0.0f)
			.Scale(vec2(30.476f, 30.476f));
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEFEETDEFAULT_COLOR),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTFOOT))
			.Anchor(0.0f)
			.Scale(vec2(30.476f, 30.476f));
	}
	{
		CModAPI_Asset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_TEEHANDSDEFAULT));
		pSkeletonSkin->SetName("teeHandsDefault");
		pSkeletonSkin->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEHANDSDEFAULT_SHADOW),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKHAND),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKHANDSHADOW))
			.Anchor(0.0f)
			.Scale(vec2(30.0f, 30.0f));
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEHANDSDEFAULT_SHADOW),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTHAND),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTHANDSHADOW))
			.Anchor(0.0f)
			.Scale(vec2(30.0f, 30.0f));
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEHANDSDEFAULT_COLOR),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKHAND),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKHAND))
			.Anchor(0.0f)
			.Scale(vec2(30.0f, 30.0f));
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEHANDSDEFAULT_COLOR),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTHAND),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTHAND))
			.Anchor(0.0f)
			.Scale(vec2(30.0f, 30.0f));
	}
	{
		CModAPI_Asset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_TEEEYESDEFAULT));
		pSkeletonSkin->SetName("teeEyesDefault");
		pSkeletonSkin->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEEYESDEFAULT_NORMAL),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_EYES),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_NORMAL))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CModAPI_Asset_SkeletonSkin::ALIGNEMENT_WORLD);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEEYESDEFAULT_ANGRY),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_EYES),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_ANGRY))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CModAPI_Asset_SkeletonSkin::ALIGNEMENT_WORLD);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEEYESDEFAULT_PAIN),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_EYES),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_PAIN))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CModAPI_Asset_SkeletonSkin::ALIGNEMENT_WORLD);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEEYESDEFAULT_HAPPY),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_EYES),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_HAPPY))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CModAPI_Asset_SkeletonSkin::ALIGNEMENT_WORLD);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_TEEEYESDEFAULT_FEAR),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_EYES),
			CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_FEAR))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CModAPI_Asset_SkeletonSkin::ALIGNEMENT_WORLD);
	}
	{
		CModAPI_Asset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_HAMMER));
		pSkeletonSkin->SetName("hammer");
		pSkeletonSkin->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_HAMMER);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_HAMMER),
			CModAPI_Asset_Skeleton::CBonePath::Local(0),
			CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEELAYER_ATTACH))
			.Translation(vec2(-44.0f, 0.0f))
			.Scale(vec2(76.8f, 76.8f));
	}
	{
		CModAPI_Asset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_GUN));
		pSkeletonSkin->SetName("gun");
		pSkeletonSkin->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_GUN);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_GUN),
			CModAPI_Asset_Skeleton::CBonePath::Local(0),
			CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEELAYER_ATTACH))
			.Translation(vec2(-21.0f, 0.0f))
			.Scale(vec2(57.24f, 57.24f));
	}
	{
		CModAPI_Asset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_SHOTGUN));
		pSkeletonSkin->SetName("shotgun");
		pSkeletonSkin->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_SHOTGUN);
		pSkeletonSkin->AddSprite(
			CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_SHOTGUN),
			CModAPI_Asset_Skeleton::CBonePath::Local(0),
			CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEELAYER_ATTACH))
			.Translation(vec2(-21.0f, 0.0f))
			.Scale(vec2(93.13f, 93.13f));
	}
	dbg_msg("assetsmanager", "Skeleton Skins initialised");
	
	//Skeleton Animations
	{
		CModAPI_Asset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONANIMATION, MODAPI_SKELETONANIMATION_TEEIDLE));
		pSkeletonAnimation->SetName("teeIdle");
		pSkeletonAnimation->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
		
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT),  0).Translation(vec2(-7.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT), 0).Translation(vec2( 7.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKARM), 0).Alignment(CModAPI_Asset_SkeletonAnimation::BONEALIGN_HOOK);
		
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKHANDSHADOW), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTHANDSHADOW), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKHAND), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTHAND), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_ANGRY), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_PAIN), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_HAPPY), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_FEAR), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}
	
	{
		CModAPI_Asset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONANIMATION, MODAPI_SKELETONANIMATION_TEEAIR));
		pSkeletonAnimation->SetName("teeAir");
		pSkeletonAnimation->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
		
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT),  0).Translation(vec2(-3.0f,  0.0f)).Angle(-0.1f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT), 0).Translation(vec2( 3.0f,  0.0f)).Angle(-0.1f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKARM), 0).Alignment(CModAPI_Asset_SkeletonAnimation::BONEALIGN_HOOK);
		
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKHANDSHADOW), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTHANDSHADOW), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKHAND), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTHAND), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_ANGRY), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_PAIN), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_HAPPY), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_FEAR), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}
	
	{
		CModAPI_Asset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONANIMATION, MODAPI_SKELETONANIMATION_TEEAIR2));
		pSkeletonAnimation->SetName("teeAir2");
		pSkeletonAnimation->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
		
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT),  0).Translation(vec2(-3.0f,  0.0f)).Angle(-0.1f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT), 0).Translation(vec2( 3.0f,  0.0f)).Angle(-0.1f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKARM), 0).Alignment(CModAPI_Asset_SkeletonAnimation::BONEALIGN_HOOK);
		
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKHANDSHADOW), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTHANDSHADOW), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKHAND), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTHAND), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKFOOT), 0).Color(vec4(0.5f, 0.5f, 0.5f, 1.0f));
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTFOOT), 0).Color(vec4(0.5f, 0.5f, 0.5f, 1.0f));
		
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_ANGRY), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_PAIN), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_HAPPY), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_FEAR), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}
	
	{
		CModAPI_Asset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONANIMATION, MODAPI_SKELETONANIMATION_TEEWALK));
		pSkeletonAnimation->SetName("teeWalk");
		pSkeletonAnimation->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
		
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKARM), 0).Alignment(CModAPI_Asset_SkeletonAnimation::BONEALIGN_HOOK);
		
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY), 0 ).Translation(vec2(0.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY), 6 ).Translation(vec2(0.0f, -1.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY), 12).Translation(vec2(0.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY), 18).Translation(vec2(0.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY), 24).Translation(vec2(0.0f, -1.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY), 30).Translation(vec2(0.0f,  0.0f));
		pSkeletonAnimation->SetCycle(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY), CModAPI_Asset_SkeletonAnimation::CYCLETYPE_LOOP);
		
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT), 0 ).Translation(vec2(  8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT), 6 ).Translation(vec2( -8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT), 12).Translation(vec2(-10.0f, -4.0f)).Angle(0.2f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT), 18).Translation(vec2( -8.0f, -8.0f)).Angle(0.3f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT), 24).Translation(vec2(  4.0f, -4.0f)).Angle(-0.2f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT), 30).Translation(vec2(  8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->SetCycle(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT), CModAPI_Asset_SkeletonAnimation::CYCLETYPE_LOOP);
		
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT), 0 ).Translation(vec2(-10.0f, -4.0f)).Angle(0.2f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT), 6 ).Translation(vec2( -8.0f, -8.0f)).Angle(0.3f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT), 12).Translation(vec2(  4.0f, -4.0f)).Angle(-0.2f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT), 18).Translation(vec2(  8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT), 24).Translation(vec2(  8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT), 30).Translation(vec2(-10.0f, -4.0f)).Angle(0.2f*pi*2.0f);
		pSkeletonAnimation->SetCycle(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT), CModAPI_Asset_SkeletonAnimation::CYCLETYPE_LOOP);
		
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKHANDSHADOW), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTHANDSHADOW), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKHAND), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTHAND), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_ANGRY), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_PAIN), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_HAPPY), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_FEAR), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}
	{
		CModAPI_Asset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONANIMATION, MODAPI_SKELETONANIMATION_HAMMERATTACK));
		pSkeletonAnimation->SetName("hammerAttack");
		pSkeletonAnimation->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_HAMMER);
		
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(0), 0).Angle(-pi/2-0.10f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(0), 2).Angle(-pi/2+0.25f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(0), 4).Angle(-pi/2+0.30f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(0), 6).Angle(-pi/2+0.25f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(0), 9).Angle(-pi/2-0.10f*pi*2.0f);
		
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_EYES), 0).Alignment(CModAPI_Asset_SkeletonAnimation::BONEALIGN_AIM);
		
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEELAYER_FRONTHANDSHADOW), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEELAYER_FRONTHAND), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}
	{
		CModAPI_Asset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONANIMATION, MODAPI_SKELETONANIMATION_GUNATTACK));
		pSkeletonAnimation->SetName("gunAttack");
		pSkeletonAnimation->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_GUN);
		
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTARM), 0)
			.Alignment(CModAPI_Asset_SkeletonAnimation::BONEALIGN_AIM);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTARM), 2)
			.Alignment(CModAPI_Asset_SkeletonAnimation::BONEALIGN_AIM);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTARM), 4)
			.Alignment(CModAPI_Asset_SkeletonAnimation::BONEALIGN_AIM);
			
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTHAND), 0)
			.Angle(-3*pi/4)
			.Translation(vec2(0.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTHAND), 2)
			.Angle(-3*pi/4)
			.Translation(vec2(-10.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTHAND), 4)
			.Angle(-3*pi/4)
			.Translation(vec2(0.0f, 0.0f));
			
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(0), 0)
			.Translation(vec2(0.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(0), 2)
			.Translation(vec2(-10.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(0), 4)
			.Translation(vec2(0.0f, 0.0f));
		
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_EYES), 0).Alignment(CModAPI_Asset_SkeletonAnimation::BONEALIGN_AIM);
		
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEELAYER_FRONTHANDSHADOW), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_VISIBLE);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEELAYER_FRONTHAND), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_VISIBLE);
	}
	{
		CModAPI_Asset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONANIMATION, MODAPI_SKELETONANIMATION_SHOTGUNATTACK));
		pSkeletonAnimation->SetName("shotgunAttack");
		pSkeletonAnimation->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_SHOTGUN);
		
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTARM), 0)
			.Alignment(CModAPI_Asset_SkeletonAnimation::BONEALIGN_AIM);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTARM), 2)
			.Alignment(CModAPI_Asset_SkeletonAnimation::BONEALIGN_AIM);
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTARM), 4)
			.Alignment(CModAPI_Asset_SkeletonAnimation::BONEALIGN_AIM);
			
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTHAND), 0)
			.Angle(-pi/2)
			.Translation(vec2(0.0f, 6.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTHAND), 2)
			.Angle(-pi/2)
			.Translation(vec2(-10.0f, 6.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_FRONTHAND), 4)
			.Angle(-pi/2)
			.Translation(vec2(0.0f, 6.0f));
			
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(0), 0)
			.Translation(vec2(0.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(0), 2)
			.Translation(vec2(-10.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Local(0), 4)
			.Translation(vec2(0.0f, 0.0f));
		
		pSkeletonAnimation->AddBoneKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEEBONE_EYES), 0).Alignment(CModAPI_Asset_SkeletonAnimation::BONEALIGN_AIM);
		
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEELAYER_FRONTHANDSHADOW), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_VISIBLE);
		pSkeletonAnimation->AddLayerKeyFrame(CModAPI_Asset_Skeleton::CBonePath::Parent(MODAPI_TEELAYER_FRONTHAND), 0).State(CModAPI_Asset_SkeletonAnimation::LAYERSTATE_VISIBLE);
	}
	dbg_msg("assetsmanager", "Skeleton Animations initialised");
	
	//Character
	{
		CModAPI_Asset_Character* pCharacter = m_CharactersCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTER, MODAPI_CHARACTER_TEE));
		pCharacter->SetName("tee");
		
		pCharacter->AddPart().Name("body").Default(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTERPART, MODAPI_CHARACTERPART_TEEBODYDEFAULT));
		pCharacter->AddPart().Name("marking");
		pCharacter->AddPart().Name("decoration");
		pCharacter->AddPart().Name("hands").Default(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTERPART, MODAPI_CHARACTERPART_TEEHANDSDEFAULT));
		pCharacter->AddPart().Name("feet").Default(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTERPART, MODAPI_CHARACTERPART_TEEFEETDEFAULT));
		pCharacter->AddPart().Name("eyes").Default(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTERPART, MODAPI_CHARACTERPART_TEEEYESDEFAULT));
		
		pCharacter->m_IdlePath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONANIMATION, MODAPI_SKELETONANIMATION_TEEIDLE);
		pCharacter->m_WalkPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONANIMATION, MODAPI_SKELETONANIMATION_TEEWALK);
		pCharacter->m_ControlledJumpPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONANIMATION, MODAPI_SKELETONANIMATION_TEEAIR);
		pCharacter->m_UncontrolledJumpPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONANIMATION, MODAPI_SKELETONANIMATION_TEEAIR2);
	}
	dbg_msg("assetsmanager", "Characters initialised");
	
	//CharacterPart
	{
		CModAPI_Asset_CharacterPart* pCharacterPart = m_CharacterPartsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTERPART, MODAPI_CHARACTERPART_TEEBODYDEFAULT));
		pCharacterPart->SetName("teeBodyDefault");
		pCharacterPart->m_CharacterPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTER, MODAPI_CHARACTER_TEE);
		pCharacterPart->m_CharacterPart = CModAPI_Asset_Character::CSubPath::Part(MODAPI_SKINPART_BODY);
		pCharacterPart->m_SkeletonSkinPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_TEEBODYDEFAULT);
	}
	{
		CModAPI_Asset_CharacterPart* pCharacterPart = m_CharacterPartsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTERPART, MODAPI_CHARACTERPART_TEEFEETDEFAULT));
		pCharacterPart->SetName("teeFeetDefault");
		pCharacterPart->m_CharacterPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTER, MODAPI_CHARACTER_TEE);
		pCharacterPart->m_CharacterPart = CModAPI_Asset_Character::CSubPath::Part(MODAPI_SKINPART_FEET);
		pCharacterPart->m_SkeletonSkinPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_TEEFEETDEFAULT);
	}
	{
		CModAPI_Asset_CharacterPart* pCharacterPart = m_CharacterPartsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTERPART, MODAPI_CHARACTERPART_TEEEYESDEFAULT));
		pCharacterPart->SetName("teeEyesDefault");
		pCharacterPart->m_CharacterPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTER, MODAPI_CHARACTER_TEE);
		pCharacterPart->m_CharacterPart = CModAPI_Asset_Character::CSubPath::Part(MODAPI_SKINPART_EYES);
		pCharacterPart->m_SkeletonSkinPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_TEEEYESDEFAULT);
	}
	dbg_msg("assetsmanager", "Character parts initialized");
	
	//Weapons
	{
		CModAPI_Asset_Weapon* pWeapon = m_WeaponsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_WEAPON, MODAPI_WEAPON_HAMMER));
		pWeapon->SetName("hammer");
		pWeapon->m_CharacterPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTER, MODAPI_CHARACTER_TEE);
		pWeapon->m_CursorPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_HAMMER_CURSOR);
		pWeapon->m_SkinPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_HAMMER);
		pWeapon->m_AttackAnimationPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONANIMATION, MODAPI_SKELETONANIMATION_HAMMERATTACK);
	}
	{
		CModAPI_Asset_Weapon* pWeapon = m_WeaponsCatalog.NewAsset(CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_WEAPON, MODAPI_WEAPON_GUN));
		pWeapon->SetName("gun");
		pWeapon->m_CharacterPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTER, MODAPI_CHARACTER_TEE);
		pWeapon->m_CursorPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SPRITE, MODAPI_SPRITE_GUN_CURSOR);
		pWeapon->m_SkinPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONSKIN, MODAPI_SKELETONSKIN_GUN);
		pWeapon->m_AttackAnimationPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETONANIMATION, MODAPI_SKELETONANIMATION_GUNATTACK);
	}
	dbg_msg("assetsmanager", "Weapons initialised");
}

int CModAPI_AssetManager::LoadSkinAssets_BodyScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CModAPI_AssetManager *pSelf = (CModAPI_AssetManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;
	
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/body/%s", pFilename);
	
	CModAPI_AssetPath ImagePath = pSelf->AddImage(IStorage::TYPE_ALL, aBuf, CModAPI_AssetPath::SRC_SKIN);
	CModAPI_Asset_Image* pImage = pSelf->GetAsset<CModAPI_Asset_Image>(ImagePath);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "body/%s.png", aName);
		pImage->SetName(aBuf);
		
		pImage->m_GridWidth = 2;
		pImage->m_GridHeight = 2;
	
		CModAPI_AssetPath ShadowPath;
		CModAPI_AssetPath ColorPath;
		CModAPI_AssetPath ShadingPath;
		CModAPI_AssetPath OutinePath;
		CModAPI_AssetPath SkeletonSkinPath;
		{
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ShadowPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "body/%s/shadow", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 1, 1);
		}
		{
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ColorPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "body/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ShadingPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "body/%s/shading", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 1, 1, 1);
		}
		{
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&OutinePath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "body/%s/outline", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 1, 1, 1);
		}
		{
			CModAPI_Asset_SkeletonSkin* pSkeletonSkin = pSelf->m_SkeletonSkinsCatalog.NewAsset(&SkeletonSkinPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "body/%s", aName);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BODYSHADOW))
				.Anchor(0.0f);
			pSkeletonSkin->AddSprite(
				ColorPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BODY))
				.Anchor(0.0f);
			pSkeletonSkin->AddSprite(
				ShadingPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_OUTLINE))
				.Anchor(0.0f);
			pSkeletonSkin->AddSprite(
				OutinePath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_OUTLINE))
				.Anchor(0.0f);
		}
		{
			CModAPI_AssetPath CharacterPartPath;
			CModAPI_Asset_CharacterPart* pCharacterPart = pSelf->m_CharacterPartsCatalog.NewAsset(&CharacterPartPath, CModAPI_AssetPath::SRC_SKIN);
			pCharacterPart->SetName(aName);
			pCharacterPart->m_CharacterPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTER, MODAPI_CHARACTER_TEE);
			pCharacterPart->m_CharacterPart = CModAPI_Asset_Character::CSubPath::Part(MODAPI_SKINPART_BODY);
			pCharacterPart->m_SkeletonSkinPath = SkeletonSkinPath;
		}
	}
	
	return 0;
}

int CModAPI_AssetManager::LoadSkinAssets_FootScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CModAPI_AssetManager *pSelf = (CModAPI_AssetManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;
	
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/feet/%s", pFilename);
	
	CModAPI_AssetPath ImagePath = pSelf->AddImage(IStorage::TYPE_ALL, aBuf, CModAPI_AssetPath::SRC_SKIN);
	CModAPI_Asset_Image* pImage = pSelf->GetAsset<CModAPI_Asset_Image>(ImagePath);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "feet/%s.png", aName);
		pImage->SetName(aBuf);
		
		pImage->m_GridWidth = 2;
		pImage->m_GridHeight = 1;
	
		CModAPI_AssetPath ShadowPath;
		CModAPI_AssetPath ColorPath;
		CModAPI_AssetPath SkeletonSkinPath;
		{
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ShadowPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "feet/%s/shadow", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ColorPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "feet/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 1, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "feet/%s", aName);
			CModAPI_Asset_SkeletonSkin* pSkeletonSkin = pSelf->m_SkeletonSkinsCatalog.NewAsset(&SkeletonSkinPath, CModAPI_AssetPath::SRC_SKIN);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKFOOTSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(30.476f, 30.476f));
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTFOOTSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(30.476f, 30.476f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKFOOT),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKFOOT))
				.Anchor(0.0f)
				.Scale(vec2(30.476f, 30.476f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTFOOT),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTFOOT))
				.Anchor(0.0f)
				.Scale(vec2(30.476f, 30.476f));
		}
		{
			CModAPI_AssetPath CharacterPartPath;
			CModAPI_Asset_CharacterPart* pCharacterPart = pSelf->m_CharacterPartsCatalog.NewAsset(&CharacterPartPath, CModAPI_AssetPath::SRC_SKIN);
			pCharacterPart->SetName(aName);
			pCharacterPart->m_CharacterPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTER, MODAPI_CHARACTER_TEE);
			pCharacterPart->m_CharacterPart = CModAPI_Asset_Character::CSubPath::Part(MODAPI_SKINPART_FEET);
			pCharacterPart->m_SkeletonSkinPath = SkeletonSkinPath;
		}
	}
	
	return 0;
}

int CModAPI_AssetManager::LoadSkinAssets_HandsScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CModAPI_AssetManager *pSelf = (CModAPI_AssetManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;
	
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/hands/%s", pFilename);
	
	CModAPI_AssetPath ImagePath = pSelf->AddImage(IStorage::TYPE_ALL, aBuf, CModAPI_AssetPath::SRC_SKIN);
	CModAPI_Asset_Image* pImage = pSelf->GetAsset<CModAPI_Asset_Image>(ImagePath);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "hands/%s.png", aName);
		pImage->SetName(aBuf);
		
		pImage->m_GridWidth = 2;
		pImage->m_GridHeight = 1;
	
		CModAPI_AssetPath ShadowPath;
		CModAPI_AssetPath ColorPath;
		CModAPI_AssetPath SkeletonSkinPath;
		{
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ShadowPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "hands/%s/shadow", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ColorPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "hands/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 1, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "hands/%s", aName);
			CModAPI_Asset_SkeletonSkin* pSkeletonSkin = pSelf->m_SkeletonSkinsCatalog.NewAsset(&SkeletonSkinPath, CModAPI_AssetPath::SRC_SKIN);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKHAND),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKHANDSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(30.0f, 30.0f));
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTHAND),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTHANDSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(30.0f, 30.0f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BACKHAND),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BACKHAND))
				.Anchor(0.0f)
				.Scale(vec2(30.0f, 30.0f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_FRONTHAND),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_FRONTHAND))
				.Anchor(0.0f)
				.Scale(vec2(30.0f, 30.0f));
		}
		{
			CModAPI_AssetPath CharacterPartPath;
			CModAPI_Asset_CharacterPart* pCharacterPart = pSelf->m_CharacterPartsCatalog.NewAsset(&CharacterPartPath, CModAPI_AssetPath::SRC_SKIN);
			pCharacterPart->SetName(aName);
			pCharacterPart->m_CharacterPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTER, MODAPI_CHARACTER_TEE);
			pCharacterPart->m_CharacterPart = CModAPI_Asset_Character::CSubPath::Part(MODAPI_SKINPART_HANDS);
			pCharacterPart->m_SkeletonSkinPath = SkeletonSkinPath;
		}
	}
	
	return 0;
}

int CModAPI_AssetManager::LoadSkinAssets_MarkingScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CModAPI_AssetManager *pSelf = (CModAPI_AssetManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;
	
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/marking/%s", pFilename);
	
	CModAPI_AssetPath ImagePath = pSelf->AddImage(IStorage::TYPE_ALL, aBuf, CModAPI_AssetPath::SRC_SKIN);
	CModAPI_Asset_Image* pImage = pSelf->GetAsset<CModAPI_Asset_Image>(ImagePath);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "marking/%s.png", aName);
		pImage->SetName(aBuf);
		
		pImage->m_GridWidth = 1;
		pImage->m_GridHeight = 1;
	
		CModAPI_AssetPath ColorPath;
		CModAPI_AssetPath SkeletonSkinPath;
		{
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ColorPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "marking/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "marking/%s", aName);
			CModAPI_Asset_SkeletonSkin* pSkeletonSkin = pSelf->m_SkeletonSkinsCatalog.NewAsset(&SkeletonSkinPath, CModAPI_AssetPath::SRC_SKIN);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ColorPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_MARKING))
				.Anchor(0.0f)
				.Scale(vec2(64.0f, 64.0f));
		}
		{
			CModAPI_AssetPath CharacterPartPath;
			CModAPI_Asset_CharacterPart* pCharacterPart = pSelf->m_CharacterPartsCatalog.NewAsset(&CharacterPartPath, CModAPI_AssetPath::SRC_SKIN);
			pCharacterPart->SetName(aName);
			pCharacterPart->m_CharacterPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTER, MODAPI_CHARACTER_TEE);
			pCharacterPart->m_CharacterPart = CModAPI_Asset_Character::CSubPath::Part(MODAPI_SKINPART_MARKING);
			pCharacterPart->m_SkeletonSkinPath = SkeletonSkinPath;
		}
	}
	
	return 0;
}

int CModAPI_AssetManager::LoadSkinAssets_DecorationScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CModAPI_AssetManager *pSelf = (CModAPI_AssetManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;
	
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/decoration/%s", pFilename);
	
	CModAPI_AssetPath ImagePath = pSelf->AddImage(IStorage::TYPE_ALL, aBuf, CModAPI_AssetPath::SRC_SKIN);
	CModAPI_Asset_Image* pImage = pSelf->GetAsset<CModAPI_Asset_Image>(ImagePath);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "decoration/%s.png", aName);
		pImage->SetName(aBuf);
		
		pImage->m_GridWidth = 2;
		pImage->m_GridHeight = 1;
	
		CModAPI_AssetPath ShadowPath;
		CModAPI_AssetPath ColorPath;
		CModAPI_AssetPath SkeletonSkinPath;
		{
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ShadowPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "decoration/%s/shadow", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ColorPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "decoration/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 1, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "decoration/%s", aName);
			CModAPI_Asset_SkeletonSkin* pSkeletonSkin = pSelf->m_SkeletonSkinsCatalog.NewAsset(&SkeletonSkinPath, CModAPI_AssetPath::SRC_SKIN);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_BODYSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(64.0f, 64.0f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_BODY),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_DECORATION))
				.Anchor(0.0f)
				.Scale(vec2(64.0f, 64.0f));
		}
		{
			CModAPI_AssetPath CharacterPartPath;
			CModAPI_Asset_CharacterPart* pCharacterPart = pSelf->m_CharacterPartsCatalog.NewAsset(&CharacterPartPath, CModAPI_AssetPath::SRC_SKIN);
			pCharacterPart->SetName(aName);
			pCharacterPart->m_CharacterPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTER, MODAPI_CHARACTER_TEE);
			pCharacterPart->m_CharacterPart = CModAPI_Asset_Character::CSubPath::Part(MODAPI_SKINPART_DECORATION);
			pCharacterPart->m_SkeletonSkinPath = SkeletonSkinPath;
		}
	}
	
	return 0;
}

int CModAPI_AssetManager::LoadSkinAssets_EyeScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CModAPI_AssetManager *pSelf = (CModAPI_AssetManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;
	
	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/eyes/%s", pFilename);
	
	CModAPI_AssetPath ImagePath = pSelf->AddImage(IStorage::TYPE_ALL, aBuf, CModAPI_AssetPath::SRC_SKIN);
	CModAPI_Asset_Image* pImage = pSelf->GetAsset<CModAPI_Asset_Image>(ImagePath);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "eyes/%s.png", aName);
		pImage->SetName(aBuf);
		
		pImage->m_GridWidth = 4;
		pImage->m_GridHeight = 4;
	
		CModAPI_AssetPath NormalPath;
		CModAPI_AssetPath AngryPath;
		CModAPI_AssetPath PainPath;
		CModAPI_AssetPath HappyPath;
		CModAPI_AssetPath FearPath;
		CModAPI_AssetPath SkeletonSkinPath;
		{
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&NormalPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/normal", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 2, 1);
		}
		{
			CModAPI_AssetPath SpritePath;
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&AngryPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/angry", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 2, 0, 2, 1);
		}
		{
			CModAPI_AssetPath SpritePath;
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&PainPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/pain", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 1, 2, 1);
		}
		{
			CModAPI_AssetPath SpritePath;
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&HappyPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/happy", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 2, 1, 2, 1);
		}
		{
			CModAPI_AssetPath SpritePath;
			CModAPI_Asset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&FearPath, CModAPI_AssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/fear", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 2, 2, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "eyes/%s", aName);
			CModAPI_Asset_SkeletonSkin* pSkeletonSkin = pSelf->m_SkeletonSkinsCatalog.NewAsset(&SkeletonSkinPath, CModAPI_AssetPath::SRC_SKIN);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_SKELETON, MODAPI_SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				NormalPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_EYES),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_NORMAL))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CModAPI_Asset_SkeletonSkin::ALIGNEMENT_WORLD);
			pSkeletonSkin->AddSprite(
				AngryPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_EYES),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_ANGRY))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CModAPI_Asset_SkeletonSkin::ALIGNEMENT_WORLD);
			pSkeletonSkin->AddSprite(
				PainPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_EYES),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_PAIN))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CModAPI_Asset_SkeletonSkin::ALIGNEMENT_WORLD);
			pSkeletonSkin->AddSprite(
				HappyPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_EYES),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_HAPPY))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CModAPI_Asset_SkeletonSkin::ALIGNEMENT_WORLD);
			pSkeletonSkin->AddSprite(
				FearPath,
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEEBONE_EYES),
				CModAPI_Asset_Skeleton::CBonePath::Local(MODAPI_TEELAYER_EYES_FEAR))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CModAPI_Asset_SkeletonSkin::ALIGNEMENT_WORLD);
		}
		{
			CModAPI_AssetPath CharacterPartPath;
			CModAPI_Asset_CharacterPart* pCharacterPart = pSelf->m_CharacterPartsCatalog.NewAsset(&CharacterPartPath, CModAPI_AssetPath::SRC_SKIN);
			pCharacterPart->SetName(aName);
			pCharacterPart->m_CharacterPath = CModAPI_AssetPath::Internal(CModAPI_AssetPath::TYPE_CHARACTER, MODAPI_CHARACTER_TEE);
			pCharacterPart->m_CharacterPart = CModAPI_Asset_Character::CSubPath::Part(MODAPI_SKINPART_EYES);
			pCharacterPart->m_SkeletonSkinPath = SkeletonSkinPath;
		}
	}
	
	return 0;
}

void CModAPI_AssetManager::LoadSkinAssets(IStorage* pStorage)
{
	dbg_msg("assetsmanager", "load skin assets");
	Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/body", CModAPI_AssetManager::LoadSkinAssets_BodyScan, this);
	dbg_msg("assetsmanager", "body assets loaded");
	Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/feet", CModAPI_AssetManager::LoadSkinAssets_FootScan, this);
	dbg_msg("assetsmanager", "feet assets loaded");
	Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/eyes", CModAPI_AssetManager::LoadSkinAssets_EyeScan, this);
	dbg_msg("assetsmanager", "eyes assets loaded");
	Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/hands", CModAPI_AssetManager::LoadSkinAssets_HandsScan, this);
	dbg_msg("assetsmanager", "hands assets loaded");
	Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/marking", CModAPI_AssetManager::LoadSkinAssets_MarkingScan, this);
	dbg_msg("assetsmanager", "marking assets loaded");
	Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/decoration", CModAPI_AssetManager::LoadSkinAssets_DecorationScan, this);
	dbg_msg("assetsmanager", "decoration assets loaded");
}

CModAPI_AssetPath CModAPI_AssetManager::FindSkinPart(CModAPI_AssetPath CharacterPath, CModAPI_Asset_Character::CSubPath CharacterPart, const char* pName)
{
	char aAssetName[256];
	str_format(aAssetName, sizeof(aAssetName), "%s", pName);
	
	for(int i=0; i<m_CharacterPartsCatalog.m_Assets[CModAPI_AssetPath::SRC_SKIN].size(); i++)
	{
		const CModAPI_Asset_CharacterPart* pCharacterPart = &m_CharacterPartsCatalog.m_Assets[CModAPI_AssetPath::SRC_SKIN][i];
		if(
			str_comp(pCharacterPart->m_aName, aAssetName) == 0 &&
			pCharacterPart->m_CharacterPath == CharacterPath &&
			pCharacterPart->m_CharacterPart == CharacterPart
		)
		{
			return CModAPI_AssetPath::Skin(CModAPI_AssetPath::TYPE_CHARACTERPART, i);
		}
	}
	
	return CModAPI_AssetPath::Null();
}

CModAPI_AssetPath CModAPI_AssetManager::AddImage(int StorageType, const char* pFilename, int Source)
{
	char aCompleteFilename[512];
	unsigned char *pBuffer;
	png_t Png; // ignore_convention

	// open file for reading
	png_init(0,0); // ignore_convention

	IOHANDLE File = Storage()->OpenFile(pFilename, IOFLAG_READ, StorageType, aCompleteFilename, sizeof(aCompleteFilename));
	if(File)
	{
		io_close(File);
	}
	else
	{
		dbg_msg("mod", "failed to open file. filename='%s'", pFilename);
		return CModAPI_AssetPath::Null();
	}

	int Error = png_open_file(&Png, aCompleteFilename); // ignore_convention
	if(Error != PNG_NO_ERROR)
	{
		dbg_msg("mod", "failed to open file. filename='%s'", aCompleteFilename);
		if(Error != PNG_FILE_ERROR)
		{
			png_close_file(&Png); // ignore_convention
		}
		return CModAPI_AssetPath::Null();
	}

	if(Png.depth != 8 || (Png.color_type != PNG_TRUECOLOR && Png.color_type != PNG_TRUECOLOR_ALPHA) || Png.width > (2<<12) || Png.height > (2<<12)) // ignore_convention
	{
		dbg_msg("mod", "invalid format. filename='%s'", aCompleteFilename);
		png_close_file(&Png); // ignore_convention
		return CModAPI_AssetPath::Null();
	}

	pBuffer = (unsigned char *) mem_alloc(Png.width * Png.height * Png.bpp, 1); // ignore_convention
	png_get_data(&Png, pBuffer); // ignore_convention
	png_close_file(&Png); // ignore_convention

	int Format;
	if(Png.color_type == PNG_TRUECOLOR) 
		Format = CImageInfo::FORMAT_RGB;
	else if(Png.color_type == PNG_TRUECOLOR_ALPHA)
		Format = CImageInfo::FORMAT_RGBA;
	
	CModAPI_AssetPath Path;
	CModAPI_Asset_Image* pImage = m_ImagesCatalog.NewAsset(&Path, Source);
	pImage->SetName(pFilename);
	pImage->m_Width = Png.width;
	pImage->m_Height = Png.height;
	pImage->m_Format = Format;
	pImage->m_pData = pBuffer;
	pImage->m_Texture = Graphics()->LoadTextureRaw(Png.width, Png.height, Format, pBuffer, CImageInfo::FORMAT_AUTO, IGraphics::TEXLOAD_MULTI_DIMENSION);

	return Path;
}

int CModAPI_AssetManager::SaveInAssetsFile(const char *pFileName, int Source)
{
	CDataFileWriter df;
	if(!df.Open(Storage(), pFileName))
	{
		dbg_msg("ModAPIGraphics", "can't create the assets file %s", pFileName);
		return 0;
	}
	
	{
		CStorageType Item;
		Item.m_Version = 0;
		Item.m_Source = Source;
		
		df.AddItem(CModAPI_Asset_Image::TypeId, 0, sizeof(CStorageType), &Item);
	}
	
	#define MODAPI_MACRO_ASSETTYPE(ClassName, CatalogName) CatalogName.SaveInAssetsFile(&df, Source);
	#include <modapi/client/assetsmacro.h>
	#undef MODAPI_MACRO_ASSETTYPE
	
	df.Finish();
	
	return 1;
}

int CModAPI_AssetManager::OnAssetsFileLoaded(IModAPI_AssetsFile* pAssetsFile)
{
	int Start, Num;
	pAssetsFile->GetType(0, &Start, &Num);
	
	int Source = CModAPI_AssetPath::SRC_EXTERNAL;
	if(Num > 0)
	{
		CStorageType* pItem = (CStorageType*) pAssetsFile->GetItem(Start, 0, 0);
		Source = pItem->m_Source % CModAPI_AssetPath::NUM_SOURCES;
	}
	
	#define MODAPI_MACRO_ASSETTYPE(ClassName, CatalogName) CatalogName.LoadFromAssetsFile(this, pAssetsFile, Source);
	#include <modapi/client/assetsmacro.h>
	#undef MODAPI_MACRO_ASSETTYPE
	
	return 1;
}

int CModAPI_AssetManager::OnAssetsFileUnloaded()
{
	#define MODAPI_MACRO_ASSETTYPE(ClassName, CatalogName) CatalogName.Unload(this);
	#include <modapi/client/assetsmacro.h>
	#undef MODAPI_MACRO_ASSETTYPE
}
	
void CModAPI_AssetManager::DeleteAsset(const CModAPI_AssetPath& Path)
{
	#define MODAPI_MACRO_ASSETTYPE(ClassName, CatalogName) CatalogName.DeleteAsset(Path);
	#include <modapi/client/assetsmacro.h>
	#undef MODAPI_MACRO_ASSETTYPE
	
	#define MODAPI_MACRO_ASSETTYPE(ClassName, CatalogName) CatalogName.OnAssetDeleted(Path);
	#include <modapi/client/assetsmacro.h>
	#undef MODAPI_MACRO_ASSETTYPE
}
	
int CModAPI_AssetManager::AddSubItem(CModAPI_AssetPath AssetPath, int SubItemType)
{
	#define MODAPI_MACRO_ASSETTYPE(ClassName, CatalogName) case ClassName::TypeId:\
	{\
		ClassName* pAsset = GetAsset<ClassName>(AssetPath);\
		if(pAsset)\
			return pAsset->AddSubItem(SubItemType);\
		else\
			return -1;\
	}

	switch(AssetPath.GetType())
	{
		#include <modapi/client/assetsmacro.h>
	}
	
	#undef MODAPI_MACRO_ASSETTYPE
	
	return -1;
}
	
void CModAPI_AssetManager::DeleteSubItem(CModAPI_AssetPath AssetPath, int SubPath)
{
	#define MODAPI_MACRO_ASSETTYPE(ClassName, CatalogName) case ClassName::TypeId:\
	{\
		ClassName* pAsset = GetAsset<ClassName>(AssetPath);\
		if(pAsset)\
			pAsset->DeleteSubItem(SubPath);\
		break;\
	}

	switch(AssetPath.GetType())
	{
		#include <modapi/client/assetsmacro.h>
	}
	#undef MODAPI_MACRO_ASSETTYPE
	
	#define MODAPI_MACRO_ASSETTYPE(ClassName, CatalogName) CatalogName.OnSubItemDeleted(AssetPath, SubPath);
	#include <modapi/client/assetsmacro.h>
	#undef MODAPI_MACRO_ASSETTYPE
}
