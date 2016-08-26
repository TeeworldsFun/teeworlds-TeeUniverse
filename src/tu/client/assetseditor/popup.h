#ifndef TU_ASSETSEDITOR_POPUP_H
#define TU_ASSETSEDITOR_POPUP_H

#include <engine/storage.h>
#include <tu/client/gui/popup.h>
#include <tu/client/gui/button.h>
#include <tu/client/gui/tabs.h>
#include <tu/client/gui/label.h>
#include <tu/client/gui/float-edit.h>
#include <tu/client/gui/text-edit.h>
#include <base/color.h>

#include "assetseditor.h"

namespace tu
{

namespace assetseditor
{

/* POPUP ADD IMAGE ****************************************************/

class CPopup_AddImage : public gui::CPopup
{
protected:
	CAssetsEditor* m_pAssetsEditor;
	gui::CVListLayout* m_Layout;
	char m_aDirectory[256];
	bool m_NeedRefresh;
	
protected:
	class CItem : public gui::CTextButton
	{
	protected:
		CPopup_AddImage* m_pPopup;
		CAssetsEditor* m_pAssetsEditor;
		char m_aFilename[128];
		int m_StorageType;
		int m_IsDirectory;
		
	protected:
		virtual void MouseClickAction()
		{
			if(m_IsDirectory)
			{
				char* pDirectory = m_pPopup->m_aDirectory;
				pDirectory += str_length(pDirectory);
				*pDirectory = '/';
				pDirectory++;
				str_copy(pDirectory, m_aFilename, (m_pPopup->m_aDirectory+sizeof(m_pPopup->m_aDirectory))-pDirectory);
				
				m_pPopup->m_NeedRefresh = true;
			}
			else
			{
				char aBuf[512];
				str_format(aBuf, sizeof(aBuf), "%s/%s", m_pPopup->m_aDirectory, m_aFilename);
				CAssetPath Path = m_pAssetsEditor->AssetsManager()->AddImage(m_StorageType, aBuf, CAssetPath::SRC_WORLD);
				if(!Path.IsNull())
				{
					m_pAssetsEditor->NewAsset(Path);
				}
				m_pPopup->Close();
			}
		}
		
	public:
		CItem(CPopup_AddImage* pPopup, const char* pFilename, int StorageType, int IsDir) :
			gui::CTextButton(pPopup->m_pAssetsEditor->m_pGuiConfig, pFilename, IsDir ? CAssetsEditor::ICON_MAPGROUP : CAssetsEditor::ICON_IMAGE),
			m_pPopup(pPopup),
			m_pAssetsEditor(pPopup->m_pAssetsEditor)
		{
			m_Centered = false;
			
			str_copy(m_aFilename, pFilename, sizeof(m_aFilename));
			m_StorageType = StorageType;
			m_IsDirectory = IsDir;
		}
	};
	
public:
	CPopup_AddImage(CAssetsEditor* pAssetsEditor, const gui::CRect& CreatorRect, int Alignment) :
		gui::CPopup(pAssetsEditor->m_pGuiConfig, CreatorRect, 300, 500, Alignment),
		m_pAssetsEditor(pAssetsEditor)
	{	
		str_copy(m_aDirectory, ".", sizeof(m_aDirectory));
			
		m_Layout = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
		Add(m_Layout);
		
		m_NeedRefresh = true;
		
		Update();
	}
	
	static int FileListFetchCallback(const char *pName, int IsDir, int StorageType, void *pUser)
	{
		CPopup_AddImage* pAddImageView = static_cast<CPopup_AddImage*>(pUser);
		
		int Length = str_length(pName);
		if(pName[0] == '.' && (pName[1] == 0))
			return 0;
		
		if(!IsDir && (Length < 4 || str_comp(pName+Length-4, ".png")))
			return 0;

		CPopup_AddImage::CItem* pItem = new CPopup_AddImage::CItem(pAddImageView, pName, StorageType, IsDir);
		pAddImageView->m_Layout->Add(pItem);

		return 0;
	}
	
	virtual void Update()
	{
		dbg_msg("TeeUniv", "Directory:%s", m_aDirectory);
		m_Layout->Clear();
		m_pAssetsEditor->Storage()->ListDirectory(IStorage::TYPE_ALL, m_aDirectory, FileListFetchCallback, this);
		gui::CPopup::Update();
		m_NeedRefresh = false;
	}
	
	virtual void Render()
	{
		if(m_NeedRefresh)
			Update();
		
		gui::CPopup::Render();
	}
};

/* POPUP ASSET EDIT ***************************************************/

class CPopup_AssetEdit : public gui::CPopup
{
public:
	CAssetsEditor* m_pAssetsEditor;
	
protected:
	class CItem : public gui::CTextButton
	{
	protected:
		CPopup_AssetEdit* m_pPopup;
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_ParentAssetPath;
		int m_ParentAssetMember;
		int m_ParentAssetSubId;
		CAssetPath m_FieldAssetPath;
		
	protected:
		virtual void MouseClickAction()
		{
			m_pAssetsEditor->AssetsManager()->SetAssetValue<CAssetPath>(m_ParentAssetPath, m_ParentAssetMember, m_ParentAssetSubId, m_FieldAssetPath);
			m_pAssetsEditor->RefreshAssetsEditor();
			m_pAssetsEditor->RefreshAssetsList();
		}
		
	public:
		CItem(CPopup_AssetEdit* pPopup, CAssetPath ParentAssetPath, int ParentAssetMember, int ParentAssetSubId, int FieldAssetType, CAssetPath FieldAssetPath) :
			gui::CTextButton(pPopup->m_pAssetsEditor->m_pGuiConfig, ""),
			m_pPopup(pPopup),
			m_pAssetsEditor(pPopup->m_pAssetsEditor),
			m_ParentAssetPath(ParentAssetPath),
			m_ParentAssetMember(ParentAssetMember),
			m_ParentAssetSubId(ParentAssetSubId),
			m_FieldAssetPath(FieldAssetPath)
		{
			m_Centered = false;
			SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK);
			
			if(m_FieldAssetPath.IsNull())
			{
				SetText("Unset value");
			}
			else
			{
				int IconId = -1;
				switch(FieldAssetPath.GetType())
				{
					case CAssetPath::TYPE_IMAGE:
						IconId = CAssetsEditor::ICON_IMAGE;
						break;
					case CAssetPath::TYPE_SPRITE:
						IconId = CAssetsEditor::ICON_SPRITE;
						break;
					case CAssetPath::TYPE_SKELETON:
						IconId = CAssetsEditor::ICON_SKELETON;
						break;
					case CAssetPath::TYPE_SKELETONSKIN:
						IconId = CAssetsEditor::ICON_SKELETONSKIN;
						break;
					case CAssetPath::TYPE_SKELETONANIMATION:
						IconId = CAssetsEditor::ICON_SKELETONANIMATION;
						break;
					case CAssetPath::TYPE_CHARACTER:
						IconId = CAssetsEditor::ICON_CHARACTER;
						break;
					case CAssetPath::TYPE_CHARACTERPART:
						IconId = CAssetsEditor::ICON_CHARACTERPART;
						break;
				}
				
				char* pName = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(m_FieldAssetPath, CAsset::NAME, -1, 0);
				SetText(pName);
				SetIcon(IconId);
			}
		}
	};

protected:
	gui::CVListLayout* m_Layout;
	
protected:
	CAssetPath m_ParentAssetPath;
	int m_ParentAssetMember;
	int m_ParentAssetSubId;
	int m_FieldAssetType;
	
protected:
	void AddListElement(CAssetPath Path)
	{	
		CItem* pItem = new CItem(
			this,
			m_ParentAssetPath,
			m_ParentAssetMember,
			m_ParentAssetSubId,
			m_FieldAssetType,
			Path
		);
		m_Layout->Add(pItem);
	}
	
public:
	CPopup_AssetEdit(
		CAssetsEditor* pAssetsEditor,
		const gui::CRect& CreatorRect,
		int Alignment,
		CAssetPath ParentAssetPath,
		int ParentAssetMember,
		int ParentAssetSubId,
		int FieldAssetType
	) :
		gui::CPopup(pAssetsEditor->m_pGuiConfig, CreatorRect, 250, 500, Alignment),
		m_pAssetsEditor(pAssetsEditor),
		m_ParentAssetPath(ParentAssetPath),
		m_ParentAssetMember(ParentAssetMember),
		m_ParentAssetSubId(ParentAssetSubId),
		m_FieldAssetType(FieldAssetType)
	{
		m_Layout = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
		Add(m_Layout);
		
		Update();
	}
	
	virtual void Update()
	{
		#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
			m_Layout->Add(new gui::CLabel(m_pConfig, AssetTypeName, gui::TEXTSTYLE_HEADER));\
			break;
		
		switch(m_FieldAssetType)
		{
			#include <tu/client/assetsmacro.h>
		}
		
		#undef TU_MACRO_ASSETTYPE
		
		AddListElement(CAssetPath::Null());
		
		#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
			m_Layout->AddSeparator();\
			m_Layout->Add(new gui::CLabel(m_pConfig, "Universe", gui::TEXTSTYLE_HEADER2));\
			for(int i=0; i<m_pAssetsEditor->AssetsManager()->GetNumAssets<ClassName>(CAssetPath::SRC_UNIVERSE); i++)\
				AddListElement(CAssetPath::Universe(ClassName::TypeId, i));\
			m_Layout->AddSeparator();\
			m_Layout->Add(new gui::CLabel(m_pConfig, "World", gui::TEXTSTYLE_HEADER2));\
			for(int i=0; i<m_pAssetsEditor->AssetsManager()->GetNumAssets<ClassName>(CAssetPath::SRC_WORLD); i++)\
				AddListElement(CAssetPath::World(ClassName::TypeId, i));\
			m_Layout->AddSeparator();\
			m_Layout->Add(new gui::CLabel(m_pConfig, "Land", gui::TEXTSTYLE_HEADER2));\
			for(int i=0; i<m_pAssetsEditor->AssetsManager()->GetNumAssets<ClassName>(CAssetPath::SRC_LAND); i++)\
				AddListElement(CAssetPath::World(ClassName::TypeId, i));\
			break;
		
		switch(m_FieldAssetType)
		{
			#include <tu/client/assetsmacro.h>
		}
		
		#undef TU_MACRO_ASSETTYPE
		
		gui::CPopup::Update();
	}
};

/* POPUP BONE EDIT ****************************************************/

class CPopup_BoneEdit : public gui::CPopup
{
public:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_AssetMember;
	int m_AssetSubPath;
	CAssetPath m_SkeletonPath;
	
protected:
	class CItem : public gui::CTextButton
	{
	protected:
		CPopup_BoneEdit* m_pPopup;
		CAsset_Skeleton::CBonePath m_BonePath;
		
	protected:
		virtual void MouseClickAction()
		{
			m_pPopup->m_pAssetsEditor->AssetsManager()->SetAssetValue<CAsset_Skeleton::CBonePath>(
				m_pPopup->m_AssetPath,
				m_pPopup->m_AssetMember,
				m_pPopup->m_AssetSubPath,
				m_BonePath);
			m_pPopup->m_pAssetsEditor->RefreshAssetsEditor();
		}
		
	public:
		CItem(CPopup_BoneEdit* pPopup, CAsset_Skeleton::CBonePath BonePath) :
			gui::CTextButton(pPopup->m_pAssetsEditor->m_pGuiConfig, ""),
			m_pPopup(pPopup),
			m_BonePath(BonePath)
		{
			m_Centered = false;
			
			if(m_BonePath.IsNull())
			{
				SetText("None");
			}
			else
			{
				if(m_BonePath.GetSource() == CAsset_Skeleton::CBonePath::SRC_LOCAL)
				{
					char* pName = pPopup->m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
						pPopup->m_SkeletonPath,
						CAsset_Skeleton::BONE_NAME,
						CAsset_Skeleton::CSubPath::Bone(m_BonePath.GetId()).ConvertToInteger(),
						0);
					SetText(pName);
					SetIcon(CAssetsEditor::ICON_BONE);
				}
				else
				{
					CAsset_Skeleton* pSkeleton = pPopup->m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pPopup->m_SkeletonPath);
					if(pSkeleton && !pSkeleton->m_ParentPath.IsNull())
					{
						char* pName = pPopup->m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
							pSkeleton->m_ParentPath,
							CAsset_Skeleton::BONE_NAME,
							CAsset_Skeleton::CSubPath::Bone(m_BonePath.GetId()).ConvertToInteger(),
							0);
						SetText(pName);
						SetIcon(CAssetsEditor::ICON_BONE);
					}					
				}
			}
		}
	};

protected:
	gui::CVListLayout* m_Layout;
	
public:
	CPopup_BoneEdit(
		CAssetsEditor* pAssetsEditor,
		const gui::CRect& CreatorRect,
		int Alignment,
		CAssetPath AssetPath,
		int AssetMember,
		int AssetSubPath,
		CAssetPath SkeletonPath
	) :
		gui::CPopup(pAssetsEditor->m_pGuiConfig, CreatorRect, 250, 500, Alignment),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_AssetMember(AssetMember),
		m_AssetSubPath(AssetSubPath),
		m_SkeletonPath(SkeletonPath)
	{		
		m_Layout = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
		Add(m_Layout);
		
		Update();
	}
	
	virtual void Update()
	{
		m_Layout->Clear();
		
		CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(m_SkeletonPath);
		if(pSkeleton)
		{
			m_Layout->Add(new gui::CLabel(m_pConfig, "Local bones", gui::TEXTSTYLE_HEADER));\
			for(int i=0; i<pSkeleton->m_Bones.size(); i++)
			{
				CItem* pItem = new CItem(this, CAsset_Skeleton::CBonePath::Local(i));
				m_Layout->Add(pItem);
			}
			
			if(!pSkeleton->m_ParentPath.IsNull())
			{
				CAsset_Skeleton* pParentSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeleton->m_ParentPath);
				if(pParentSkeleton)
				{
					m_Layout->Add(new gui::CLabel(m_pConfig, "Parent bones", gui::TEXTSTYLE_HEADER));\
					for(int i=0; i<pParentSkeleton->m_Bones.size(); i++)
					{
						CItem* pItem = new CItem(this, CAsset_Skeleton::CBonePath::Parent(i));
						m_Layout->Add(pItem);
					}
				}
			}
		}
		
		gui::CPopup::Update();
	}
};

/* POPUP LAYER EDIT ****************************************************/

class CPopup_LayerEdit : public gui::CPopup
{
public:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_AssetMember;
	int m_AssetSubPath;
	CAssetPath m_SkeletonPath;
	
protected:
	class CItem : public gui::CTextButton
	{
	protected:
		CPopup_LayerEdit* m_pPopup;
		CAsset_Skeleton::CBonePath m_LayerPath;
		
	protected:
		virtual void MouseClickAction()
		{
			m_pPopup->m_pAssetsEditor->AssetsManager()->SetAssetValue<CAsset_Skeleton::CBonePath>(
				m_pPopup->m_AssetPath,
				m_pPopup->m_AssetMember,
				m_pPopup->m_AssetSubPath,
				m_LayerPath);
			m_pPopup->m_pAssetsEditor->RefreshAssetsEditor();
		}
		
	public:
		CItem(CPopup_LayerEdit* pPopup, CAsset_Skeleton::CBonePath LayerPath) :
			gui::CTextButton(pPopup->m_pAssetsEditor->m_pGuiConfig, ""),
			m_pPopup(pPopup),
			m_LayerPath(LayerPath)
		{
			m_Centered = false;
			
			if(m_LayerPath.IsNull())
			{
				SetText("None");
			}
			else
			{
				if(m_LayerPath.GetSource() == CAsset_Skeleton::CBonePath::SRC_LOCAL)
				{
					char* pName = pPopup->m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
						pPopup->m_SkeletonPath,
						CAsset_Skeleton::LAYER_NAME,
						CAsset_Skeleton::CSubPath::Layer(m_LayerPath.GetId()).ConvertToInteger(),
						0);
					SetText(pName);
					SetIcon(CAssetsEditor::ICON_BONE);
				}
				else
				{
					CAsset_Skeleton* pSkeleton = pPopup->m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pPopup->m_SkeletonPath);
					if(pSkeleton && !pSkeleton->m_ParentPath.IsNull())
					{
						char* pName = pPopup->m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
							pSkeleton->m_ParentPath,
							CAsset_Skeleton::LAYER_NAME,
							CAsset_Skeleton::CSubPath::Layer(m_LayerPath.GetId()).ConvertToInteger(),
							0);
						SetText(pName);
						SetIcon(CAssetsEditor::ICON_BONE);
					}					
				}
			}
		}
	};

protected:
	gui::CVListLayout* m_Layout;
	
public:
	CPopup_LayerEdit(
		CAssetsEditor* pAssetsEditor,
		const gui::CRect& CreatorRect,
		int Alignment,
		CAssetPath AssetPath,
		int AssetMember,
		int AssetSubPath,
		CAssetPath SkeletonPath
	) :
		gui::CPopup(pAssetsEditor->m_pGuiConfig, CreatorRect, 250, 500, Alignment),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_AssetMember(AssetMember),
		m_AssetSubPath(AssetSubPath),
		m_SkeletonPath(SkeletonPath)
	{		
		m_Layout = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
		Add(m_Layout);
		
		Update();
	}
	
	virtual void Update()
	{
		m_Layout->Clear();
		
		CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(m_SkeletonPath);
		if(pSkeleton)
		{
			m_Layout->Add(new gui::CLabel(m_pConfig, "Local layers", gui::TEXTSTYLE_HEADER));\
			for(int i=0; i<pSkeleton->m_Layers.size(); i++)
			{
				CItem* pItem = new CItem(this, CAsset_Skeleton::CBonePath::Local(i));
				m_Layout->Add(pItem);
			}
			
			if(!pSkeleton->m_ParentPath.IsNull())
			{
				CAsset_Skeleton* pParentSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(pSkeleton->m_ParentPath);
				if(pParentSkeleton)
				{
					m_Layout->Add(new gui::CLabel(m_pConfig, "Parent layers", gui::TEXTSTYLE_HEADER));\
					for(int i=0; i<pParentSkeleton->m_Layers.size(); i++)
					{
						CItem* pItem = new CItem(this, CAsset_Skeleton::CBonePath::Parent(i));
						m_Layout->Add(pItem);
					}
				}
			}
		}
		
		gui::CPopup::Update();
	}
};

/* POPUP SUBPATH EDIT *************************************************/

class CPopup_CharacterPartEdit : public gui::CPopup
{
public:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_AssetMember;
	int m_AssetSubPath;
	CAssetPath m_CharacterPath;
	
protected:
	class CItem : public gui::CTextButton
	{
	protected:
		CPopup_CharacterPartEdit* m_pPopup;
		CAsset_Character::CSubPath m_SubPath;
		
	protected:
		virtual void MouseClickAction()
		{
			m_pPopup->m_pAssetsEditor->AssetsManager()->SetAssetValue<int>(
				m_pPopup->m_AssetPath,
				m_pPopup->m_AssetMember,
				m_pPopup->m_AssetSubPath,
				m_SubPath.ConvertToInteger());
			m_pPopup->m_pAssetsEditor->RefreshAssetsEditor();
		}
		
	public:
		CItem(CPopup_CharacterPartEdit* pPopup, CAsset_Character::CSubPath SubPath) :
			gui::CTextButton(pPopup->m_pAssetsEditor->m_pGuiConfig, ""),
			m_pPopup(pPopup),
			m_SubPath(SubPath)
		{
			m_Centered = false;
			
			if(m_SubPath.IsNull())
			{
				SetText("None");
			}
			else
			{
				char* pName = pPopup->m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
					pPopup->m_CharacterPath,
					CAsset_Character::PART_NAME,
					m_SubPath.ConvertToInteger(),
					0);
				SetText(pName);
				SetIcon(CAssetsEditor::ICON_CHARACTERPART);
			}
		}
	};

protected:
	gui::CVListLayout* m_Layout;
	
public:
	CPopup_CharacterPartEdit(
		CAssetsEditor* pAssetsEditor,
		const gui::CRect& CreatorRect,
		int Alignment,
		CAssetPath AssetPath,
		int AssetMember,
		int AssetSubPath,
		CAssetPath CharacterPath
	) :
		gui::CPopup(pAssetsEditor->m_pGuiConfig, CreatorRect, 250, 500, Alignment),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_AssetMember(AssetMember),
		m_AssetSubPath(AssetSubPath),
		m_CharacterPath(CharacterPath)
	{		
		m_Layout = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
		Add(m_Layout);
		
		Update();
	}
	
	virtual void Update()
	{
		m_Layout->Clear();
		
		CAsset_Character* pCharacter = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Character>(m_CharacterPath);
		if(pCharacter)
		{
			for(int i=0; i<pCharacter->m_Parts.size(); i++)
			{
				CItem* pItem = new CItem(this, CAsset_Character::CSubPath::Part(i));
				m_Layout->Add(pItem);
			}
		}
		
		gui::CPopup::Update();
	}
};

/* POPUP COLOR EDIT ***************************************************/

class CPopup_ColorEdit : public gui::CPopup
{
public:
	class CColorSliderEdit : public gui::CWidget
	{	
	protected:
		CPopup_ColorEdit* m_pPopup;		
		bool m_Clicked;
		
	public:
		CColorSliderEdit(CPopup_ColorEdit* pPopup) :
			gui::CWidget(pPopup->m_pAssetsEditor->m_pGuiConfig),
			m_pPopup(pPopup),
			m_Clicked(false)
		{
			m_Rect.w = m_pConfig->m_ButtonHeight;
			m_Rect.h = m_pConfig->m_ButtonHeight;
		}
	public:
	
		virtual vec4 GetSliderValue(float Value) = 0;

		void RenderLines(float Cursor)
		{
			Graphics()->LinesBegin();
			Graphics()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			
			IGraphics::CLineItem Lines[4] = {
					IGraphics::CLineItem(m_Rect.x, m_Rect.y, m_Rect.x+m_Rect.w, m_Rect.y),
					IGraphics::CLineItem(m_Rect.x+m_Rect.w, m_Rect.y, m_Rect.x+m_Rect.w, m_Rect.y+m_Rect.h),
					IGraphics::CLineItem(m_Rect.x+m_Rect.w, m_Rect.y+m_Rect.h, m_Rect.x, m_Rect.y+m_Rect.h),
					IGraphics::CLineItem(m_Rect.x, m_Rect.y+m_Rect.h, m_Rect.x, m_Rect.y)
			};
			Graphics()->LinesDraw(Lines, 4);
			
			IGraphics::CLineItem CursorLine(m_Rect.x+m_Rect.w*Cursor, m_Rect.y, m_Rect.x+m_Rect.w*Cursor, m_Rect.y+m_Rect.h);
			Graphics()->LinesDraw(&CursorLine, 1);
			Graphics()->LinesEnd();
		}
		
		void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Rect.IsInside(X, Y) && m_Clicked)
			{
				float Value = static_cast<float>(X - m_Rect.x)/static_cast<float>(m_Rect.w);
				m_pPopup->SetColor(GetSliderValue(Value));
			}
		}

		void OnButtonClick(int X, int Y, int Button, int Count)
		{
			if(Button != KEY_MOUSE_1)
				return;
			
			if(m_Rect.IsInside(X, Y))
			{
				m_Clicked = true;
			}
		}

		void OnButtonRelease(int Button)
		{
			if(Button != KEY_MOUSE_1)
				return;
			
			m_Clicked = false;
		}
	};
	
	template<int C>
	class CRGBSliderEdit : public CColorSliderEdit
	{
	public:
		CRGBSliderEdit(CPopup_ColorEdit* pPopup) :
			CColorSliderEdit(pPopup)
		{
			
		}
		
		virtual void Render()
		{
			Graphics()->TextureClear();
			Graphics()->QuadsBegin();
			
			vec4 Color = m_pPopup->GetColor();
			vec4 minColor = Color;
			*(static_cast<float*>(&minColor.r)+C) = 0.0f;
			vec4 maxColor = Color;
			*(static_cast<float*>(&maxColor.r)+C) = 1.0f;
						
			IGraphics::CColorVertex ColorArray[4];
			ColorArray[0] = IGraphics::CColorVertex(0, minColor.r, minColor.g, minColor.b, 1.0f);
			ColorArray[1] = IGraphics::CColorVertex(1, maxColor.r, maxColor.g, maxColor.b, 1.0f);
			ColorArray[2] = IGraphics::CColorVertex(2, maxColor.r, maxColor.g, maxColor.b, 1.0f);
			ColorArray[3] = IGraphics::CColorVertex(3, minColor.r, minColor.g, minColor.b, 1.0f);
			Graphics()->SetColorVertex(ColorArray, 4);
		
			IGraphics::CQuadItem QuadItem(m_Rect.x, m_Rect.y, m_Rect.w, m_Rect.h);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
			Graphics()->QuadsEnd();
			
			float Cursor = *(static_cast<float*>(&Color.r)+C);
			RenderLines(Cursor);
		}

		virtual vec4 GetSliderValue(float Value)
		{
			vec4 Color = m_pPopup->GetColor();
			*(static_cast<float*>(&Color.r)+C) = Value;
			return Color;
		}
	};
	
	class CAlphaSliderEdit : public CRGBSliderEdit<3>
	{		
	public:
		CAlphaSliderEdit(CPopup_ColorEdit* pPopup) :
			CRGBSliderEdit<3>(pPopup)
		{
			
		}
		
	public:
		virtual void Render()
		{
			Graphics()->TextureClear();
			Graphics()->QuadsBegin();
			
			float SquareSize = m_Rect.h/3.0f;
			int nbColumns = m_Rect.w / SquareSize;
			
			for(int j=0; j<3; j++)
			{
				for(int i=0; i<nbColumns; i++)
				{
					if(i%2==j%2)
						Graphics()->SetColor(0.4f, 0.4f, 0.4f, 1.0f);
					else
						Graphics()->SetColor(0.6f, 0.6f, 0.6f, 1.0f);
					IGraphics::CQuadItem QuadItem(m_Rect.x + i*SquareSize, m_Rect.y + j*SquareSize, SquareSize, SquareSize);
					Graphics()->QuadsDrawTL(&QuadItem, 1);
				}
			}
			
			vec4 Color = m_pPopup->GetColor();
			
			IGraphics::CColorVertex ColorArray[4];
			ColorArray[0] = IGraphics::CColorVertex(0, 0.0f, 0.0f, 0.0f, 0.0f);
			ColorArray[1] = IGraphics::CColorVertex(1, Color.r, Color.g, Color.b, 1.0f);
			ColorArray[2] = IGraphics::CColorVertex(2, Color.r, Color.g, Color.b, 1.0f);
			ColorArray[3] = IGraphics::CColorVertex(3, 0.0f, 0.0f, 0.0f, 0.0f);
			Graphics()->SetColorVertex(ColorArray, 4);
		
			IGraphics::CQuadItem QuadItem(m_Rect.x, m_Rect.y, m_Rect.w, m_Rect.h);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
			
			Graphics()->QuadsEnd();
			
			float Cursor = Color.a;
			RenderLines(Cursor);
		}
	};
	
	template<int C>
	class CHSVSliderEdit : public CColorSliderEdit
	{
	public:
		CHSVSliderEdit(CPopup_ColorEdit* pPopup) :
			CColorSliderEdit(pPopup)
		{
			
		}
		
		virtual void Render()
		{
			Graphics()->TextureClear();
			Graphics()->QuadsBegin();

			vec4 Color = m_pPopup->GetColor();
			vec3 ColorHSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
			
			int nbCells = 6;
			float Offset = m_Rect.w/static_cast<float>(nbCells);
			for(int i=0; i<nbCells; i++)
			{
				float Value0 = i*1.0f/nbCells;
				float Value1 = (i+1)*1.0f/nbCells;
				vec3 ColorHSV0 = ColorHSV;
				*(static_cast<float*>(&ColorHSV0.r)+C) = Value0;
				vec3 ColorHSV1 = ColorHSV;		
				*(static_cast<float*>(&ColorHSV1.r)+C) = Value1;
				vec3 ColorRGB0 = HsvToRgb(vec3(ColorHSV0.r, ColorHSV0.g, ColorHSV0.b));
				vec3 ColorRGB1 = HsvToRgb(vec3(ColorHSV1.r, ColorHSV1.g, ColorHSV1.b));

				IGraphics::CColorVertex ColorArray[4];
				ColorArray[0] = IGraphics::CColorVertex(0, ColorRGB0.r, ColorRGB0.g, ColorRGB0.b, 1.0f);
				ColorArray[1] = IGraphics::CColorVertex(1, ColorRGB1.r, ColorRGB1.g, ColorRGB1.b, 1.0f);
				ColorArray[2] = IGraphics::CColorVertex(2, ColorRGB1.r, ColorRGB1.g, ColorRGB1.b, 1.0f);
				ColorArray[3] = IGraphics::CColorVertex(3, ColorRGB0.r, ColorRGB0.g, ColorRGB0.b, 1.0f);
				Graphics()->SetColorVertex(ColorArray, 4);
				IGraphics::CQuadItem QuadItem(m_Rect.x+i*Offset, m_Rect.y, Offset, m_Rect.h);
				Graphics()->QuadsDrawTL(&QuadItem, 1);
			}
		
			Graphics()->QuadsEnd();

			float Cursor = *(static_cast<float*>(&ColorHSV.r)+C);
			RenderLines(Cursor);
		}

		virtual vec4 GetSliderValue(float Value)
		{
			vec4 Color = m_pPopup->GetColor();
			vec3 HSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
			*(static_cast<float*>(&HSV.r)+C) = Value;
			vec3 RGB = HsvToRgb(vec3(HSV.r, HSV.g, HSV.b));
			return vec4(RGB.r, RGB.g, RGB.b, Color.a);
		}
	};
	
	class CHueVSliderEdit : public gui::CWidget
	{	
	protected:
		CPopup_ColorEdit* m_pPopup;		
		bool m_Clicked;
		
	public:
		CHueVSliderEdit(CPopup_ColorEdit* pPopup) :
			gui::CWidget(pPopup->m_pAssetsEditor->m_pGuiConfig),
			m_pPopup(pPopup),
			m_Clicked(false)
		{
			m_Rect.w = m_pConfig->m_ButtonHeight;
			m_Rect.h = 250;
		}
	public:

		void RenderLines(float Cursor)
		{
			Graphics()->LinesBegin();
			Graphics()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			
			IGraphics::CLineItem Lines[4] = {
					IGraphics::CLineItem(m_Rect.x, m_Rect.y, m_Rect.x+m_Rect.w, m_Rect.y),
					IGraphics::CLineItem(m_Rect.x+m_Rect.w, m_Rect.y, m_Rect.x+m_Rect.w, m_Rect.y+m_Rect.h),
					IGraphics::CLineItem(m_Rect.x+m_Rect.w, m_Rect.y+m_Rect.h, m_Rect.x, m_Rect.y+m_Rect.h),
					IGraphics::CLineItem(m_Rect.x, m_Rect.y+m_Rect.h, m_Rect.x, m_Rect.y)
			};
			Graphics()->LinesDraw(Lines, 4);
			
			IGraphics::CLineItem CursorLine(m_Rect.x, m_Rect.y+m_Rect.h*Cursor, m_Rect.x+m_Rect.w, m_Rect.y+m_Rect.h*Cursor);
			Graphics()->LinesDraw(&CursorLine, 1);
			Graphics()->LinesEnd();
		}
		
		virtual void Render()
		{
			Graphics()->TextureClear();
			Graphics()->QuadsBegin();
			
			int nbCells = 6;
			float Offset = m_Rect.h/static_cast<float>(nbCells);
			for(int i=0; i<nbCells; i++)
			{
				float Value0 = i*1.0f/nbCells;
				float Value1 = (i+1)*1.0f/nbCells;
				vec3 ColorRGB0 = HsvToRgb(vec3(Value0, 1.0f, 1.0f));
				vec3 ColorRGB1 = HsvToRgb(vec3(Value1, 1.0f, 1.0f));

				IGraphics::CColorVertex ColorArray[4];
				ColorArray[0] = IGraphics::CColorVertex(0, ColorRGB0.r, ColorRGB0.g, ColorRGB0.b, 1.0f);
				ColorArray[1] = IGraphics::CColorVertex(1, ColorRGB0.r, ColorRGB0.g, ColorRGB0.b, 1.0f);
				ColorArray[2] = IGraphics::CColorVertex(2, ColorRGB1.r, ColorRGB1.g, ColorRGB1.b, 1.0f);
				ColorArray[3] = IGraphics::CColorVertex(3, ColorRGB1.r, ColorRGB1.g, ColorRGB1.b, 1.0f);
				Graphics()->SetColorVertex(ColorArray, 4);
				IGraphics::CQuadItem QuadItem(m_Rect.x, m_Rect.y+i*Offset, m_Rect.w, Offset);
				Graphics()->QuadsDrawTL(&QuadItem, 1);
			}
		
			Graphics()->QuadsEnd();

			vec4 Color = m_pPopup->GetColor();
			vec3 ColorHSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
			RenderLines(ColorHSV.r);
		}

		virtual vec4 GetSliderValue(float Value)
		{
			vec4 Color = m_pPopup->GetColor();
			vec3 HSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
			HSV.r = Value;
			vec3 RGB = HsvToRgb(vec3(HSV.r, HSV.g, HSV.b));
			return vec4(RGB.r, RGB.g, RGB.b, Color.a);
		}
		
		void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Rect.IsInside(X, Y) && m_Clicked)
			{
				float Value = static_cast<float>(Y - m_Rect.y)/static_cast<float>(m_Rect.h);
				m_pPopup->SetColor(GetSliderValue(Value));
			}
		}

		void OnButtonClick(int X, int Y, int Button, int Count)
		{
			if(Button != KEY_MOUSE_1)
				return;
			
			if(m_Rect.IsInside(X, Y))
			{
				m_Clicked = true;
			}
		}

		void OnButtonRelease(int Button)
		{
			if(Button != KEY_MOUSE_1)
				return;
			
			m_Clicked = false;
		}
	};
	
	class CSquarePicker : public gui::CWidget
	{	
	protected:
		CPopup_ColorEdit* m_pPopup;		
		bool m_Clicked;
		
	public:
		CSquarePicker(CPopup_ColorEdit* pPopup) :
			gui::CWidget(pPopup->m_pAssetsEditor->m_pGuiConfig),
			m_pPopup(pPopup),
			m_Clicked(false)
		{
			m_Rect.w = 250;
			m_Rect.h = 250;
		}
	public:		
		virtual void Render()
		{
			Graphics()->TextureClear();
			Graphics()->QuadsBegin();

			vec4 Color = m_pPopup->GetColor();
			vec3 ColorHSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
			vec3 ColorRGB0 = HsvToRgb(vec3(ColorHSV.x, 0.0f, 1.0f));
			vec3 ColorRGB1 = HsvToRgb(vec3(ColorHSV.x, 1.0f, 1.0f));

			{
				IGraphics::CColorVertex ColorArray[4];
				ColorArray[0] = IGraphics::CColorVertex(0, ColorRGB0.r, ColorRGB0.g, ColorRGB0.b, 1.0f);
				ColorArray[1] = IGraphics::CColorVertex(1, ColorRGB1.r, ColorRGB1.g, ColorRGB1.b, 1.0f);
				ColorArray[2] = IGraphics::CColorVertex(2, ColorRGB1.r, ColorRGB1.g, ColorRGB1.b, 1.0f);
				ColorArray[3] = IGraphics::CColorVertex(3, ColorRGB0.r, ColorRGB0.g, ColorRGB0.b, 1.0f);
				Graphics()->SetColorVertex(ColorArray, 4);
				IGraphics::CQuadItem QuadItem(m_Rect.x, m_Rect.y, m_Rect.w, m_Rect.h);
				Graphics()->QuadsDrawTL(&QuadItem, 1);
			}
			
			{
				IGraphics::CColorVertex ColorArray[4];
				ColorArray[0] = IGraphics::CColorVertex(0, 0.0f, 0.0f, 0.0f, 0.0f);
				ColorArray[1] = IGraphics::CColorVertex(1, 0.0f, 0.0f, 0.0f, 0.0f);
				ColorArray[2] = IGraphics::CColorVertex(2, 0.0f, 0.0f, 0.0f, 1.0f);
				ColorArray[3] = IGraphics::CColorVertex(3, 0.0f, 0.0f, 0.0f, 1.0f);
				Graphics()->SetColorVertex(ColorArray, 4);
				IGraphics::CQuadItem QuadItem(m_Rect.x, m_Rect.y, m_Rect.w, m_Rect.h);
				Graphics()->QuadsDrawTL(&QuadItem, 1);
			}
		
			Graphics()->QuadsEnd();

			Graphics()->LinesBegin();
			Graphics()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			
			IGraphics::CLineItem Lines[4] = {
					IGraphics::CLineItem(m_Rect.x, m_Rect.y, m_Rect.x+m_Rect.w, m_Rect.y),
					IGraphics::CLineItem(m_Rect.x+m_Rect.w, m_Rect.y, m_Rect.x+m_Rect.w, m_Rect.y+m_Rect.h),
					IGraphics::CLineItem(m_Rect.x+m_Rect.w, m_Rect.y+m_Rect.h, m_Rect.x, m_Rect.y+m_Rect.h),
					IGraphics::CLineItem(m_Rect.x, m_Rect.y+m_Rect.h, m_Rect.x, m_Rect.y)
			};
			
			vec2 CursorPos = vec2(m_Rect.x + m_Rect.w*ColorHSV.g, m_Rect.y + m_Rect.h*(1.0f-ColorHSV.b));
			IGraphics::CLineItem CursorLines[4] = {
					IGraphics::CLineItem(CursorPos.x - 2.0f, CursorPos.y - 2.0f, CursorPos.x + 2.0f, CursorPos.y - 2.0f),
					IGraphics::CLineItem(CursorPos.x + 2.0f, CursorPos.y - 2.0f, CursorPos.x + 2.0f, CursorPos.y + 2.0f),
					IGraphics::CLineItem(CursorPos.x + 2.0f, CursorPos.y + 2.0f, CursorPos.x - 2.0f, CursorPos.y + 2.0f),
					IGraphics::CLineItem(CursorPos.x - 2.0f, CursorPos.y + 2.0f, CursorPos.x - 2.0f, CursorPos.y - 2.0f),
			};
			
			Graphics()->LinesDraw(CursorLines, 4);
			Graphics()->LinesEnd();
		}
		
		void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Rect.IsInside(X, Y) && m_Clicked)
			{
				float Value0 = static_cast<float>(X - m_Rect.x)/static_cast<float>(m_Rect.w);
				float Value1 = 1.0f - static_cast<float>(Y - m_Rect.y)/static_cast<float>(m_Rect.h);
				
				vec4 Color = m_pPopup->GetColor();
				vec3 HSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
				HSV.g = Value0;
				HSV.b = Value1;
				vec3 RGB = HsvToRgb(vec3(HSV.r, HSV.g, HSV.b));

				m_pPopup->SetColor(vec4(RGB.r, RGB.g, RGB.b, Color.a));
			}
		}

		void OnButtonClick(int X, int Y, int Button, int Count)
		{
			if(Button != KEY_MOUSE_1)
				return;
			
			if(m_Rect.IsInside(X, Y))
			{
				m_Clicked = true;
			}
		}

		void OnButtonRelease(int Button)
		{
			if(Button != KEY_MOUSE_1)
				return;
			
			m_Clicked = false;
		}
	};
	
	class CWheelPicker : public gui::CWidget
	{	
	protected:
		CPopup_ColorEdit* m_pPopup;		
		int m_Clicked;
		
	public:
		CWheelPicker(CPopup_ColorEdit* pPopup) :
			gui::CWidget(pPopup->m_pAssetsEditor->m_pGuiConfig),
			m_pPopup(pPopup),
			m_Clicked(0)
		{
			m_Rect.w = 250;
			m_Rect.h = 250;
		}
	public:		
		virtual void Render()
		{
			Graphics()->TextureClear();

			vec2 Center(m_Rect.x + m_Rect.w/2, m_Rect.y + m_Rect.h/2);
			float Radius1 = min(m_Rect.w, m_Rect.h)/2.0f - 1.0f;
			float Radius0 = Radius1*0.8f;

			vec4 Color = m_pPopup->GetColor();
			vec3 ColorHSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
			vec3 ColorRGB0 = HsvToRgb(vec3(ColorHSV.x, 0.0f, 1.0f));
			vec3 ColorRGB1 = HsvToRgb(vec3(ColorHSV.x, 1.0f, 1.0f));

			float CursorAngle = ColorHSV.r*2.0f*pi;
			
			const int nbSegments = 64;
			const float deltaAngle = pi*2.0f/nbSegments;
			
			Graphics()->QuadsBegin();
			
			for(int i=0; i<nbSegments; i++)
			{
				float Hue0 = i*1.0f/nbSegments;
				float Hue1 = (i+1)*1.0f/nbSegments;
				vec3 ColorRGB0 = HsvToRgb(vec3(Hue0, 1.0f, 1.0f));
				vec3 ColorRGB1 = HsvToRgb(vec3(Hue1, 1.0f, 1.0f));

				IGraphics::CColorVertex ColorArray[4];
				ColorArray[0] = IGraphics::CColorVertex(0, ColorRGB0.r, ColorRGB0.g, ColorRGB0.b, 1.0f);
				ColorArray[1] = IGraphics::CColorVertex(1, ColorRGB0.r, ColorRGB0.g, ColorRGB0.b, 1.0f);
				ColorArray[2] = IGraphics::CColorVertex(2, ColorRGB1.r, ColorRGB1.g, ColorRGB1.b, 1.0f);
				ColorArray[3] = IGraphics::CColorVertex(3, ColorRGB1.r, ColorRGB1.g, ColorRGB1.b, 1.0f);
				Graphics()->SetColorVertex(ColorArray, 4);
				
				IGraphics::CFreeformItem Freeform(
					Center.x + Radius1*cos(i*deltaAngle), Center.y + Radius1*sin(i*deltaAngle),
					Center.x + Radius0*cos(i*deltaAngle), Center.y + Radius0*sin(i*deltaAngle),
					Center.x + Radius1*cos((i+1)*deltaAngle), Center.y + Radius1*sin((i+1)*deltaAngle),
					Center.x + Radius0*cos((i+1)*deltaAngle), Center.y + Radius0*sin((i+1)*deltaAngle));
				Graphics()->QuadsDrawFreeform(&Freeform, 1);
			}
			
			{
				IGraphics::CColorVertex ColorArray[4];
				vec3 Hue = HsvToRgb(vec3(ColorHSV.r, 1.0f, 1.0f));
				vec3 Saturation = HsvToRgb(vec3(ColorHSV.r, 1.0f, 0.0f));
				vec3 Value = HsvToRgb(vec3(ColorHSV.r, 0.0f, 1.0f));
				
				ColorArray[0] = IGraphics::CColorVertex(0, Saturation.r, Saturation.g, Saturation.b, 1.0f);
				ColorArray[1] = IGraphics::CColorVertex(1, Saturation.r, Saturation.g, Saturation.b, 1.0f);
				ColorArray[2] = IGraphics::CColorVertex(2, Value.r, Value.g, Value.b, 1.0f);
				ColorArray[3] = IGraphics::CColorVertex(3, Hue.r, Hue.g, Hue.b, 1.0f);
				Graphics()->SetColorVertex(ColorArray, 4);
				
				IGraphics::CFreeformItem Freeform(
					Center.x + Radius0*cos(CursorAngle-2.0f*pi/3.0f), Center.y + Radius0*sin(CursorAngle-2.0f*pi/3.0f),
					Center.x + Radius0*cos(CursorAngle-2.0f*pi/3.0f), Center.y + Radius0*sin(CursorAngle-2.0f*pi/3.0f),
					Center.x + Radius0*cos(CursorAngle+2.0f*pi/3.0f), Center.y + Radius0*sin(CursorAngle+2.0f*pi/3.0f),
					Center.x + Radius0*cos(CursorAngle), Center.y + Radius0*sin(CursorAngle));
				Graphics()->QuadsDrawFreeform(&Freeform, 1);
			}
			
			Graphics()->QuadsEnd();
			
			Graphics()->LinesBegin();
			Graphics()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			
			for(int i=0; i<nbSegments; i++)
			{
				IGraphics::CLineItem Line1(Center.x + Radius1*cos(i*deltaAngle), Center.y + Radius1*sin(i*deltaAngle), Center.x + Radius1*cos((i+1)*deltaAngle), Center.y + Radius1*sin((i+1)*deltaAngle));
				Graphics()->LinesDraw(&Line1, 1);
				IGraphics::CLineItem Line2(Center.x + Radius0*cos(i*deltaAngle), Center.y + Radius0*sin(i*deltaAngle), Center.x + Radius0*cos((i+1)*deltaAngle), Center.y + Radius0*sin((i+1)*deltaAngle));
				Graphics()->LinesDraw(&Line2, 1);
			}
			{
				vec2 HuePos(Center.x + Radius0*cos(CursorAngle), Center.y + Radius0*sin(CursorAngle));
				vec2 SaturationPos(Center.x + Radius0*cos(CursorAngle+2.0f*pi/3.0f), Center.y + Radius0*sin(CursorAngle+2.0f*pi/3.0f));
				vec2 ValuePos(Center.x + Radius0*cos(CursorAngle-2.0f*pi/3.0f), Center.y + Radius0*sin(CursorAngle-2.0f*pi/3.0f));
				vec2 Origin = (SaturationPos + HuePos)/2.0f;
				
				IGraphics::CLineItem TriangleLines[3] = {
					IGraphics::CLineItem(HuePos.x, HuePos.y, SaturationPos.x, SaturationPos.y),
					IGraphics::CLineItem(SaturationPos.x, SaturationPos.y, ValuePos.x, ValuePos.y),
					IGraphics::CLineItem(ValuePos.x, ValuePos.y, HuePos.x, HuePos.y),
				};
				Graphics()->LinesDraw(TriangleLines, 3);
				
				vec2 CursorPos = Origin + (HuePos - Origin)*2.0f*(ColorHSV.g-0.5f)*ColorHSV.b + (ValuePos - Origin)*(1.0f-ColorHSV.b);
				IGraphics::CLineItem CursorLines[4] = {
						IGraphics::CLineItem(CursorPos.x - 2.0f, CursorPos.y - 2.0f, CursorPos.x + 2.0f, CursorPos.y - 2.0f),
						IGraphics::CLineItem(CursorPos.x + 2.0f, CursorPos.y - 2.0f, CursorPos.x + 2.0f, CursorPos.y + 2.0f),
						IGraphics::CLineItem(CursorPos.x + 2.0f, CursorPos.y + 2.0f, CursorPos.x - 2.0f, CursorPos.y + 2.0f),
						IGraphics::CLineItem(CursorPos.x - 2.0f, CursorPos.y + 2.0f, CursorPos.x - 2.0f, CursorPos.y - 2.0f),
				};
				Graphics()->LinesDraw(CursorLines, 4);
			}
			
			Graphics()->LinesEnd();
		}
		
		void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Clicked == 1)
			{
				vec2 Center(m_Rect.x + m_Rect.w/2, m_Rect.y + m_Rect.h/2);
				vec2 CursorPos(X, Y);
				float Hue = angle(CursorPos-Center)/(2.0f*pi);
				if(Hue < 0.0f) Hue += 1.0f;
				
				vec4 Color = m_pPopup->GetColor();
				vec3 HSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
				vec3 RGB = HsvToRgb(vec3(Hue, HSV.g, HSV.b));

				m_pPopup->SetColor(vec4(RGB.r, RGB.g, RGB.b, Color.a));
			}
			else if(m_Clicked == 2)
			{
				vec4 Color = m_pPopup->GetColor();
				vec3 ColorHSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
				float CursorAngle = ColorHSV.r*2.0f*pi;
				
				float Radius1 = min(m_Rect.w, m_Rect.h)/2.0f - 1.0f;
				float Radius0 = Radius1*0.8f;
				
				vec2 Center(m_Rect.x + m_Rect.w/2, m_Rect.y + m_Rect.h/2);
				vec2 HuePos(Center.x + Radius0*cos(CursorAngle), Center.y + Radius0*sin(CursorAngle));
				vec2 SaturationPos(Center.x + Radius0*cos(CursorAngle+2.0f*pi/3.0f), Center.y + Radius0*sin(CursorAngle+2.0f*pi/3.0f));
				vec2 ValuePos(Center.x + Radius0*cos(CursorAngle-2.0f*pi/3.0f), Center.y + Radius0*sin(CursorAngle-2.0f*pi/3.0f));
				vec2 Origin = (SaturationPos + HuePos)/2.0f;
				float Size = length(SaturationPos - HuePos);
				float Size2 = length(ValuePos - Origin);
				vec2 CursorPos(X, Y);
				
				float Value = clamp(1.0f-dot(normalize(ValuePos - Origin), CursorPos - Origin)/Size2, 0.0f, 1.0f);
				vec2 ValueOrigin = Origin + (ValuePos - Origin)*(1.0f-Value) - (HuePos - SaturationPos)*Value/2.0f;
				float Saturation = clamp(length(CursorPos - ValueOrigin)/(Size*Value), 0.0f, 1.0f);
				
				vec3 RGB = HsvToRgb(vec3(ColorHSV.r, Saturation, Value));

				m_pPopup->SetColor(vec4(RGB.r, RGB.g, RGB.b, Color.a));
			}
		}

		void OnButtonClick(int X, int Y, int Button, int Count)
		{
			if(Button != KEY_MOUSE_1)
				return;
			
			vec4 Color = m_pPopup->GetColor();
			vec3 ColorHSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
			float CursorAngle = ColorHSV.r*2.0f*pi;
			
			float Radius1 = min(m_Rect.w, m_Rect.h)/2.0f - 1.0f;
			float Radius0 = Radius1*0.8f;
			
			vec2 Center(m_Rect.x + m_Rect.w/2, m_Rect.y + m_Rect.h/2);
			vec2 HuePos(Center.x + Radius0*cos(CursorAngle), Center.y + Radius0*sin(CursorAngle));
			vec2 SaturationPos(Center.x + Radius0*cos(CursorAngle+2.0f*pi/3.0f), Center.y + Radius0*sin(CursorAngle+2.0f*pi/3.0f));
			vec2 ValuePos(Center.x + Radius0*cos(CursorAngle-2.0f*pi/3.0f), Center.y + Radius0*sin(CursorAngle-2.0f*pi/3.0f));
			vec2 Origin = (SaturationPos + HuePos)/2.0f;
				
			vec2 CursorPos(X, Y);
			float d = distance(Center, CursorPos);
			if(Radius0 < d && d < Radius1)
			{
				m_Clicked = 1;
			}
			else
			{
				vec2 Vertices[3];
				Vertices[0] = HuePos - CursorPos;
				Vertices[1] = SaturationPos - CursorPos;
				Vertices[2] = ValuePos - CursorPos;
				
				bool isInside = true;
				for(int i=0; i<3; i++)
				{
					vec2 Edge = Vertices[(i+1)%3] - Vertices[i];
					if(Edge.x * Vertices[i].y - Edge.y * Vertices[i].x > 0.0f)
					{
						isInside = false;
						break;
					}
				}
				
				if(isInside)
				{
					m_Clicked = 2;
				}
				else m_Clicked = 0;
			}
		}

		void OnButtonRelease(int Button)
		{
			if(Button != KEY_MOUSE_1)
				return;
			
			m_Clicked = 0;
		}
	};

	template<int C>
	class CRGBIntegerEdit : public gui::CAbstractFloatEdit
	{
	protected:
		CPopup_ColorEdit* m_pPopup;
		
		virtual void SetValue(float Value)
		{
			vec4 Color = m_pPopup->GetColor();
			*(static_cast<float*>(&Color.r)+C) = Value/255.0f;
			m_pPopup->SetColor(Color);
		}
		
		virtual float GetValue()
		{
			vec4 Color = m_pPopup->GetColor();
			float Value = *(static_cast<float*>(&Color.r)+C);
			return Value*255.0f;
		}
		
		virtual void ApplyFormat()
		{
			str_format(m_aFloatText, sizeof(m_aFloatText), "%.00f", GetValue());
		}
		
	public:
		CRGBIntegerEdit(CPopup_ColorEdit* pPopup) :
			gui::CAbstractFloatEdit(pPopup->m_pConfig),
			m_pPopup(pPopup)
		{
			
		}
	};

	template<int C>
	class CRGBFloatEdit : public gui::CAbstractFloatEdit
	{
	protected:
		CPopup_ColorEdit* m_pPopup;
		
		virtual void SetValue(float Value)
		{
			vec4 Color = m_pPopup->GetColor();
			*(static_cast<float*>(&Color.r)+C) = Value;
			m_pPopup->SetColor(Color);
		}
		
		virtual float GetValue()
		{
			vec4 Color = m_pPopup->GetColor();
			float Value = *(static_cast<float*>(&Color.r)+C);
			return Value;
		}
		
		virtual void ApplyFormat()
		{
			str_format(m_aFloatText, sizeof(m_aFloatText), "%.03f", GetValue());
		}
		
	public:
		CRGBFloatEdit(CPopup_ColorEdit* pPopup) :
			gui::CAbstractFloatEdit(pPopup->m_pConfig),
			m_pPopup(pPopup)
		{
			
		}
	};

	template<int C>
	class CHSVIntegerEdit : public gui::CAbstractFloatEdit
	{
	protected:
		CPopup_ColorEdit* m_pPopup;
		
		virtual void SetValue(float Value)
		{
			vec4 Color = m_pPopup->GetColor();
			vec3 ColorHSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
			*(static_cast<float*>(&ColorHSV.r)+C) = Value/255.0f;
			vec3 ColorRGB = HsvToRgb(vec3(ColorHSV.r, ColorHSV.g, ColorHSV.b));
			m_pPopup->SetColor(vec4(ColorRGB.r, ColorRGB.g, ColorRGB.b, Color.a));
		}
		
		virtual float GetValue()
		{
			vec4 Color = m_pPopup->GetColor();
			vec3 ColorHSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
			float Value = *(static_cast<float*>(&ColorHSV.r)+C);
			return Value*255.0f;
		}
		
		virtual void ApplyFormat()
		{
			str_format(m_aFloatText, sizeof(m_aFloatText), "%.00f", GetValue());
		}
		
	public:
		CHSVIntegerEdit(CPopup_ColorEdit* pPopup) :
			gui::CAbstractFloatEdit(pPopup->m_pConfig),
			m_pPopup(pPopup)
		{
			
		}
	};

	template<int C>
	class CHSVFloatEdit : public gui::CAbstractFloatEdit
	{
	protected:
		CPopup_ColorEdit* m_pPopup;
		
		virtual void SetValue(float Value)
		{
			vec4 Color = m_pPopup->GetColor();
			vec3 ColorHSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
			*(static_cast<float*>(&ColorHSV.r)+C) = Value;
			vec3 ColorRGB = HsvToRgb(vec3(ColorHSV.r, ColorHSV.g, ColorHSV.b));
			m_pPopup->SetColor(vec4(ColorRGB.r, ColorRGB.g, ColorRGB.b, Color.a));
		}
		
		virtual float GetValue()
		{
			vec4 Color = m_pPopup->GetColor();
			vec3 ColorHSV = RgbToHsv(vec3(Color.r, Color.g, Color.b));
			float Value = *(static_cast<float*>(&ColorHSV.r)+C);
			return Value;
		}
		
		virtual void ApplyFormat()
		{
			str_format(m_aFloatText, sizeof(m_aFloatText), "%.03f", GetValue());
		}
		
	public:
		CHSVFloatEdit(CPopup_ColorEdit* pPopup) :
			gui::CAbstractFloatEdit(pPopup->m_pConfig),
			m_pPopup(pPopup)
		{
			
		}
	};
	
public:
	CAssetsEditor* m_pAssetsEditor;
	CAssetPath m_AssetPath;
	int m_AssetMember;
	int m_AssetSubPath;
	
protected:
	gui::CTabs* m_Tabs;
	gui::CVListLayout* m_TabRGB;
	gui::CVListLayout* m_TabHSV;
	gui::CVListLayout* m_TabSquare;
	gui::CVListLayout* m_TabWheel;
	
public:
	CPopup_ColorEdit(
		CAssetsEditor* pAssetsEditor,
		const gui::CRect& CreatorRect,
		int Alignment,
		CAssetPath AssetPath,
		int AssetMember,
		int AssetSubPath
	) :
		gui::CPopup(pAssetsEditor->m_pGuiConfig, CreatorRect, 300, 380, Alignment),
		m_pAssetsEditor(pAssetsEditor),
		m_AssetPath(AssetPath),
		m_AssetMember(AssetMember),
		m_AssetSubPath(AssetSubPath)
	{		
		m_Tabs = new gui::CTabs(pAssetsEditor->m_pGuiConfig);
		Add(m_Tabs);
		
		Update();
		
		m_TabRGB = new gui::CVListLayout(pAssetsEditor->m_pGuiConfig);
		m_Tabs->AddTab(m_TabRGB, CAssetsEditor::ICON_COLORPICKER_RGB, "RGB Color Chooser");
		
		{
			gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
			pLayout->SetHeight(m_pConfig->m_ButtonHeight);
			m_TabRGB->Add(pLayout);
			
			gui::CLabel* pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Red:");
			pLayout->Add(pLabel);
			pLayout->Add(new CRGBFloatEdit<0>(this));
			pLayout->Add(new CRGBIntegerEdit<0>(this));
			
			m_TabRGB->Add(new CRGBSliderEdit<0>(this));
		}
		{
			gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
			pLayout->SetHeight(m_pConfig->m_ButtonHeight);
			m_TabRGB->Add(pLayout);
			
			gui::CLabel* pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Green:");
			pLayout->Add(pLabel);
			pLayout->Add(new CRGBFloatEdit<1>(this));
			pLayout->Add(new CRGBIntegerEdit<1>(this));
			
			m_TabRGB->Add(new CRGBSliderEdit<1>(this));
		}
		{
			gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
			pLayout->SetHeight(m_pConfig->m_ButtonHeight);
			m_TabRGB->Add(pLayout);
			
			gui::CLabel* pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Blue:");
			pLayout->Add(pLabel);
			pLayout->Add(new CRGBFloatEdit<2>(this));
			pLayout->Add(new CRGBIntegerEdit<2>(this));
			
			m_TabRGB->Add(new CRGBSliderEdit<2>(this));
		}
		{
			gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
			pLayout->SetHeight(m_pConfig->m_ButtonHeight);
			m_TabRGB->Add(pLayout);
			
			gui::CLabel* pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Alpha:");
			pLayout->Add(pLabel);
			pLayout->Add(new CRGBFloatEdit<3>(this));
			pLayout->Add(new CRGBIntegerEdit<3>(this));
			
			m_TabRGB->Add(new CAlphaSliderEdit(this));
		}
		
		m_TabHSV = new gui::CVListLayout(pAssetsEditor->m_pGuiConfig);
		m_Tabs->AddTab(m_TabHSV, CAssetsEditor::ICON_COLORPICKER_HSV, "HSV Color Chooser");
		{
			gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
			pLayout->SetHeight(m_pConfig->m_ButtonHeight);
			m_TabHSV->Add(pLayout);
			
			gui::CLabel* pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Hue:");
			pLayout->Add(pLabel);
			pLayout->Add(new CHSVFloatEdit<0>(this));
			pLayout->Add(new CHSVIntegerEdit<0>(this));
			
			m_TabHSV->Add(new CHSVSliderEdit<0>(this));
		}
		{
			gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
			pLayout->SetHeight(m_pConfig->m_ButtonHeight);
			m_TabHSV->Add(pLayout);
			
			gui::CLabel* pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Saturation:");
			pLayout->Add(pLabel);
			pLayout->Add(new CHSVFloatEdit<1>(this));
			pLayout->Add(new CHSVIntegerEdit<1>(this));
			
			m_TabHSV->Add(new CHSVSliderEdit<1>(this));
		}
		{
			gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
			pLayout->SetHeight(m_pConfig->m_ButtonHeight);
			m_TabHSV->Add(pLayout);
			
			gui::CLabel* pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Value:");
			pLayout->Add(pLabel);
			pLayout->Add(new CHSVFloatEdit<2>(this));
			pLayout->Add(new CHSVIntegerEdit<2>(this));
			
			m_TabHSV->Add(new CHSVSliderEdit<2>(this));
		}
		{
			gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
			pLayout->SetHeight(m_pConfig->m_ButtonHeight);
			m_TabHSV->Add(pLayout);
			
			gui::CLabel* pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Alpha:");
			pLayout->Add(pLabel);
			pLayout->Add(new CRGBFloatEdit<3>(this));
			pLayout->Add(new CRGBIntegerEdit<3>(this));
			
			m_TabHSV->Add(new CAlphaSliderEdit(this));
		}
		
		m_TabSquare = new gui::CVListLayout(pAssetsEditor->m_pGuiConfig);
		m_Tabs->AddTab(m_TabSquare, CAssetsEditor::ICON_COLORPICKER_SQUARE, "HSV Square Color Chooser");
		{
			gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_LAST);
			pLayout->SetHeight(250);
			m_TabSquare->Add(pLayout);
			
			pLayout->Add(new CSquarePicker(this));
			pLayout->Add(new CHueVSliderEdit(this));
		}
		{
			gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
			pLayout->SetHeight(m_pConfig->m_ButtonHeight);
			m_TabSquare->Add(pLayout);
			
			gui::CLabel* pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Alpha:");
			pLayout->Add(pLabel);
			pLayout->Add(new CRGBFloatEdit<3>(this));
			pLayout->Add(new CRGBIntegerEdit<3>(this));
			
			m_TabSquare->Add(new CAlphaSliderEdit(this));
		}
		
		m_TabWheel = new gui::CVListLayout(pAssetsEditor->m_pGuiConfig);
		m_Tabs->AddTab(m_TabWheel, CAssetsEditor::ICON_COLORPICKER_WHEEL, "HSV Wheel Color Chooser");
		{
			m_TabWheel->Add(new CWheelPicker(this));
		}
		{
			gui::CHListLayout* pLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_ALL);
			pLayout->SetHeight(m_pConfig->m_ButtonHeight);
			m_TabWheel->Add(pLayout);
			
			gui::CLabel* pLabel = new gui::CLabel(m_pAssetsEditor->m_pGuiConfig, "Alpha:");
			pLayout->Add(pLabel);
			pLayout->Add(new CRGBFloatEdit<3>(this));
			pLayout->Add(new CRGBIntegerEdit<3>(this));
			
			m_TabWheel->Add(new CAlphaSliderEdit(this));
		}
		
		Update();
	}
	
	void SetColor(vec4 Color)
	{
		m_pAssetsEditor->AssetsManager()->SetAssetValue<vec4>(
			m_AssetPath,
			m_AssetMember,
			m_AssetSubPath,
			Color);
	}
	
	vec4 GetColor()
	{
		return m_pAssetsEditor->AssetsManager()->GetAssetValue<vec4>(
			m_AssetPath,
			m_AssetMember,
			m_AssetSubPath,
			vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
};

/* NEW ASSET ***************************************************/

class CPopup_NewAsset : public gui::CPopup
{	
protected:
	
	class CItem : public gui::CTextButton
	{
	protected:
		CPopup_NewAsset* m_pPopup;
		CAssetsEditor* m_pAssetsEditor;
		int m_AssetType;
		
	protected:
		virtual void MouseClickAction()
		{
			m_pPopup->CreateAsset(m_AssetType);
		}
		
	public:
		CItem(CPopup_NewAsset* pPopup, const char* pText, int IconId, int AssetType) :
			gui::CTextButton(pPopup->m_pAssetsEditor->m_pGuiConfig, pText, IconId),
			m_pPopup(pPopup),
			m_pAssetsEditor(pPopup->m_pAssetsEditor),
			m_AssetType(AssetType)
		{
			m_Centered = false;
			SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK);
		}
	};
	
protected:
	CAssetsEditor* m_pAssetsEditor;
	int m_Source;
	gui::CRect m_CreatorRect;
	int m_Alignment;
	
public:
	CPopup_NewAsset(CAssetsEditor* pAssetsEditor, int Source, const gui::CRect& CreatorRect, int Alignment) :
		gui::CPopup(pAssetsEditor->m_pGuiConfig, CreatorRect, 200, 400, Alignment),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source),
		m_CreatorRect(CreatorRect),
		m_Alignment(Alignment)
	{
		gui::CVListLayout* pLayout = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_DEFAULT);
		Add(pLayout);
				
		pLayout->Add(new CItem(this, "Image", CAssetsEditor::ICON_IMAGE, CAssetPath::TYPE_IMAGE));
		pLayout->Add(new CItem(this, "Sprite", CAssetsEditor::ICON_SPRITE, CAssetPath::TYPE_SPRITE));
		pLayout->AddSeparator();
		pLayout->Add(new CItem(this, "Map group", CAssetsEditor::ICON_MAPGROUP, CAssetPath::TYPE_MAPGROUP));
		pLayout->Add(new CItem(this, "Map tiles layer", CAssetsEditor::ICON_MAPLAYERTILES, CAssetPath::TYPE_MAPLAYERTILES));
		pLayout->Add(new CItem(this, "Map quads layer", CAssetsEditor::ICON_MAPLAYERQUADS, CAssetPath::TYPE_MAPLAYERQUADS));
		pLayout->AddSeparator();
		pLayout->Add(new CItem(this, "Skeleton", CAssetsEditor::ICON_SKELETON, CAssetPath::TYPE_SKELETON));
		pLayout->Add(new CItem(this, "Skeleton animation", CAssetsEditor::ICON_SKELETONANIMATION, CAssetPath::TYPE_SKELETONANIMATION));
		pLayout->Add(new CItem(this, "Skeleton skin", CAssetsEditor::ICON_SKELETONSKIN, CAssetPath::TYPE_SKELETONSKIN));
		pLayout->AddSeparator();
		pLayout->Add(new CItem(this, "Charater", CAssetsEditor::ICON_CHARACTER, CAssetPath::TYPE_CHARACTER));
		pLayout->Add(new CItem(this, "Charater part", CAssetsEditor::ICON_CHARACTERPART, CAssetPath::TYPE_CHARACTERPART));
		pLayout->Add(new CItem(this, "Weapon", CAssetsEditor::ICON_WEAPON, CAssetPath::TYPE_WEAPON));
		
		Update();
	}
	
	void CreateAsset(int AssetType)
	{
		#define ON_NEW_ASSET(TypeName, AssetName) case TypeName::TypeId:\
		{\
			TypeName* pAsset = m_pAssetsEditor->AssetsManager()->GetAssetCatalog<TypeName>()->NewAsset(&NewAssetPath, m_Source);\
			char aBuf[128];\
			str_format(aBuf, sizeof(aBuf), AssetName, NewAssetPath.GetId());\
			pAsset->SetName(aBuf);\
			m_pAssetsEditor->NewAsset(NewAssetPath);\
			break;\
		}
			
		CAssetPath NewAssetPath;
		
		switch(AssetType)
		{
			case CAssetPath::TYPE_IMAGE:
			{
				Close();
				m_pAssetsEditor->DisplayPopup(new CPopup_AddImage(
					m_pAssetsEditor, m_CreatorRect, m_Alignment
				));
				break;
			}
			//Search Tag: TAG_NEW_ASSET
			ON_NEW_ASSET(CAsset_Sprite, "sprite%d")
			ON_NEW_ASSET(CAsset_Skeleton, "skeleton%d")
			ON_NEW_ASSET(CAsset_SkeletonSkin, "skin%d")
			ON_NEW_ASSET(CAsset_SkeletonAnimation, "animation%d")
			ON_NEW_ASSET(CAsset_Character, "character%d")
			ON_NEW_ASSET(CAsset_CharacterPart, "characterpart%d")
			ON_NEW_ASSET(CAsset_Weapon, "weapon%d")
			ON_NEW_ASSET(CAsset_MapGroup, "group%d")
			ON_NEW_ASSET(CAsset_MapLayerTiles, "tilelayer%d")
			ON_NEW_ASSET(CAsset_MapLayerQuads, "quadlayer%d")
		}
		
		Close();
	}
};

/* SAVE/LOAD ASSETS ***************************************************/

class CPopup_SaveLoadAssets : public gui::CPopup
{	
protected:	
	class CSave : public gui::CTextButton
	{
	protected:
		CPopup_SaveLoadAssets* m_pPopup;
		
	public:
		CSave(CPopup_SaveLoadAssets* pPopup) :
			gui::CTextButton(pPopup->m_pConfig, "Save"),
			m_pPopup(pPopup)
		{ SetWidth(120); }

		virtual void MouseClickAction() { m_pPopup->Save(); }
	};
	
	class CItem_Load : public gui::CTextButton
	{
	protected:
		CPopup_SaveLoadAssets* m_pPopup;
		CAssetsEditor* m_pAssetsEditor;
		char m_aFilename[256];
		int m_StorageType;
		int m_IsDirectory;
		
	protected:
		virtual void MouseClickAction()
		{
			if(!m_IsDirectory)
			{
				m_pAssetsEditor->LoadAssetsFile(m_aFilename, m_pPopup->m_Source);
				m_pPopup->Close();
			}
		}
		
	public:
		CItem_Load(CPopup_SaveLoadAssets* pPopup, const char* pFilename, int StorageType, int IsDir) :
			gui::CTextButton(pPopup->m_pAssetsEditor->m_pGuiConfig, pFilename, CAssetsEditor::ICON_ASSET),
			m_pPopup(pPopup),
			m_pAssetsEditor(pPopup->m_pAssetsEditor)
		{
			m_Centered = false;
			SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK);
			
			str_copy(m_aFilename, pFilename, sizeof(m_aFilename));
			m_StorageType = StorageType;
			m_IsDirectory = IsDir;
		}
	};
	
	class CItem_Save : public gui::CTextButton
	{
	protected:
		CPopup_SaveLoadAssets* m_pPopup;
		CAssetsEditor* m_pAssetsEditor;
		char m_aFilename[256];
		int m_StorageType;
		int m_IsDirectory;
		
	protected:
		virtual void MouseClickAction()
		{
			if(!m_IsDirectory)
			{
				m_pPopup->SetFilename(m_aFilename);
			}
		}
		
	public:
		CItem_Save(CPopup_SaveLoadAssets* pPopup, const char* pFilename, int StorageType, int IsDir) :
			gui::CTextButton(pPopup->m_pAssetsEditor->m_pGuiConfig, pFilename, CAssetsEditor::ICON_ASSET),
			m_pPopup(pPopup),
			m_pAssetsEditor(pPopup->m_pAssetsEditor)
		{
			m_Centered = false;
			SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK);
			
			str_copy(m_aFilename, pFilename, sizeof(m_aFilename));
			m_StorageType = StorageType;
			m_IsDirectory = IsDir;
		}
	};

public:
	enum
	{
		MODE_SAVE=0,
		MODE_LOAD,
	};
	
protected:
	CAssetsEditor* m_pAssetsEditor;
	char m_aFilename[256];
	int m_Source;
	int m_Mode;
	gui::CVListLayout* m_pFilelist;
	
public:
	CPopup_SaveLoadAssets(CAssetsEditor* pAssetsEditor, int Source, int Mode, const gui::CRect& CreatorRect, int Alignment) :
		gui::CPopup(pAssetsEditor->m_pGuiConfig, CreatorRect, 500, 500, Alignment),
		m_pAssetsEditor(pAssetsEditor),
		m_Source(Source),
		m_Mode(Mode),
		m_pFilelist(0)
	{
		gui::CVListLayout* pLayout = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_DEFAULT, gui::LAYOUTFILLING_LAST);
		Add(pLayout);
		
		if(m_Mode == MODE_SAVE)
		{
			switch(m_Source)
			{
				case CAssetPath::SRC_UNIVERSE:
					str_copy(m_aFilename, "myuniverse.assets", sizeof(m_aFilename));
					break;
				case CAssetPath::SRC_WORLD:
					str_copy(m_aFilename, "myworld.assets", sizeof(m_aFilename));
					break;
				case CAssetPath::SRC_LAND:
					str_copy(m_aFilename, "myland.assets", sizeof(m_aFilename));
					break;
				case CAssetPath::SRC_SKIN:
					str_copy(m_aFilename, "myskin.assets", sizeof(m_aFilename));
					break;
			}
		
			{
				gui::CHListLayout* pHLayout = new gui::CHListLayout(m_pAssetsEditor->m_pGuiConfig, gui::CConfig::LAYOUTSTYLE_NONE, gui::LAYOUTFILLING_FIRST);
				pHLayout->SetHeight(m_pConfig->m_ButtonHeight);
				pLayout->Add(pHLayout);
				
				gui::CExternalTextEdit* pTextEdit = new gui::CExternalTextEdit(m_pAssetsEditor->m_pGuiConfig, m_aFilename, sizeof(m_aFilename));
				pHLayout->Add(pTextEdit);
				
				pHLayout->Add(new CSave(this));
			}
		}
		
		m_pFilelist = new gui::CVListLayout(m_pAssetsEditor->m_pGuiConfig);
		pLayout->Add(m_pFilelist);
		
		Update();
	}
	
	static int FileListFetchCallback_Universe(const char *pName, int IsDir, int StorageType, void *pUser)
	{
		CPopup_SaveLoadAssets* pPopup = static_cast<CPopup_SaveLoadAssets*>(pUser);
		
		int Length = str_length(pName);
		if(pName[0] == '.' && (pName[1] == 0))
			return 0;
		
		if(Length < 7 || str_comp(pName+Length-7, ".assets"))
			return 0;

		char aBuf[256];
		str_format(aBuf, sizeof(aBuf), "assets/universes/%s", pName);
		
		if(pPopup->m_Mode == MODE_SAVE)
			pPopup->m_pFilelist->Add(new CItem_Save(pPopup, aBuf, StorageType, IsDir));
		else
			pPopup->m_pFilelist->Add(new CItem_Load(pPopup, aBuf, StorageType, IsDir));
		
		return 0;
	}
	
	static int FileListFetchCallback_World(const char *pName, int IsDir, int StorageType, void *pUser)
	{
		CPopup_SaveLoadAssets* pPopup = static_cast<CPopup_SaveLoadAssets*>(pUser);
		
		int Length = str_length(pName);
		if(pName[0] == '.' && (pName[1] == 0))
			return 0;
		
		if(Length < 7 || str_comp(pName+Length-7, ".assets"))
			return 0;

		char aBuf[256];
		str_format(aBuf, sizeof(aBuf), "assets/worlds/%s", pName);
		
		if(pPopup->m_Mode == MODE_SAVE)
			pPopup->m_pFilelist->Add(new CItem_Save(pPopup, aBuf, StorageType, IsDir));
		else
			pPopup->m_pFilelist->Add(new CItem_Load(pPopup, aBuf, StorageType, IsDir));

		return 0;
	}
	
	static int FileListFetchCallback_Land(const char *pName, int IsDir, int StorageType, void *pUser)
	{
		CPopup_SaveLoadAssets* pPopup = static_cast<CPopup_SaveLoadAssets*>(pUser);
				
		int Length = str_length(pName);
		if(pName[0] == '.' && (pName[1] == 0))
			return 0;
		
		if(Length < 4 || str_comp(pName+Length-4, ".map"))
			return 0;

		char aBuf[256];
		str_format(aBuf, sizeof(aBuf), "maps/%s", pName);
		
		if(pPopup->m_Mode == MODE_SAVE)
			pPopup->m_pFilelist->Add(new CItem_Save(pPopup, aBuf, StorageType, IsDir));
		else
			pPopup->m_pFilelist->Add(new CItem_Load(pPopup, aBuf, StorageType, IsDir));

		return 0;
	}
	
	virtual void Update()
	{
		switch(m_Source)
		{
			case CAssetPath::SRC_UNIVERSE:
				m_pAssetsEditor->Storage()->ListDirectory(IStorage::TYPE_ALL, "assets/universes", FileListFetchCallback_Universe, this);
				break;
			case CAssetPath::SRC_WORLD:
				m_pAssetsEditor->Storage()->ListDirectory(IStorage::TYPE_ALL, "assets/worlds", FileListFetchCallback_World, this);
				break;
			case CAssetPath::SRC_LAND:
				m_pAssetsEditor->Storage()->ListDirectory(IStorage::TYPE_ALL, "maps", FileListFetchCallback_Land, this);
				break;
		}
		
		gui::CPopup::Update();
	}
	
	void SetFilename(const char* pFilename)
	{
		str_copy(m_aFilename, pFilename, sizeof(m_aFilename));
	}
	
	void Save()
	{
		char aBuf[256];
		switch(m_Source)
		{
			case CAssetPath::SRC_UNIVERSE:
				str_format(aBuf, sizeof(aBuf), "assets/universes/%s", m_aFilename);
				break;
			case CAssetPath::SRC_WORLD:
				str_format(aBuf, sizeof(aBuf), "assets/worlds/%s", m_aFilename);
				break;
			case CAssetPath::SRC_LAND:
				str_format(aBuf, sizeof(aBuf), "maps/%s", m_aFilename);
				break;
		}
		
		m_pAssetsEditor->AssetsManager()->SaveInAssetsFile(aBuf, m_Source);
		Close();
	}
};

}

}

#endif
