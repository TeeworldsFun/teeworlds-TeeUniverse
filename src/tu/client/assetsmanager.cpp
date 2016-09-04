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

CAssetsManager::CAssetsManager(IGraphics* pGraphics, IStorage* pStorage)
{
	m_pGraphics = pGraphics;
	m_pStorage = pStorage;
}

#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) template<>\
CAssetCatalog<ClassName>* CAssetsManager::GetAssetCatalog<ClassName>()\
{\
	return &CatalogName;\
}

#include <tu/client/assetsmacro.h>

#undef TU_MACRO_ASSETTYPE

void CAssetsManager::Init(IStorage* pStorage)
{
	LoadInteralAssets();
	LoadSkinAssets(pStorage);
}

void CAssetsManager::LoadInteralAssets()
{
	//Images
	{
		CAssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "game.png", CAssetPath::SRC_UNIVERSE);
		CAsset_Image* pImage = GetAsset<CAsset_Image>(ImagePath);
		if(pImage)
		{
			pImage->m_GridWidth = 32;
			pImage->m_GridHeight = 16;
		}
	}
	{
		CAssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "particles.png", CAssetPath::SRC_UNIVERSE);
		CAsset_Image* pImage = GetAsset<CAsset_Image>(ImagePath);
		if(pImage)
		{
			pImage->m_GridWidth = 8;
			pImage->m_GridHeight = 8;
		}
	}
	{
		CAssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "skins/body/standard.png", CAssetPath::SRC_UNIVERSE);
		CAsset_Image* pImage = GetAsset<CAsset_Image>(ImagePath);
		if(pImage)
		{
			pImage->m_GridWidth = 2;
			pImage->m_GridHeight = 2;
		}
	}
	{
		CAssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "skins/feet/standard.png", CAssetPath::SRC_UNIVERSE);
		CAsset_Image* pImage = GetAsset<CAsset_Image>(ImagePath);
		if(pImage)
		{
			pImage->m_GridWidth = 2;
			pImage->m_GridHeight = 1;
		}
	}
	{
		CAssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "skins/hands/standard.png", CAssetPath::SRC_UNIVERSE);
		CAsset_Image* pImage = GetAsset<CAsset_Image>(ImagePath);
		if(pImage)
		{
			pImage->m_GridWidth = 2;
			pImage->m_GridHeight = 1;
		}
	}
	{
		CAssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "skins/eyes/standard.png", CAssetPath::SRC_UNIVERSE);
		CAsset_Image* pImage = GetAsset<CAsset_Image>(ImagePath);
		if(pImage)
		{
			pImage->m_GridWidth = 4;
			pImage->m_GridHeight = 4;
		}
	}
	{
		CAssetPath ImagePath = AddImage(IStorage::TYPE_ALL, "skins/hands/standard.png", CAssetPath::SRC_UNIVERSE);
		CAsset_Image* pImage = GetAsset<CAsset_Image>(ImagePath);
		if(pImage)
		{
			pImage->m_GridWidth = 2;
			pImage->m_GridHeight = 1;
		}
	}
	dbg_msg("assetsmanager", "Images initialised");

	#define CREATE_INTERNAL_SPRITE(id, name, image, x, y, w, h) {\
		CAsset_Sprite* pSprite = m_SpritesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SPRITE, id));\
		pSprite->SetName(name);\
		pSprite->Init(CAssetPath::Universe(CAssetPath::TYPE_IMAGE, image), x, y, w, h);\
	}

	{
		CAsset_Sprite* pSprite = m_SpritesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_WHITESQUARE));
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
		CAsset_Skeleton* pSkeleton = m_SkeletonsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE));
		pSkeleton->SetName("tee");
		pSkeleton->AddBone().Name("body").Color(vec4(1.0f, 0.0f, 1.0f, 1.0f))
			.Length(16.0f).Translation(vec2(0.0f, -4.0f));
		pSkeleton->AddBone().Name("eyes").Color(vec4(0.0f, 1.0f, 1.0f, 1.0f)).Parent(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY))
			.Length(8.0f).Translation(vec2(-16.0f, -3.2f)).Angle(-pi/4.0f);
		pSkeleton->AddBone().Name("backFoot").Color(vec4(0.5f, 0.0f, 0.0f, 1.0f))
			.Length(16.0f).Translation(vec2(0.0f, 10.0f));
		pSkeleton->AddBone().Name("frontFoot").Color(vec4(1.0f, 0.0f, 0.0f, 1.0f))
			.Length(16.0f).Translation(vec2(0.0f, 10.0f));
		pSkeleton->AddBone().Name("backArm").Color(vec4(0.0f, 0.5f, 0.0f, 1.0f))
			.Length(21.0f);
		pSkeleton->AddBone().Name("backHand").Color(vec4(0.0f, 0.5f, 0.0f, 1.0f)).Parent(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKARM))
			.Length(8.0f);
		pSkeleton->AddBone().Name("frontArm").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f))
			.Length(21.0f);
		pSkeleton->AddBone().Name("frontHand").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f)).Parent(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTARM))
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
		CAsset_Skeleton* pSkeleton = m_SkeletonsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_HAMMER));
		pSkeleton->SetName("hammer");
		pSkeleton->m_DefaultSkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_HAMMER);
		pSkeleton->m_ParentPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeleton->AddBone().Name("attach").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f)).Parent(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_BODY))
			.Translation(vec2(-12.0f, -16.0f))
			.Length(48.0f);
	}
	{
		CAsset_Skeleton* pSkeleton = m_SkeletonsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_GUN));
		pSkeleton->SetName("gun");
		pSkeleton->m_DefaultSkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_GUN);
		pSkeleton->m_ParentPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeleton->AddBone().Name("attach").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f)).Parent(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTARM))
			.Translation(vec2(0.0f, 0.0f))
			.Length(32.0f);
	}
	{
		CAsset_Skeleton* pSkeleton = m_SkeletonsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_SHOTGUN));
		pSkeleton->SetName("shotgun");
		pSkeleton->m_DefaultSkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_SHOTGUN);
		pSkeleton->m_ParentPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeleton->AddBone().Name("attach").Color(vec4(0.0f, 1.0f, 0.0f, 1.0f)).Parent(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTARM))
			.Translation(vec2(0.0f, 0.0f))
			.Length(32.0f);
	}
	dbg_msg("assetsmanager", "Skeletons initialised");

	//Skeleton Skins
	{
		CAsset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEBODYDEFAULT));
		pSkeletonSkin->SetName("teeBodyDefault");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEBODYDEFAULT_SHADOW),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BODYSHADOW))
				.Anchor(0.0f);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEBODYDEFAULT_COLOR),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BODY))
				.Anchor(0.0f);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEBODYDEFAULT_SHADING),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_OUTLINE))
				.Anchor(0.0f);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEBODYDEFAULT_OUTLINE),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_OUTLINE))
				.Anchor(0.0f);
	}
	{
		CAsset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEFEETDEFAULT));
		pSkeletonSkin->SetName("teeFeetDefault");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEFEETDEFAULT_SHADOW),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKFOOTSHADOW))
			.Anchor(0.0f)
			.Scale(vec2(30.476f, 30.476f));
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEFEETDEFAULT_SHADOW),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTFOOTSHADOW))
			.Anchor(0.0f)
			.Scale(vec2(30.476f, 30.476f));
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEFEETDEFAULT_COLOR),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKFOOT))
			.Anchor(0.0f)
			.Scale(vec2(30.476f, 30.476f));
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEFEETDEFAULT_COLOR),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTFOOT))
			.Anchor(0.0f)
			.Scale(vec2(30.476f, 30.476f));
	}
	{
		CAsset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEHANDSDEFAULT));
		pSkeletonSkin->SetName("teeHandsDefault");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEHANDSDEFAULT_SHADOW),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKHAND),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKHANDSHADOW))
			.Anchor(0.0f)
			.Scale(vec2(30.0f, 30.0f));
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEHANDSDEFAULT_SHADOW),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTHAND),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTHANDSHADOW))
			.Anchor(0.0f)
			.Scale(vec2(30.0f, 30.0f));
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEHANDSDEFAULT_COLOR),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKHAND),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKHAND))
			.Anchor(0.0f)
			.Scale(vec2(30.0f, 30.0f));
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEHANDSDEFAULT_COLOR),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTHAND),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTHAND))
			.Anchor(0.0f)
			.Scale(vec2(30.0f, 30.0f));
	}
	{
		CAsset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEEYESDEFAULT));
		pSkeletonSkin->SetName("teeEyesDefault");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEEYESDEFAULT_NORMAL),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_EYES),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_NORMAL))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEEYESDEFAULT_ANGRY),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_EYES),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_ANGRY))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEEYESDEFAULT_PAIN),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_EYES),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_PAIN))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEEYESDEFAULT_HAPPY),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_EYES),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_HAPPY))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_TEEEYESDEFAULT_FEAR),
			CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_EYES),
			CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_FEAR))
			.Scale(vec2(38.4f, 38.4f))
			.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
	}
	{
		CAsset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_HAMMER));
		pSkeletonSkin->SetName("hammer");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_HAMMER);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_HAMMER),
			CAsset_Skeleton::CBonePath::Local(0),
			CAsset_Skeleton::CBonePath::Parent(tu::TEELAYER_ATTACH))
			.Translation(vec2(-44.0f, 0.0f))
			.Scale(vec2(76.8f, 76.8f));
	}
	{
		CAsset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_GUN));
		pSkeletonSkin->SetName("gun");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_GUN);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_GUN),
			CAsset_Skeleton::CBonePath::Local(0),
			CAsset_Skeleton::CBonePath::Parent(tu::TEELAYER_ATTACH))
			.Translation(vec2(-21.0f, 0.0f))
			.Scale(vec2(57.24f, 57.24f));
	}
	{
		CAsset_SkeletonSkin* pSkeletonSkin = m_SkeletonSkinsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_SHOTGUN));
		pSkeletonSkin->SetName("shotgun");
		pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_SHOTGUN);
		pSkeletonSkin->AddSprite(
			CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_SHOTGUN),
			CAsset_Skeleton::CBonePath::Local(0),
			CAsset_Skeleton::CBonePath::Parent(tu::TEELAYER_ATTACH))
			.Translation(vec2(-21.0f, 0.0f))
			.Scale(vec2(93.13f, 93.13f));
	}
	dbg_msg("assetsmanager", "Skeleton Skins initialised");

	//Skeleton Animations
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEIDLE));
		pSkeletonAnimation->SetName("teeIdle");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT),  0).Translation(vec2(-7.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT), 0).Translation(vec2( 7.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKARM), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_HOOK);

		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);

		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_ANGRY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_PAIN), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_HAPPY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_FEAR), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}

	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEAIR));
		pSkeletonAnimation->SetName("teeAir");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT),  0).Translation(vec2(-3.0f,  0.0f)).Angle(-0.1f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT), 0).Translation(vec2( 3.0f,  0.0f)).Angle(-0.1f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKARM), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_HOOK);

		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);

		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_ANGRY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_PAIN), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_HAPPY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_FEAR), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}

	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEAIR2));
		pSkeletonAnimation->SetName("teeAir2");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT),  0).Translation(vec2(-3.0f,  0.0f)).Angle(-0.1f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT), 0).Translation(vec2( 3.0f,  0.0f)).Angle(-0.1f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKARM), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_HOOK);

		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKFOOT), 0).Color(vec4(0.5f, 0.5f, 0.5f, 1.0f));
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTFOOT), 0).Color(vec4(0.5f, 0.5f, 0.5f, 1.0f));

		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_ANGRY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_PAIN), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_HAPPY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_FEAR), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}

	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEWALK));
		pSkeletonAnimation->SetName("teeWalk");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKARM), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_HOOK);

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY), 0 ).Translation(vec2(0.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY), 6 ).Translation(vec2(0.0f, -1.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY), 12).Translation(vec2(0.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY), 18).Translation(vec2(0.0f,  0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY), 24).Translation(vec2(0.0f, -1.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY), 30).Translation(vec2(0.0f,  0.0f));
		pSkeletonAnimation->SetCycle(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY), CAsset_SkeletonAnimation::CYCLETYPE_LOOP);

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT), 0 ).Translation(vec2(  8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT), 6 ).Translation(vec2( -8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT), 12).Translation(vec2(-10.0f, -4.0f)).Angle(0.2f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT), 18).Translation(vec2( -8.0f, -8.0f)).Angle(0.3f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT), 24).Translation(vec2(  4.0f, -4.0f)).Angle(-0.2f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT), 30).Translation(vec2(  8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->SetCycle(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT), CAsset_SkeletonAnimation::CYCLETYPE_LOOP);

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT), 0 ).Translation(vec2(-10.0f, -4.0f)).Angle(0.2f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT), 6 ).Translation(vec2( -8.0f, -8.0f)).Angle(0.3f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT), 12).Translation(vec2(  4.0f, -4.0f)).Angle(-0.2f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT), 18).Translation(vec2(  8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT), 24).Translation(vec2(  8.0f,  0.0f)).Angle(0.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT), 30).Translation(vec2(-10.0f, -4.0f)).Angle(0.2f*pi*2.0f);
		pSkeletonAnimation->SetCycle(CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT), CAsset_SkeletonAnimation::CYCLETYPE_LOOP);

		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);

		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_ANGRY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_PAIN), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_HAPPY), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_FEAR), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_HAMMERATTACK));
		pSkeletonAnimation->SetName("hammerAttack");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_HAMMER);

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(0), 0).Angle(-pi/2-0.10f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(0), 2).Angle(-pi/2+0.25f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(0), 4).Angle(-pi/2+0.30f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(0), 6).Angle(-pi/2+0.25f*pi*2.0f);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(0), 9).Angle(-pi/2-0.10f*pi*2.0f);

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_EYES), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);

		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_HIDDEN);
	}
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_GUNATTACK));
		pSkeletonAnimation->SetName("gunAttack");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_GUN);

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTARM), 0)
			.Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTARM), 2)
			.Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTARM), 4)
			.Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTHAND), 0)
			.Angle(-3*pi/4)
			.Translation(vec2(0.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTHAND), 2)
			.Angle(-3*pi/4)
			.Translation(vec2(-10.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTHAND), 4)
			.Angle(-3*pi/4)
			.Translation(vec2(0.0f, 0.0f));

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(0), 0)
			.Translation(vec2(0.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(0), 2)
			.Translation(vec2(-10.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(0), 4)
			.Translation(vec2(0.0f, 0.0f));

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_EYES), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);

		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_VISIBLE);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_VISIBLE);
	}
	{
		CAsset_SkeletonAnimation* pSkeletonAnimation = m_SkeletonAnimationsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_SHOTGUNATTACK));
		pSkeletonAnimation->SetName("shotgunAttack");
		pSkeletonAnimation->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_SHOTGUN);

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTARM), 0)
			.Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTARM), 2)
			.Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTARM), 4)
			.Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTHAND), 0)
			.Angle(-pi/2)
			.Translation(vec2(0.0f, 6.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTHAND), 2)
			.Angle(-pi/2)
			.Translation(vec2(-10.0f, 6.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_FRONTHAND), 4)
			.Angle(-pi/2)
			.Translation(vec2(0.0f, 6.0f));

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(0), 0)
			.Translation(vec2(0.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(0), 2)
			.Translation(vec2(-10.0f, 0.0f));
		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Local(0), 4)
			.Translation(vec2(0.0f, 0.0f));

		pSkeletonAnimation->AddBoneKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEEBONE_EYES), 0).Alignment(CAsset_SkeletonAnimation::BONEALIGN_AIM);

		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEELAYER_FRONTHANDSHADOW), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_VISIBLE);
		pSkeletonAnimation->AddLayerKeyFrame(CAsset_Skeleton::CBonePath::Parent(tu::TEELAYER_FRONTHAND), 0).State(CAsset_SkeletonAnimation::LAYERSTATE_VISIBLE);
	}
	dbg_msg("assetsmanager", "Skeleton Animations initialised");

	//Character
	{
		CAsset_Character* pCharacter = m_CharactersCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE));
		pCharacter->SetName("tee");

		pCharacter->AddPart().Name("body").Default(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEBODYDEFAULT));
		pCharacter->AddPart().Name("marking");
		pCharacter->AddPart().Name("decoration");
		pCharacter->AddPart().Name("hands").Default(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEHANDSDEFAULT));
		pCharacter->AddPart().Name("feet").Default(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEFEETDEFAULT));
		pCharacter->AddPart().Name("eyes").Default(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEEYESDEFAULT));

		pCharacter->m_IdlePath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEIDLE);
		pCharacter->m_WalkPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEWALK);
		pCharacter->m_ControlledJumpPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEAIR);
		pCharacter->m_UncontrolledJumpPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_TEEAIR2);
	}
	dbg_msg("assetsmanager", "Characters initialised");

	//CharacterPart
	{
		CAsset_CharacterPart* pCharacterPart = m_CharacterPartsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEBODYDEFAULT));
		pCharacterPart->SetName("teeBodyDefault");
		pCharacterPart->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
		pCharacterPart->m_CharacterPart = CAsset_Character::CSubPath::Part(tu::SKINPART_BODY);
		pCharacterPart->m_SkeletonSkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEBODYDEFAULT);
	}
	{
		CAsset_CharacterPart* pCharacterPart = m_CharacterPartsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEFEETDEFAULT));
		pCharacterPart->SetName("teeFeetDefault");
		pCharacterPart->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
		pCharacterPart->m_CharacterPart = CAsset_Character::CSubPath::Part(tu::SKINPART_FEET);
		pCharacterPart->m_SkeletonSkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEFEETDEFAULT);
	}
	{
		CAsset_CharacterPart* pCharacterPart = m_CharacterPartsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_CHARACTERPART, CHARACTERPART_TEEEYESDEFAULT));
		pCharacterPart->SetName("teeEyesDefault");
		pCharacterPart->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
		pCharacterPart->m_CharacterPart = CAsset_Character::CSubPath::Part(tu::SKINPART_EYES);
		pCharacterPart->m_SkeletonSkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_TEEEYESDEFAULT);
	}
	dbg_msg("assetsmanager", "Character parts initialized");

	//Weapons
	{
		CAsset_Weapon* pWeapon = m_WeaponsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_WEAPON, tu::WEAPON_HAMMER));
		pWeapon->SetName("hammer");
		pWeapon->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
		pWeapon->m_CursorPath = CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_HAMMER_CURSOR);
		pWeapon->m_SkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_HAMMER);
		pWeapon->m_AttackAnimationPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_HAMMERATTACK);
	}
	{
		CAsset_Weapon* pWeapon = m_WeaponsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_WEAPON, tu::WEAPON_GUN));
		pWeapon->SetName("gun");
		pWeapon->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
		pWeapon->m_CursorPath = CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_GUN_CURSOR);
		pWeapon->m_SkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_GUN);
		pWeapon->m_AttackAnimationPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_GUNATTACK);
	}
	dbg_msg("assetsmanager", "Weapons initialised");

	//Weapons
	{
		CAsset_Weapon* pWeapon = m_WeaponsCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_WEAPON, tu::WEAPON_HAMMER));
		pWeapon->SetName("hammer");
		pWeapon->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
		pWeapon->m_CursorPath = CAssetPath::Universe(CAssetPath::TYPE_SPRITE, tu::SPRITE_HAMMER_CURSOR);
		pWeapon->m_SkinPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONSKIN, tu::SKELETONSKIN_HAMMER);
		pWeapon->m_AttackAnimationPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETONANIMATION, tu::SKELETONANIMATION_HAMMERATTACK);
	}

	//GuiRectStyles
	{
		CAsset_GuiRectStyle* pGuiRectStyle = m_GuiRectStylesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_BUTTON_DEFAULT));
		pGuiRectStyle->SetName("buttonDefault");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(0.6f, 0.6f, 0.6f, 1.0f);
		pGuiRectStyle->m_BorderColor = vec4(0.7f, 0.7f, 0.7f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
		pGuiRectStyle->m_CornerRadius = 3.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = m_GuiRectStylesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_BUTTON_DEFAULT_MOUSEOVER));
		pGuiRectStyle->SetName("buttonDefaultMO");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(0.8f, 0.8f, 0.8f, 1.0f);
		pGuiRectStyle->m_BorderColor = vec4(0.7f, 0.7f, 0.7f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
		pGuiRectStyle->m_CornerRadius = 3.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = m_GuiRectStylesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_BUTTON_DEFAULT_HIGHLIGHT));
		pGuiRectStyle->SetName("buttonDefaultHL");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(14.0f/255.0f, 76.0f/255.0f, 137.0f/255.0f, 1.0f);
		pGuiRectStyle->m_BorderColor = vec4(0.7f, 0.7f, 0.7f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
		pGuiRectStyle->m_CornerRadius = 3.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = m_GuiRectStylesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_BUTTON_LINK));
		pGuiRectStyle->SetName("buttonLink");
		pGuiRectStyle->m_Flags = 0;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = m_GuiRectStylesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_BUTTON_LINK_MOUSEOVER));
		pGuiRectStyle->SetName("buttonLinkMO");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(1.0f, 1.0f, 1.0f, 0.2f);
		pGuiRectStyle->m_CornerRadius = 3.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = m_GuiRectStylesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_BUTTON_LINK_HIGHLIGHT));
		pGuiRectStyle->SetName("buttonLinkHL");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(1.0f, 1.0f, 1.0f, 0.4f);
		pGuiRectStyle->m_CornerRadius = 3.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = m_GuiRectStylesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_LAYOUT_DEFAULT));
		pGuiRectStyle->SetName("layoutDefault");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
		pGuiRectStyle->m_CornerRadius = 4.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = m_GuiRectStylesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_ENTRY_DEFAULT));
		pGuiRectStyle->SetName("entryDefault");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER;
		pGuiRectStyle->m_BackgroundColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);
		pGuiRectStyle->m_BorderColor = vec4(0.7f, 0.7f, 0.7f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = m_GuiRectStylesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_ENTRY_DEFAULT_MOUSEOVER));
		pGuiRectStyle->SetName("entryDefaultMouseOver");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER;
		pGuiRectStyle->m_BackgroundColor = vec4(0.8f, 0.8f, 0.8f, 1.0f);
		pGuiRectStyle->m_BorderColor = vec4(0.7f, 0.7f, 0.7f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = m_GuiRectStylesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_TAB_CONTENT));
		pGuiRectStyle->SetName("tabContent");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
		pGuiRectStyle->m_CornerRadius = 5.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_ALL;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = m_GuiRectStylesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_TAB_BUTTON));
		pGuiRectStyle->SetName("tabButton");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_BORDER | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
		pGuiRectStyle->m_BorderColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
		pGuiRectStyle->m_BorderFlags = CAsset_GuiRectStyle::BORDERFLAG_B;
		pGuiRectStyle->m_CornerRadius = 5.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_T;
	}
	{
		CAsset_GuiRectStyle* pGuiRectStyle = m_GuiRectStylesCatalog.NewAsset(CAssetPath::Universe(CAssetPath::TYPE_GUIRECTSTYLE, tu::GUIRECTSTYLE_TAB_BUTTON_HIGHLIGHT));
		pGuiRectStyle->SetName("tabButtonHL");
		pGuiRectStyle->m_Flags = CAsset_GuiRectStyle::FLAG_BACKGROUND | CAsset_GuiRectStyle::FLAG_ROUNDCORNER;
		pGuiRectStyle->m_BackgroundColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
		pGuiRectStyle->m_CornerRadius = 5.0f;
		pGuiRectStyle->m_CornerFlags = CAsset_GuiRectStyle::CORNERFLAG_T;
	}
	dbg_msg("assetsmanager", "GuiRectStyles initialised");
}

int CAssetsManager::LoadSkinAssets_BodyScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CAssetsManager *pSelf = (CAssetsManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;

	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/body/%s", pFilename);

	CAssetPath ImagePath = pSelf->AddImage(IStorage::TYPE_ALL, aBuf, CAssetPath::SRC_SKIN);
	CAsset_Image* pImage = pSelf->GetAsset<CAsset_Image>(ImagePath);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "body/%s.png", aName);
		pImage->SetName(aBuf);

		pImage->m_GridWidth = 2;
		pImage->m_GridHeight = 2;

		CAssetPath ShadowPath;
		CAssetPath ColorPath;
		CAssetPath ShadingPath;
		CAssetPath OutinePath;
		CAssetPath SkeletonSkinPath;
		{
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ShadowPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "body/%s/shadow", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 1, 1);
		}
		{
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ColorPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "body/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ShadingPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "body/%s/shading", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 1, 1, 1);
		}
		{
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&OutinePath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "body/%s/outline", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 1, 1, 1);
		}
		{
			CAsset_SkeletonSkin* pSkeletonSkin = pSelf->m_SkeletonSkinsCatalog.NewAsset(&SkeletonSkinPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "body/%s", aName);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BODYSHADOW))
				.Anchor(0.0f);
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BODY))
				.Anchor(0.0f);
			pSkeletonSkin->AddSprite(
				ShadingPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_OUTLINE))
				.Anchor(0.0f);
			pSkeletonSkin->AddSprite(
				OutinePath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_OUTLINE))
				.Anchor(0.0f);
		}
		{
			CAssetPath CharacterPartPath;
			CAsset_CharacterPart* pCharacterPart = pSelf->m_CharacterPartsCatalog.NewAsset(&CharacterPartPath, CAssetPath::SRC_SKIN);
			pCharacterPart->SetName(aName);
			pCharacterPart->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
			pCharacterPart->m_CharacterPart = CAsset_Character::CSubPath::Part(tu::SKINPART_BODY);
			pCharacterPart->m_SkeletonSkinPath = SkeletonSkinPath;
		}
	}

	return 0;
}

int CAssetsManager::LoadSkinAssets_FootScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CAssetsManager *pSelf = (CAssetsManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;

	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/feet/%s", pFilename);

	CAssetPath ImagePath = pSelf->AddImage(IStorage::TYPE_ALL, aBuf, CAssetPath::SRC_SKIN);
	CAsset_Image* pImage = pSelf->GetAsset<CAsset_Image>(ImagePath);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "feet/%s.png", aName);
		pImage->SetName(aBuf);

		pImage->m_GridWidth = 2;
		pImage->m_GridHeight = 1;

		CAssetPath ShadowPath;
		CAssetPath ColorPath;
		CAssetPath SkeletonSkinPath;
		{
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ShadowPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "feet/%s/shadow", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ColorPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "feet/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 1, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "feet/%s", aName);
			CAsset_SkeletonSkin* pSkeletonSkin = pSelf->m_SkeletonSkinsCatalog.NewAsset(&SkeletonSkinPath, CAssetPath::SRC_SKIN);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKFOOTSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(30.476f, 30.476f));
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTFOOTSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(30.476f, 30.476f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKFOOT),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKFOOT))
				.Anchor(0.0f)
				.Scale(vec2(30.476f, 30.476f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTFOOT),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTFOOT))
				.Anchor(0.0f)
				.Scale(vec2(30.476f, 30.476f));
		}
		{
			CAssetPath CharacterPartPath;
			CAsset_CharacterPart* pCharacterPart = pSelf->m_CharacterPartsCatalog.NewAsset(&CharacterPartPath, CAssetPath::SRC_SKIN);
			pCharacterPart->SetName(aName);
			pCharacterPart->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
			pCharacterPart->m_CharacterPart = CAsset_Character::CSubPath::Part(tu::SKINPART_FEET);
			pCharacterPart->m_SkeletonSkinPath = SkeletonSkinPath;
		}
	}

	return 0;
}

int CAssetsManager::LoadSkinAssets_HandsScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CAssetsManager *pSelf = (CAssetsManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;

	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/hands/%s", pFilename);

	CAssetPath ImagePath = pSelf->AddImage(IStorage::TYPE_ALL, aBuf, CAssetPath::SRC_SKIN);
	CAsset_Image* pImage = pSelf->GetAsset<CAsset_Image>(ImagePath);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "hands/%s.png", aName);
		pImage->SetName(aBuf);

		pImage->m_GridWidth = 2;
		pImage->m_GridHeight = 1;

		CAssetPath ShadowPath;
		CAssetPath ColorPath;
		CAssetPath SkeletonSkinPath;
		{
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ShadowPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "hands/%s/shadow", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ColorPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "hands/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 1, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "hands/%s", aName);
			CAsset_SkeletonSkin* pSkeletonSkin = pSelf->m_SkeletonSkinsCatalog.NewAsset(&SkeletonSkinPath, CAssetPath::SRC_SKIN);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKHAND),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKHANDSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(30.0f, 30.0f));
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTHAND),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTHANDSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(30.0f, 30.0f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BACKHAND),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BACKHAND))
				.Anchor(0.0f)
				.Scale(vec2(30.0f, 30.0f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_FRONTHAND),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_FRONTHAND))
				.Anchor(0.0f)
				.Scale(vec2(30.0f, 30.0f));
		}
		{
			CAssetPath CharacterPartPath;
			CAsset_CharacterPart* pCharacterPart = pSelf->m_CharacterPartsCatalog.NewAsset(&CharacterPartPath, CAssetPath::SRC_SKIN);
			pCharacterPart->SetName(aName);
			pCharacterPart->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
			pCharacterPart->m_CharacterPart = CAsset_Character::CSubPath::Part(tu::SKINPART_HANDS);
			pCharacterPart->m_SkeletonSkinPath = SkeletonSkinPath;
		}
	}

	return 0;
}

int CAssetsManager::LoadSkinAssets_MarkingScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CAssetsManager *pSelf = (CAssetsManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;

	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/marking/%s", pFilename);

	CAssetPath ImagePath = pSelf->AddImage(IStorage::TYPE_ALL, aBuf, CAssetPath::SRC_SKIN);
	CAsset_Image* pImage = pSelf->GetAsset<CAsset_Image>(ImagePath);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "marking/%s.png", aName);
		pImage->SetName(aBuf);

		pImage->m_GridWidth = 1;
		pImage->m_GridHeight = 1;

		CAssetPath ColorPath;
		CAssetPath SkeletonSkinPath;
		{
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ColorPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "marking/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "marking/%s", aName);
			CAsset_SkeletonSkin* pSkeletonSkin = pSelf->m_SkeletonSkinsCatalog.NewAsset(&SkeletonSkinPath, CAssetPath::SRC_SKIN);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_MARKING))
				.Anchor(0.0f)
				.Scale(vec2(64.0f, 64.0f));
		}
		{
			CAssetPath CharacterPartPath;
			CAsset_CharacterPart* pCharacterPart = pSelf->m_CharacterPartsCatalog.NewAsset(&CharacterPartPath, CAssetPath::SRC_SKIN);
			pCharacterPart->SetName(aName);
			pCharacterPart->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
			pCharacterPart->m_CharacterPart = CAsset_Character::CSubPath::Part(tu::SKINPART_MARKING);
			pCharacterPart->m_SkeletonSkinPath = SkeletonSkinPath;
		}
	}

	return 0;
}

int CAssetsManager::LoadSkinAssets_DecorationScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CAssetsManager *pSelf = (CAssetsManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;

	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/decoration/%s", pFilename);

	CAssetPath ImagePath = pSelf->AddImage(IStorage::TYPE_ALL, aBuf, CAssetPath::SRC_SKIN);
	CAsset_Image* pImage = pSelf->GetAsset<CAsset_Image>(ImagePath);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "decoration/%s.png", aName);
		pImage->SetName(aBuf);

		pImage->m_GridWidth = 2;
		pImage->m_GridHeight = 1;

		CAssetPath ShadowPath;
		CAssetPath ColorPath;
		CAssetPath SkeletonSkinPath;
		{
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ShadowPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "decoration/%s/shadow", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 1, 0, 1, 1);
		}
		{
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&ColorPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "decoration/%s/color", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 1, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "decoration/%s", aName);
			CAsset_SkeletonSkin* pSkeletonSkin = pSelf->m_SkeletonSkinsCatalog.NewAsset(&SkeletonSkinPath, CAssetPath::SRC_SKIN);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				ShadowPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_BODYSHADOW))
				.Anchor(0.0f)
				.Scale(vec2(64.0f, 64.0f));
			pSkeletonSkin->AddSprite(
				ColorPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_BODY),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_DECORATION))
				.Anchor(0.0f)
				.Scale(vec2(64.0f, 64.0f));
		}
		{
			CAssetPath CharacterPartPath;
			CAsset_CharacterPart* pCharacterPart = pSelf->m_CharacterPartsCatalog.NewAsset(&CharacterPartPath, CAssetPath::SRC_SKIN);
			pCharacterPart->SetName(aName);
			pCharacterPart->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
			pCharacterPart->m_CharacterPart = CAsset_Character::CSubPath::Part(tu::SKINPART_DECORATION);
			pCharacterPart->m_SkeletonSkinPath = SkeletonSkinPath;
		}
	}

	return 0;
}

int CAssetsManager::LoadSkinAssets_EyeScan(const char *pFilename, int IsDir, int DirType, void *pUser)
{
	CAssetsManager *pSelf = (CAssetsManager *)pUser;
	int l = str_length(pFilename);
	if(l < 4 || IsDir || str_comp(pFilename+l-4, ".png") != 0)
		return 0;

	char aName[128];
	str_copy(aName, pFilename, sizeof(aName));
	aName[str_length(aName)-4] = 0;

	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "skins/eyes/%s", pFilename);

	CAssetPath ImagePath = pSelf->AddImage(IStorage::TYPE_ALL, aBuf, CAssetPath::SRC_SKIN);
	CAsset_Image* pImage = pSelf->GetAsset<CAsset_Image>(ImagePath);
	if(pImage)
	{
		str_format(aBuf, sizeof(aBuf), "eyes/%s.png", aName);
		pImage->SetName(aBuf);

		pImage->m_GridWidth = 4;
		pImage->m_GridHeight = 4;

		CAssetPath NormalPath;
		CAssetPath AngryPath;
		CAssetPath PainPath;
		CAssetPath HappyPath;
		CAssetPath FearPath;
		CAssetPath SkeletonSkinPath;
		{
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&NormalPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/normal", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 0, 2, 1);
		}
		{
			CAssetPath SpritePath;
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&AngryPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/angry", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 2, 0, 2, 1);
		}
		{
			CAssetPath SpritePath;
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&PainPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/pain", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 1, 2, 1);
		}
		{
			CAssetPath SpritePath;
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&HappyPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/happy", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 2, 1, 2, 1);
		}
		{
			CAssetPath SpritePath;
			CAsset_Sprite* pSprite = pSelf->m_SpritesCatalog.NewAsset(&FearPath, CAssetPath::SRC_SKIN);
			str_format(aBuf, sizeof(aBuf), "eyes/%s/fear", aName);
			pSprite->SetName(aBuf);
			pSprite->Init(ImagePath, 0, 2, 2, 1);
		}
		{
			str_format(aBuf, sizeof(aBuf), "eyes/%s", aName);
			CAsset_SkeletonSkin* pSkeletonSkin = pSelf->m_SkeletonSkinsCatalog.NewAsset(&SkeletonSkinPath, CAssetPath::SRC_SKIN);
			pSkeletonSkin->SetName(aBuf);
			pSkeletonSkin->m_SkeletonPath = CAssetPath::Universe(CAssetPath::TYPE_SKELETON, tu::SKELETON_TEE);
			pSkeletonSkin->AddSprite(
				NormalPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_EYES),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_NORMAL))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
			pSkeletonSkin->AddSprite(
				AngryPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_EYES),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_ANGRY))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
			pSkeletonSkin->AddSprite(
				PainPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_EYES),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_PAIN))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
			pSkeletonSkin->AddSprite(
				HappyPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_EYES),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_HAPPY))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
			pSkeletonSkin->AddSprite(
				FearPath,
				CAsset_Skeleton::CBonePath::Local(tu::TEEBONE_EYES),
				CAsset_Skeleton::CBonePath::Local(tu::TEELAYER_EYES_FEAR))
				.Scale(vec2(38.4, 38.4))
				.Alignment(CAsset_SkeletonSkin::ALIGNEMENT_WORLD);
		}
		{
			CAssetPath CharacterPartPath;
			CAsset_CharacterPart* pCharacterPart = pSelf->m_CharacterPartsCatalog.NewAsset(&CharacterPartPath, CAssetPath::SRC_SKIN);
			pCharacterPart->SetName(aName);
			pCharacterPart->m_CharacterPath = CAssetPath::Universe(CAssetPath::TYPE_CHARACTER, CHARACTER_TEE);
			pCharacterPart->m_CharacterPart = CAsset_Character::CSubPath::Part(tu::SKINPART_EYES);
			pCharacterPart->m_SkeletonSkinPath = SkeletonSkinPath;
		}
	}

	return 0;
}

void CAssetsManager::LoadSkinAssets(IStorage* pStorage)
{
	dbg_msg("assetsmanager", "load skin assets");
	Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/body", CAssetsManager::LoadSkinAssets_BodyScan, this);
	dbg_msg("assetsmanager", "body assets loaded");
	Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/feet", CAssetsManager::LoadSkinAssets_FootScan, this);
	dbg_msg("assetsmanager", "feet assets loaded");
	Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/eyes", CAssetsManager::LoadSkinAssets_EyeScan, this);
	dbg_msg("assetsmanager", "eyes assets loaded");
	Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/hands", CAssetsManager::LoadSkinAssets_HandsScan, this);
	dbg_msg("assetsmanager", "hands assets loaded");
	Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/marking", CAssetsManager::LoadSkinAssets_MarkingScan, this);
	dbg_msg("assetsmanager", "marking assets loaded");
	Storage()->ListDirectory(IStorage::TYPE_ALL, "skins/decoration", CAssetsManager::LoadSkinAssets_DecorationScan, this);
	dbg_msg("assetsmanager", "decoration assets loaded");
}

CAssetPath CAssetsManager::FindSkinPart(CAssetPath CharacterPath, CAsset_Character::CSubPath CharacterPart, const char* pName)
{
	char aAssetName[256];
	str_format(aAssetName, sizeof(aAssetName), "%s", pName);

	for(int i=0; i<m_CharacterPartsCatalog.m_Assets[CAssetPath::SRC_SKIN].size(); i++)
	{
		const CAsset_CharacterPart* pCharacterPart = &m_CharacterPartsCatalog.m_Assets[CAssetPath::SRC_SKIN][i];
		if(
			str_comp(pCharacterPart->m_aName, aAssetName) == 0 &&
			pCharacterPart->m_CharacterPath == CharacterPath &&
			pCharacterPart->m_CharacterPart == CharacterPart
		)
		{
			return CAssetPath::Skin(CAssetPath::TYPE_CHARACTERPART, i);
		}
	}

	return CAssetPath::Null();
}

CAssetPath CAssetsManager::AddImage(int StorageType, const char* pFilename, int Source)
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
		return CAssetPath::Null();
	}

	int Error = png_open_file(&Png, aCompleteFilename); // ignore_convention
	if(Error != PNG_NO_ERROR)
	{
		dbg_msg("mod", "failed to open file. filename='%s'", aCompleteFilename);
		if(Error != PNG_FILE_ERROR)
		{
			png_close_file(&Png); // ignore_convention
		}
		return CAssetPath::Null();
	}

	if(Png.depth != 8 || (Png.color_type != PNG_TRUECOLOR && Png.color_type != PNG_TRUECOLOR_ALPHA) || Png.width > (2<<12) || Png.height > (2<<12)) // ignore_convention
	{
		dbg_msg("mod", "invalid format. filename='%s'", aCompleteFilename);
		png_close_file(&Png); // ignore_convention
		return CAssetPath::Null();
	}

	pBuffer = (unsigned char *) mem_alloc(Png.width * Png.height * Png.bpp, 1); // ignore_convention
	png_get_data(&Png, pBuffer); // ignore_convention
	png_close_file(&Png); // ignore_convention

	int Format;
	if(Png.color_type == PNG_TRUECOLOR)
		Format = CImageInfo::FORMAT_RGB;
	else if(Png.color_type == PNG_TRUECOLOR_ALPHA)
		Format = CImageInfo::FORMAT_RGBA;

	CAssetPath Path;
	CAsset_Image* pImage = m_ImagesCatalog.NewAsset(&Path, Source);
	pImage->SetAssetsManager(this);
	pImage->SetName(pFilename);
	pImage->SetData(Png.width, Png.height, 1, 1, Format, pBuffer);
	mem_free(pBuffer);

	return Path;
}

void CAssetsManager::UpdateAssets()
{
	//TODO: Do it only for images
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) CatalogName.Update();
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE
}

int CAssetsManager::SaveInAssetsFile(const char *pFileName, int Source)
{
	CDataFileWriter df;
	if(!df.Open(Storage(), pFileName))
	{
		dbg_msg("TUGraphics", "can't create the assets file %s", pFileName);
		return 0;
	}

	{
		CStorageType Item;
		Item.m_Version = 0;
		Item.m_Source = Source;

		df.AddItem(CAsset_Image::TypeId, 0, sizeof(CStorageType), &Item);
	}

	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) CatalogName.SaveInAssetsFile(&df, Source);
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE

	df.Finish(DATAFILE_TYPE_ASSET);

	return 1;
}

int CAssetsManager::OnAssetsFileLoaded_Asset(tu::IAssetsFile* pAssetsFile)
{
	int Start, Num;
	pAssetsFile->GetType(0, &Start, &Num);

	int Source = CAssetPath::SRC_WORLD;
	if(Num > 0)
	{
		CStorageType* pItem = (CStorageType*) pAssetsFile->GetItem(Start, 0, 0);
		Source = pItem->m_Source % CAssetPath::NUM_SOURCES;
	}

	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) CatalogName.LoadFromAssetsFile(this, pAssetsFile, Source);
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE

	return 1;
}

int CAssetsManager::OnAssetsFileLoaded_Map(tu::IAssetsFile* pAssetsFile)
{
	CMapItemVersion *pItem = (CMapItemVersion *)pAssetsFile->FindItem(MAPITEMTYPE_VERSION, 0);
	if(!pItem)
		return 0;

	if(pItem->m_Version != CMapItemVersion::CURRENT_VERSION)
		return 0;

	char aBuf[128];

	CAssetPath* pImagePath;
	//Load images
	{
		int Start, Num;
		pAssetsFile->GetType(MAPITEMTYPE_IMAGE, &Start, &Num);

		pImagePath = new CAssetPath[Num];

		for(int i = 0; i < Num; i++)
		{
			CMapItemImage *pItem = (CMapItemImage *)pAssetsFile->GetItem(Start+i, 0, 0);

			if(!pItem->m_External)
			{
				CAsset_Image* pImage = GetAssetCatalog<CAsset_Image>()->NewAsset(
					pImagePath+i,
					CAssetPath::SRC_LAND
				);
				pImage->SetAssetsManager(this);

				//Image name
				{
					char *pName = (char *)pAssetsFile->GetData(pItem->m_ImageName);
					pImage->SetName(pName);
					pAssetsFile->UnloadData(pItem->m_ImageName);
				}

				//Image data
				{
					unsigned char* pData = (unsigned char*) pAssetsFile->GetData(pItem->m_ImageData);
					int Format = CAsset_Image::FORMAT_RGBA;
					if(pItem->m_Version > 1)
					{
						switch(pItem->m_Format)
						{
							case CImageInfo::FORMAT_RGB:
								Format = CAsset_Image::FORMAT_RGB;
								break;
							case CImageInfo::FORMAT_AUTO:
							case CImageInfo::FORMAT_RGBA:
								Format = CAsset_Image::FORMAT_RGBA;
								break;
							case CImageInfo::FORMAT_ALPHA:
								Format = CAsset_Image::FORMAT_ALPHA;
								break;
						}
					}

					pImage->SetData(pItem->m_Width, pItem->m_Height, pItem->m_Width/64, pItem->m_Height/64, Format, pData);
					pAssetsFile->UnloadData(pItem->m_ImageData);
					pImage->m_Texture = Graphics()->LoadTextureRaw(pImage->m_Width, pImage->m_Height, pImage->m_Format, pImage->m_GridWidth, pImage->m_GridHeight, pImage->m_pData, CImageInfo::FORMAT_AUTO, IGraphics::TEXLOAD_MULTI_DIMENSION);

				}
			}
			else
			{
				//Check if the image exists and load it if needed
			}
		}
	}

	//Load groups
	{
		int LayersStart, LayersNum;
		pAssetsFile->GetType(MAPITEMTYPE_LAYER, &LayersStart, &LayersNum);

		int Start, Num;
		pAssetsFile->GetType(MAPITEMTYPE_GROUP, &Start, &Num);
		for(int g = 0; g < Num; g++)
		{
			CMapItemGroup *pGItem = (CMapItemGroup *)pAssetsFile->GetItem(Start+g, 0, 0);

			if(pGItem->m_Version < 1 || pGItem->m_Version > CMapItemGroup::CURRENT_VERSION)
				continue;

			CAssetPath MapGroupPath;
			CAsset_MapGroup* pMapGroup = GetAssetCatalog<CAsset_MapGroup>()->NewAsset(
				&MapGroupPath,
				CAssetPath::SRC_LAND
			);

			pMapGroup->m_Position = vec2(pGItem->m_OffsetX, pGItem->m_OffsetY);
			pMapGroup->m_HardParallax = vec2(pGItem->m_ParallaxX/100.0f, pGItem->m_ParallaxY/100.0f);

			//~ if(pGItem->m_Version >= 2)
			//~ {
				//~ pGroup->m_UseClipping = pGItem->m_UseClipping;
				//~ pGroup->m_ClipX = pGItem->m_ClipX;
				//~ pGroup->m_ClipY = pGItem->m_ClipY;
				//~ pGroup->m_ClipW = pGItem->m_ClipW;
				//~ pGroup->m_ClipH = pGItem->m_ClipH;
			//~ }

			// load group name
			aBuf[0] = 0;

			if(pGItem->m_Version >= 3)
				IntsToStr(pGItem->m_aName, sizeof(pGItem->m_aName)/sizeof(int), aBuf);

			if(!aBuf[0])
				str_format(aBuf, sizeof(aBuf), "group%d", MapGroupPath.GetId());

			pMapGroup->SetName(aBuf);

			for(int l = 0; l < pGItem->m_NumLayers; l++)
			{
				CMapItemLayer *pLayerItem = (CMapItemLayer *)pAssetsFile->GetItem(LayersStart+pGItem->m_StartLayer+l, 0, 0);
				if(!pLayerItem)
					continue;

				if(pLayerItem->m_Type == LAYERTYPE_TILES)
				{
					CMapItemLayerTilemap *pTilemapItem = (CMapItemLayerTilemap *)pLayerItem;

					if(pTilemapItem->m_Flags&TILESLAYERFLAG_GAME)
						continue;

					CAssetPath MapLayerPath;
					CAsset_MapLayerTiles* pMapLayer = GetAssetCatalog<CAsset_MapLayerTiles>()->NewAsset(
						&MapLayerPath,
						CAssetPath::SRC_LAND
					);

					pMapGroup->m_Layers.add(MapLayerPath);

					//Name
					aBuf[0] = 0;

					if(pTilemapItem->m_Version >= 3)
						IntsToStr(pTilemapItem->m_aName, sizeof(pTilemapItem->m_aName)/sizeof(int), aBuf);

					if(!aBuf[0])
						str_format(aBuf, sizeof(aBuf), "tilelayer%d", MapLayerPath.GetId());

					pMapLayer->SetName(aBuf);

					//Tiles
					int Width = pTilemapItem->m_Width;
					pMapLayer->SetSize(Width, pTilemapItem->m_Height);
					CTile* pTiles = (CTile*) pAssetsFile->GetData(pTilemapItem->m_Data);

					for(int j=0; j<pMapLayer->m_Height; j++)
					{
						for(int i=0; i<pMapLayer->m_Width; i++)
						{
							int Skip = pTiles[j*Width+i].m_Skip;
							pMapLayer->GetTile(i, j)->m_Index = pTiles[j*Width+i].m_Index;
							pMapLayer->GetTile(i, j)->m_Flags = pTiles[j*Width+i].m_Flags;
							pMapLayer->GetTile(i, j)->m_Reserved = pTiles[j*Width+i].m_Reserved;
							pMapLayer->GetTile(i, j)->m_Skip = Skip;
							i += Skip;
						}
					}

					//Image
					if(pTilemapItem->m_Image >= 0)
						pMapLayer->m_ImagePath = pImagePath[pTilemapItem->m_Image];

					//Color
					pMapLayer->m_Color.r = static_cast<float>(pTilemapItem->m_Color.r)/255.0f;
					pMapLayer->m_Color.g = static_cast<float>(pTilemapItem->m_Color.g)/255.0f;
					pMapLayer->m_Color.b = static_cast<float>(pTilemapItem->m_Color.b)/255.0f;
					pMapLayer->m_Color.a = static_cast<float>(pTilemapItem->m_Color.a)/255.0f;
				}
				else if(pLayerItem->m_Type == LAYERTYPE_QUADS)
				{
					CMapItemLayerQuads *pQuadsItem = (CMapItemLayerQuads *)pLayerItem;

					CAssetPath MapLayerPath;
					CAsset_MapLayerQuads* pMapLayer = GetAssetCatalog<CAsset_MapLayerQuads>()->NewAsset(
						&MapLayerPath,
						CAssetPath::SRC_LAND
					);
					pMapGroup->m_Layers.add(MapLayerPath);

					//Name
					aBuf[0] = 0;

					if(pQuadsItem->m_Version >= 2)
						IntsToStr(pQuadsItem->m_aName, sizeof(pQuadsItem->m_aName)/sizeof(int), aBuf);

					if(!aBuf[0])
						str_format(aBuf, sizeof(aBuf), "quadlayer%d", MapLayerPath.GetId());

					pMapLayer->SetName(aBuf);

					//Quads
					CQuad *pQuads = (CQuad *)pAssetsFile->GetDataSwapped(pQuadsItem->m_Data);
					for(int i=0; i<pQuadsItem->m_NumQuads; i++)
					{
						CAsset_MapLayerQuads::CQuad Quad;
						Quad.m_Pivot.x = fx2f(pQuads[i].m_aPoints[4].x);
						Quad.m_Pivot.y = fx2f(pQuads[i].m_aPoints[4].y);
						for(int p=0; p<4; p++)
						{
							Quad.m_Vertices[p].x = fx2f(pQuads[i].m_aPoints[p].x) - Quad.m_Pivot.x;
							Quad.m_Vertices[p].y = fx2f(pQuads[i].m_aPoints[p].y) - Quad.m_Pivot.y;
							Quad.m_UVs[p].x = fx2f(pQuads[i].m_aTexcoords[p].x);
							Quad.m_UVs[p].y = fx2f(pQuads[i].m_aTexcoords[p].y);

							Quad.m_Colors[p].r = static_cast<float>(pQuads[i].m_aColors[p].r)/255.0f;
							Quad.m_Colors[p].g = static_cast<float>(pQuads[i].m_aColors[p].g)/255.0f;
							Quad.m_Colors[p].b = static_cast<float>(pQuads[i].m_aColors[p].b)/255.0f;
							Quad.m_Colors[p].a = static_cast<float>(pQuads[i].m_aColors[p].a)/255.0f;
						}
						pMapLayer->m_Quads.add(Quad);
					}

					//Image
					if(pQuadsItem->m_Image >= 0)
						pMapLayer->m_ImagePath = pImagePath[pQuadsItem->m_Image];
				}
			}
		}
	}

	delete[] pImagePath;

	return 1;
}

int CAssetsManager::OnAssetsFileLoaded(tu::IAssetsFile* pAssetsFile)
{
	if(pAssetsFile->GetDataFileType() == DATAFILE_TYPE_ASSET)
		return OnAssetsFileLoaded_Asset(pAssetsFile);
	else
		return OnAssetsFileLoaded_Map(pAssetsFile);
}

int CAssetsManager::OnAssetsFileUnloaded()
{
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) CatalogName.Unload();
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE
}

void CAssetsManager::DeleteAsset(const CAssetPath& Path)
{
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) CatalogName.DeleteAsset(Path);
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE

	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) CatalogName.OnAssetDeleted(Path);
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE
}

int CAssetsManager::AddSubItem(CAssetPath AssetPath, int SubItemType)
{
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) case ClassName::TypeId:\
	{\
		ClassName* pAsset = GetAsset<ClassName>(AssetPath);\
		if(pAsset)\
			return pAsset->AddSubItem(SubItemType);\
		else\
			return -1;\
	}

	switch(AssetPath.GetType())
	{
		#include <tu/client/assetsmacro.h>
	}

	#undef TU_MACRO_ASSETTYPE

	return -1;
}

void CAssetsManager::DeleteSubItem(CAssetPath AssetPath, int SubPath)
{
	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) case ClassName::TypeId:\
	{\
		ClassName* pAsset = GetAsset<ClassName>(AssetPath);\
		if(pAsset)\
			pAsset->DeleteSubItem(SubPath);\
		break;\
	}

	switch(AssetPath.GetType())
	{
		#include <tu/client/assetsmacro.h>
	}
	#undef TU_MACRO_ASSETTYPE

	#define TU_MACRO_ASSETTYPE(ClassName, CatalogName) CatalogName.OnSubItemDeleted(AssetPath, SubPath);
	#include <tu/client/assetsmacro.h>
	#undef TU_MACRO_ASSETTYPE
}

}
