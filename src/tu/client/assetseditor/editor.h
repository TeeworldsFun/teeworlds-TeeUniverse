#ifndef TU_ASSETSEDITOR_EDITOR_H
#define TU_ASSETSEDITOR_EDITOR_H

#include <base/vmath.h>
#include <engine/kernel.h>
#include <game/client/render.h>
#include <tu/client/gui/button.h>
#include <tu/client/gui/layout.h>
#include <tu/client/gui/integer-edit.h>
#include <tu/client/gui/float-edit.h>

#include "popup.h"
#include "assetseditor.h"

namespace tu
{

namespace assetseditor
{

class CEditor : public gui::CTabs
{	
public:
	class CAssetNameEdit : public gui::CAbstractTextEdit
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_AssetPath;
		
		virtual char* GetTextPtr()
		{
			return m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(m_AssetPath, CAsset::NAME, -1, 0);
		}
		
	public:
		CAssetNameEdit(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath) :
			gui::CAbstractTextEdit(pAssetsEditor->m_pGuiConfig, sizeof(CAsset::m_aName), gui::TEXTSTYLE_NORMAL),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(AssetPath)
		{
			
		}
	};
			
	class CTextAssetMemberEdit : public gui::CAbstractTextEdit
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_AssetPath;
		int m_Member;
		int m_SubId;
		
		virtual char* GetTextPtr()
		{
			return m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(m_AssetPath, m_Member, m_SubId, 0);
		}
		
	public:
		CTextAssetMemberEdit(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Member, int SubId, int TextSize) :
			gui::CAbstractTextEdit(pAssetsEditor->m_pGuiConfig, TextSize, gui::TEXTSTYLE_NORMAL),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(AssetPath),
			m_Member(Member),
			m_SubId(SubId)
		{
			
		}
	};
			
	class CFloatAssetMemberEdit : public gui::CAbstractFloatEdit
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_AssetPath;
		int m_Member;
		int m_SubId;
		
		virtual void SetValue(float v)
		{
			m_pAssetsEditor->AssetsManager()->SetAssetValue<float>(m_AssetPath, m_Member, m_SubId, v);
		}
		
		virtual float GetValue()
		{
			return m_pAssetsEditor->AssetsManager()->GetAssetValue<float>(m_AssetPath, m_Member, m_SubId, -2);
		}
		
	public:
		CFloatAssetMemberEdit(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Member, int SubId = -1) :
			gui::CAbstractFloatEdit(pAssetsEditor->m_pGuiConfig),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(AssetPath),
			m_Member(Member),
			m_SubId(SubId)
		{
			
		}
	};
			
	class CAnglerEdit : public gui::CAbstractFloatEdit
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_AssetPath;
		int m_Member;
		int m_SubId;
		
		virtual void SetValue(float v)
		{
			m_pAssetsEditor->AssetsManager()->SetAssetValue<float>(m_AssetPath, m_Member, m_SubId, pi*v/180.0f);
		}
		
		virtual float GetValue()
		{
			return 180.0f*m_pAssetsEditor->AssetsManager()->GetAssetValue<float>(m_AssetPath, m_Member, m_SubId, -2)/pi;
		}
		
	public:
		CAnglerEdit(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Member, int SubId = -1) :
			gui::CAbstractFloatEdit(pAssetsEditor->m_pGuiConfig),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(AssetPath),
			m_Member(Member),
			m_SubId(SubId)
		{
			
		}
	};

	class CIntegerAssetMemberEdit : public gui::CAbstractIntegerEdit
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_AssetPath;
		int m_Member;
		int m_SubId;
		
		virtual void SetValue(int v)
		{
			m_pAssetsEditor->AssetsManager()->SetAssetValue<int>(m_AssetPath, m_Member, m_SubId, v);
		}
		
		virtual int GetValue()
		{
			return m_pAssetsEditor->AssetsManager()->GetAssetValue<int>(m_AssetPath, m_Member, m_SubId, -2);
		}
		
	public:
		CIntegerAssetMemberEdit(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Member, int SubId = -1) :
			gui::CAbstractIntegerEdit(pAssetsEditor->m_pGuiConfig),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(AssetPath),
			m_Member(Member),
			m_SubId(SubId)
		{
			
		}
	};

	class CAssetEdit : public gui::CExternalTextButton
	{
	public:
		static const char* m_aNoneText;
		
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_ParentAssetPath;
		int m_ParentAssetMember;
		int m_ParentAssetSubId;
		int m_FieldAssetType;
		CAssetPath m_FieldAssetPath;
		
	protected:
		virtual void MouseClickAction()
		{
			m_pAssetsEditor->DisplayPopup(new CPopup_AssetEdit(
				m_pAssetsEditor, m_Rect, gui::CPopup::ALIGNMENT_LEFT,
				m_ParentAssetPath, m_ParentAssetMember, m_ParentAssetSubId, m_FieldAssetType
			));
		}
		
	public:
		CAssetEdit(
			CAssetsEditor* pAssetsEditor,
			CAssetPath ParentAssetPath,
			int ParentAssetMember,
			int ParentAssetSubId,
			int FieldAssetType
		) :
			gui::CExternalTextButton(pAssetsEditor->m_pGuiConfig, 0),
			m_ParentAssetPath(ParentAssetPath),
			m_ParentAssetMember(ParentAssetMember),
			m_ParentAssetSubId(ParentAssetSubId),
			m_FieldAssetType(FieldAssetType),
			m_pAssetsEditor(pAssetsEditor)
		{
			m_Centered = false;
			
			Update();
		}
		
		virtual void Update()
		{
			CAssetPath Path = m_pAssetsEditor->AssetsManager()->GetAssetValue<CAssetPath>(m_ParentAssetPath, m_ParentAssetMember, m_ParentAssetSubId, CAssetPath::Null());
			if(Path.IsNull())
			{
				m_pText = m_aNoneText;
			}
			else
			{
				m_pText = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(Path, CAsset::NAME, -1, 0);
			}
			
			int IconId = -1;
			switch(Path.GetType())
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
			}
			SetIcon(IconId);
			
			gui::CExternalTextButton::Update();
		}
	};

	class CBoneEdit : public gui::CExternalTextButton
	{
	public:
		static const char* m_aNoneText;
		
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_ParentAssetPath;
		int m_ParentAssetMember;
		int m_ParentAssetSubId;
		CAssetPath m_SkeletonPath;
		
	protected:
		virtual void MouseClickAction()
		{
			m_pAssetsEditor->DisplayPopup(new CPopup_BoneEdit(
				m_pAssetsEditor, m_Rect, gui::CPopup::ALIGNMENT_LEFT,
				m_ParentAssetPath, m_ParentAssetMember, m_ParentAssetSubId, m_SkeletonPath
			));
		}
		
	public:
		CBoneEdit(
			CAssetsEditor* pAssetsEditor,
			CAssetPath ParentAssetPath,
			int ParentAssetMember,
			int ParentAssetSubId,
			CAssetPath SkeletonPath
		) :
			gui::CExternalTextButton(pAssetsEditor->m_pGuiConfig, 0),
			m_ParentAssetPath(ParentAssetPath),
			m_ParentAssetMember(ParentAssetMember),
			m_ParentAssetSubId(ParentAssetSubId),
			m_SkeletonPath(SkeletonPath),
			m_pAssetsEditor(pAssetsEditor)
		{
			m_Centered = false;
			
			Update();
		}
		
		virtual void Update()
		{
			CAsset_Skeleton::CBonePath BonePath = m_pAssetsEditor->AssetsManager()->GetAssetValue<CAsset_Skeleton::CBonePath>(m_ParentAssetPath, m_ParentAssetMember, m_ParentAssetSubId, CAsset_Skeleton::CBonePath::Null());
			if(BonePath.IsNull())
			{
				m_pText = m_aNoneText;
			}
			else if(BonePath.GetSource() == CAsset_Skeleton::CBonePath::SRC_PARENT)
			{
				CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(m_SkeletonPath);
				if(pSkeleton)
				{
					m_pText = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
						pSkeleton->m_ParentPath,
						CAsset_Skeleton::BONE_NAME,
						CAsset_Skeleton::CSubPath::Bone(BonePath.GetId()).ConvertToInteger(),
						0);
					SetIcon(CAssetsEditor::ICON_BONE);
				}
			}
			else
			{
				m_pText = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
					m_SkeletonPath,
					CAsset_Skeleton::BONE_NAME,
					CAsset_Skeleton::CSubPath::Bone(BonePath.GetId()).ConvertToInteger(),
					0);
				SetIcon(CAssetsEditor::ICON_BONE);
			}
			
			gui::CExternalTextButton::Update();
		}
	};

	class CLayerEdit : public gui::CExternalTextButton
	{
	public:
		static const char* m_aNoneText;
		
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_ParentAssetPath;
		int m_ParentAssetMember;
		int m_ParentAssetSubId;
		CAssetPath m_SkeletonPath;
		
	protected:
		virtual void MouseClickAction()
		{
			m_pAssetsEditor->DisplayPopup(new CPopup_LayerEdit(
				m_pAssetsEditor, m_Rect, gui::CPopup::ALIGNMENT_LEFT,
				m_ParentAssetPath, m_ParentAssetMember, m_ParentAssetSubId, m_SkeletonPath
			));
		}
		
	public:
		CLayerEdit(
			CAssetsEditor* pAssetsEditor,
			CAssetPath ParentAssetPath,
			int ParentAssetMember,
			int ParentAssetSubId,
			CAssetPath SkeletonPath
		) :
			gui::CExternalTextButton(pAssetsEditor->m_pGuiConfig, 0),
			m_ParentAssetPath(ParentAssetPath),
			m_ParentAssetMember(ParentAssetMember),
			m_ParentAssetSubId(ParentAssetSubId),
			m_SkeletonPath(SkeletonPath),
			m_pAssetsEditor(pAssetsEditor)
		{
			m_Centered = false;
			
			Update();
		}
		
		virtual void Update()
		{
			CAsset_Skeleton::CBonePath LayerPath = m_pAssetsEditor->AssetsManager()->GetAssetValue<CAsset_Skeleton::CBonePath>(m_ParentAssetPath, m_ParentAssetMember, m_ParentAssetSubId, CAsset_Skeleton::CBonePath::Null());
			if(LayerPath.IsNull())
			{
				m_pText = m_aNoneText;
			}
			else if(LayerPath.GetSource() == CAsset_Skeleton::CBonePath::SRC_PARENT)
			{
				CAsset_Skeleton* pSkeleton = m_pAssetsEditor->AssetsManager()->GetAsset<CAsset_Skeleton>(m_SkeletonPath);
				if(pSkeleton)
				{
					m_pText = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
						pSkeleton->m_ParentPath,
						CAsset_Skeleton::LAYER_NAME,
						CAsset_Skeleton::CSubPath::Layer(LayerPath.GetId()).ConvertToInteger(),
						0);
					SetIcon(CAssetsEditor::ICON_LAYERS);
				}
			}
			else
			{
				m_pText = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
					m_SkeletonPath,
					CAsset_Skeleton::LAYER_NAME,
					CAsset_Skeleton::CSubPath::Layer(LayerPath.GetId()).ConvertToInteger(),
					0);
				SetIcon(CAssetsEditor::ICON_LAYERS);
			}
			
			gui::CExternalTextButton::Update();
		}
	};

	class CCharacterPartEdit : public gui::CExternalTextButton
	{
	public:
		static const char* m_aNoneText;
		
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_ParentAssetPath;
		int m_ParentAssetMember;
		int m_ParentAssetSubId;
		CAssetPath m_CharacterPath;
		
	protected:
		virtual void MouseClickAction()
		{
			m_pAssetsEditor->DisplayPopup(new CPopup_CharacterPartEdit(
				m_pAssetsEditor, m_Rect, gui::CPopup::ALIGNMENT_LEFT,
				m_ParentAssetPath, m_ParentAssetMember, m_ParentAssetSubId, m_CharacterPath
			));
		}
		
	public:
		CCharacterPartEdit(
			CAssetsEditor* pAssetsEditor,
			CAssetPath ParentAssetPath,
			int ParentAssetMember,
			int ParentAssetSubId,
			CAssetPath CharacterPath
		) :
			gui::CExternalTextButton(pAssetsEditor->m_pGuiConfig, 0),
			m_ParentAssetPath(ParentAssetPath),
			m_ParentAssetMember(ParentAssetMember),
			m_ParentAssetSubId(ParentAssetSubId),
			m_CharacterPath(CharacterPath),
			m_pAssetsEditor(pAssetsEditor)
		{
			m_Centered = false;
			
			Update();
		}
		
		virtual void Update()
		{
			CAsset_Character::CSubPath SubPath = m_pAssetsEditor->AssetsManager()->GetAssetValue<int>(
				m_ParentAssetPath,
				m_ParentAssetMember,
				m_ParentAssetSubId,
				CAsset_Character::CSubPath::Null().ConvertToInteger()
			);
			
			if(SubPath.IsNull())
			{
				m_pText = m_aNoneText;
			}
			else
			{
				m_pText = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(
					m_CharacterPath,
					CAsset_Character::PART_NAME,
					CAsset_Character::CSubPath::Part(SubPath.GetId()).ConvertToInteger(),
					0);
				SetIcon(CAssetsEditor::ICON_CHARACTERPART);
			}
			
			gui::CExternalTextButton::Update();
		}
	};

	class CColorEdit : public gui::CWidget
	{		
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_AssetPath;
		int m_AssetMember;
		int m_AssetSubId;
		
		bool m_Clicked;
		bool m_UnderMouse;
		
	public:
		CColorEdit(
			CAssetsEditor* pAssetsEditor,
			CAssetPath AssetPath,
			int AssetMember,
			int AssetSubId
		) :
			gui::CWidget(pAssetsEditor->m_pGuiConfig),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(AssetPath),
			m_AssetMember(AssetMember),
			m_AssetSubId(AssetSubId),
			m_Clicked(false),
			m_UnderMouse(false)
		{
			m_Rect.w = m_pConfig->m_ButtonHeight;
			m_Rect.h = m_pConfig->m_ButtonHeight;
		}
		
		virtual void Render()
		{
			{
				CUIRect rect;
				rect.x = m_Rect.x;
				rect.y = m_Rect.y;
				rect.w = m_Rect.w;
				rect.h = m_Rect.h;
				
				RenderTools()->DrawRoundRect(&rect, vec4(1.0f, 1.0f, 1.0f, 0.25f), s_ButtonCornerRadius);
			}
			{
				vec4 Color = m_pAssetsEditor->AssetsManager()->GetAssetValue<vec4>(
					m_AssetPath,
					m_AssetMember,
					m_AssetSubId,
					vec4(1.0f, 1.0f, 1.0f, 1.0f));
					
				CUIRect rect;
				rect.x = m_Rect.x+1;
				rect.y = m_Rect.y+1;
				rect.w = (m_Rect.w-2)/2;
				rect.h = m_Rect.h-2;
				
				vec4 SquareColor = Color;
				SquareColor.a = 1.0f;
				RenderTools()->DrawRoundRect(&rect, SquareColor, s_ButtonCornerRadius-1);
			}
			{
				vec4 Color = m_pAssetsEditor->AssetsManager()->GetAssetValue<vec4>(
					m_AssetPath,
					m_AssetMember,
					m_AssetSubId,
					vec4(1.0f, 1.0f, 1.0f, 1.0f));
				
				int alphaSquareX = m_Rect.x+2+(m_Rect.w-2)/2;
				int alphaSquareW = (m_Rect.x + m_Rect.w - 1) - alphaSquareX;
				float SquareHeight = (m_Rect.h-2)/3.0f;
				int nbColumns = alphaSquareW / SquareHeight;
				float SquareWidth = alphaSquareW/nbColumns;
				
				Graphics()->TextureClear();
				Graphics()->QuadsBegin();
				for(int j=0; j<3; j++)
				{
					for(int i=0; i<nbColumns; i++)
					{
						if(i%2==j%2)
							Graphics()->SetColor(0.4f, 0.4f, 0.4f, 1.0f);
						else
							Graphics()->SetColor(0.6f, 0.6f, 0.6f, 1.0f);
						IGraphics::CQuadItem QuadItem(alphaSquareX + i*SquareWidth, m_Rect.y+1 + j*SquareHeight, SquareWidth, SquareHeight);
						Graphics()->QuadsDrawTL(&QuadItem, 1);
					}
				}
				Graphics()->QuadsEnd();
				
				CUIRect rect;
				rect.x = alphaSquareX;
				rect.y = m_Rect.y+1;
				rect.w = alphaSquareW;
				rect.h = m_Rect.h-2;
				
				vec4 SquareColor = vec4(Color.r*Color.a, Color.g*Color.a, Color.b*Color.a, Color.a);
				RenderTools()->DrawRoundRect(&rect, SquareColor, s_ButtonCornerRadius-1);
			}
		}

		void OnMouseOver(int X, int Y, int RelX, int RelY, int KeyState)
		{
			if(m_Rect.IsInside(X, Y))
			{
				m_UnderMouse = true;
			}
			else
			{
				m_UnderMouse = false;
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
			
			if(m_UnderMouse && m_Clicked)
			{
				m_Clicked = false;
				m_pAssetsEditor->DisplayPopup(new CPopup_ColorEdit(
					m_pAssetsEditor, m_Rect, gui::CPopup::ALIGNMENT_LEFT,
					m_AssetPath, m_AssetMember, m_AssetSubId
				));
			}
		}
	};

	class CEnumEdit : public gui::CTextButton
	{
	protected:
		CAssetPath m_AssetPath;
		int m_Member;
		int m_SubId;
		CAssetsEditor* m_pAssetsEditor;
		int m_NbElements;
		const char** m_aTexts;
		
	protected:
		virtual void MouseClickAction()
		{
			int OldValue = m_pAssetsEditor->AssetsManager()->GetAssetValue<int>(m_AssetPath, m_Member, m_SubId, 0);
			int NewValue = (OldValue + 1)%m_NbElements;
			m_pAssetsEditor->AssetsManager()->SetAssetValue<int>(m_AssetPath, m_Member, m_SubId, NewValue);
			UpdateButtonLabel(NewValue);
		}
		
		void UpdateButtonLabel(int Value)
		{
			SetText(m_aTexts[Value%m_NbElements]);
		}
		
	public:
		CEnumEdit(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int Member, int SubId, const char** aTexts, int NbElements) :
			gui::CTextButton(pAssetsEditor->m_pGuiConfig, "", -1),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(AssetPath),
			m_Member(Member),
			m_SubId(SubId),
			m_aTexts(aTexts),
			m_NbElements(NbElements)
		{
			
		}
		
		virtual void Update()
		{
			int Value = m_pAssetsEditor->AssetsManager()->GetAssetValue<int>(m_AssetPath, m_Member, m_SubId, 0);
			UpdateButtonLabel(Value);
			
			gui::CTextButton::Update();
		}
	};

	class CDeleteAsset : public gui::CTextButton
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_AssetPath;
		
	protected:
		virtual void MouseClickAction()
		{
			m_pAssetsEditor->DeleteAsset(m_AssetPath);
		}

	public:
		CDeleteAsset(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath) :
			gui::CTextButton(pAssetsEditor->m_pGuiConfig, "Delete", CAssetsEditor::ICON_DELETE),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(AssetPath)
		{
			
		}
	};

	class CDeleteSubItem : public gui::CIconButton
	{
	protected:
		CAssetPath m_AssetPath;
		int m_SubId;
		CAssetsEditor* m_pAssetsEditor;

	public:
		CDeleteSubItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int SubId) :
			gui::CIconButton(pAssetsEditor->m_pGuiConfig, CAssetsEditor::ICON_DELETE),
			m_AssetPath(AssetPath),
			m_SubId(SubId),
			m_pAssetsEditor(pAssetsEditor)
		{
			
		}
	};

	class CMoveDownSubItem : public gui::CIconButton
	{
	protected:
		CAssetPath m_AssetPath;
		int m_SubId;
		CAssetsEditor* m_pAssetsEditor;

	public:
		CMoveDownSubItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int SubId) :
			gui::CIconButton(pAssetsEditor->m_pGuiConfig, CAssetsEditor::ICON_UP),
			m_AssetPath(AssetPath),
			m_SubId(SubId),
			m_pAssetsEditor(pAssetsEditor)
		{
			
		}
	};

	class CMoveUpSubItem : public gui::CIconButton
	{
	protected:
		CAssetPath m_AssetPath;
		int m_SubId;
		CAssetsEditor* m_pAssetsEditor;

	public:
		CMoveUpSubItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int SubId) :
			gui::CIconButton(pAssetsEditor->m_pGuiConfig, CAssetsEditor::ICON_DOWN),
			m_AssetPath(AssetPath),
			m_SubId(SubId),
			m_pAssetsEditor(pAssetsEditor)
		{
			
		}
	};

	class CSubItemListItem : public gui::CExternalTextButton
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		
		CAssetPath m_AssetPath;
		int m_AssetSubPath;
		char m_aText[128];
		
	protected:
		virtual void MouseClickAction()
		{
			m_pAssetsEditor->EditAssetSubItem(m_AssetPath, m_AssetSubPath);
		}
	
	public:
		CSubItemListItem(CAssetsEditor* pAssetsEditor, int Icon) :
			gui::CExternalTextButton(pAssetsEditor->m_pGuiConfig, 0, Icon),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(CAssetPath::Null()),
			m_AssetSubPath(-1)
		{
			m_Centered = false;
			SetButtonStyle(gui::CConfig::BUTTONSTYLE_LINK);
		}
		
		void SetTarget(CAssetPath AssetPath, int AssetSubPath)
		{
			m_AssetPath = AssetPath;
			m_AssetSubPath = AssetSubPath;
		}
		
		void SetText(CAssetPath AssetPath, int AssetSubType, int AssetSubPath)
		{
			m_pText = m_pAssetsEditor->AssetsManager()->GetAssetValue<char*>(AssetPath, AssetSubType, AssetSubPath, 0);
		}
		
		void SetText(const char* pText)
		{
			str_copy(m_aText, pText, sizeof(m_aText));
			m_pText = m_aText;
		}
	};

	class CAddSubItem : public gui::CTextButton
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		
		CAssetPath m_AssetPath;
		int m_SubItemType;
		int m_TabId;
		
	protected:
		virtual void MouseClickAction()
		{
			int SubPath = m_pAssetsEditor->AssetsManager()->AddSubItem(m_AssetPath, m_SubItemType);
			m_pAssetsEditor->EditAssetSubItem(m_AssetPath, SubPath, m_TabId);
		}
	
	public:
		CAddSubItem(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int SubType, const char* pText, int IconId, int Tab=-1) :
			gui::CTextButton(pAssetsEditor->m_pGuiConfig, pText, IconId),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(AssetPath),
			m_SubItemType(SubType),
			m_TabId(Tab)
		{
			m_Centered = false;
		}
	};
	
	class CDuplicateFrameButton : public gui::CTextButton
	{
	protected:
		CAssetsEditor* m_pAssetsEditor;
		CAssetPath m_AssetPath;
		int m_FrameId;
		
	public:
		CDuplicateFrameButton(CAssetsEditor* pAssetsEditor, CAssetPath AssetPath, int FrameId) :
			gui::CTextButton(pAssetsEditor->m_pGuiConfig, "Duplicate", CAssetsEditor::ICON_DUPLICATE),
			m_pAssetsEditor(pAssetsEditor),
			m_AssetPath(AssetPath),
			m_FrameId(FrameId)
		{
			
		}
	};
	
public:
	//Search Tag: TAG_NEW_ASSET
	enum
	{
		TAB_ASSET=0,
		
		TAB_SKELETON_BONES=1,
		TAB_SKELETON_LAYERS,
		
		TAB_SKELETONSKIN_SPRITES=1,
		
		TAB_SKELETONANIMATION_ANIMATIONS=1,
		TAB_SKELETONANIMATION_KEYFRAMES,
		
		TAB_CHARACTER_PARTS=1,
		
		TAB_MAPGROUP_LAYERS=1,
		
		TAB_MAPLAYERQUADS_QUADS=1,
		
		NUM_TABS=3
	};
	enum
	{
		LIST_SKELETON_BONES=0,
		LIST_SKELETON_LAYERS,
		
		LIST_SKELETONSKIN_SPRITES=0,
		
		LIST_SKELETONANIMATION_ANIMATIONS=0,
		LIST_SKELETONANIMATION_KEYFRAMES,
		
		LIST_CHARACTER_PARTS=0,
		
		LIST_MAPGROUP_LAYERS=0,
		
		LIST_MAPLAYERQUADS_QUADS=0,
		
		NUM_LISTS=2
	};
	
protected:
	CAssetsEditor* m_pAssetsEditor;
	
	int m_LastEditedAssetType;
	gui::CVListLayout* m_pTabs[NUM_TABS];
	gui::CVListLayout* m_pLists[NUM_LISTS];

protected:
	void AddAssetTabCommons(gui::CVListLayout* pList);
	void AddSubTitle(gui::CVListLayout* pList, const char* pText);
	
	void AddField(gui::CVListLayout* pList, gui::CWidget* pWidget, const char* pLabelText);
	
	void AddTextField(gui::CVListLayout* pList, int Member, int SubId, int TextSize, const char* pLabelText = 0);
	void AddFloatField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText = 0);
	void AddFloat2Field(gui::CVListLayout* pList, int Member1, int Member2, int SubId, const char* pLabelText = 0);
	void AddAngleField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText = 0);
	void AddIntegerField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText = 0);
	void AddAssetField(gui::CVListLayout* pList, int Member, int AssetType, int SubId, const char* pLabelText = 0);
	void AddBoneField(gui::CVListLayout* pList, int Member, int SubId, CAssetPath SkeletonPath, const char* pLabelText = 0);
	void AddLayerField(gui::CVListLayout* pList, int Member, int SubId, CAssetPath SkeletonPath, const char* pLabelText = 0);
	void AddCharacterPartField(gui::CVListLayout* pList, int Member, int SubId, CAssetPath CharacterPath, const char* pLabelText = 0);
	void AddCycleField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText = 0);
	void AddSpriteAlignField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText = 0);
	void AddLayerStateField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText = 0);
	void AddBoneAlignField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText = 0);
	void AddColorField(gui::CVListLayout* pList, int Member, int SubId, const char* pLabelText = 0);
	
	//Search Tag: TAG_NEW_ASSET
	void RefreshTab_Image_Asset(bool KeepStatus);
	void RefreshTab_Sprite_Asset(bool KeepStatus);
	void RefreshTab_Skeleton_Asset(bool KeepStatus);
	void RefreshTab_Skeleton_Bones(bool KeepStatus);
	void RefreshTab_Skeleton_Layers(bool KeepStatus);
	void RefreshTab_SkeletonSkin_Asset(bool KeepStatus);
	void RefreshTab_SkeletonSkin_Sprites(bool KeepStatus);
	void RefreshTab_SkeletonAnimation_Asset(bool KeepStatus);
	void RefreshTab_SkeletonAnimation_Animations(bool KeepStatus);
	void RefreshTab_SkeletonAnimation_KeyFrames(bool KeepStatus);
	void RefreshTab_Character_Asset(bool KeepStatus);
	void RefreshTab_Character_Parts(bool KeepStatus);
	void RefreshTab_CharacterPart_Asset(bool KeepStatus);
	void RefreshTab_Weapon_Asset(bool KeepStatus);
	void RefreshTab_MapGroup_Asset(bool KeepStatus);
	void RefreshTab_MapGroup_Layers(bool KeepStatus);
	void RefreshTab_MapLayerTiles_Asset(bool KeepStatus);
	void RefreshTab_MapLayerQuads_Asset(bool KeepStatus);
	void RefreshTab_MapLayerQuads_Quads(bool KeepStatus);
	
public:
	CEditor(CAssetsEditor* pAssetsEditor);
	void OnEditedAssetChange();
	void Refresh(int Tab=-1);
};

}

}

#endif
