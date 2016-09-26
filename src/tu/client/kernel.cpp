#include <tu/client/kernel.h>

//TAG_NEW_KERNEL_COMPONENT
#include <engine/graphics.h>
#include <engine/textrender.h>
#include <tu/client/assetsmanager.h>
#include <tu/client/assetsrenderer.h>
#include <tu/client/textrenderer.h>
#include <tu/client/localization.h>

namespace tu
{
	
CKernel::CKernel()
{
	#define TU_MACRO_KERNELCOMPONENT(TypeName, FuncName, MemberName, IsAllocated) MemberName = 0;
	#include <tu/client/kernelmacro.h>
	#undef TU_MACRO_KERNELCOMPONENT
}
	
CKernel::~CKernel()
{
	//TAG_NEW_KERNEL_COMPONENT
	delete m_pTextRenderer;
	delete m_pAssetsRenderer;
	delete m_pAssetsManager;
	delete m_pLocalization;
}
	
void CKernel::Init(IGraphics* pGraphics, ITextRender* pTextRender, IInput* pInput, IStorage* pStorage)
{
	//TAG_NEW_KERNEL_COMPONENT
	m_pGraphics = pGraphics;
	m_pTextRender = pTextRender;
	m_pInput = pInput;
	m_pStorage = pStorage;
	
	m_pLocalization = new CLocalization(this);
	m_pAssetsManager = new CAssetsManager(this);
	m_pAssetsRenderer = new CAssetsRenderer(this);
	m_pTextRenderer = new CTextRenderer(this);
	
	m_pLocalization->Init();
	m_pAssetsManager->Init();
	m_pAssetsRenderer->Init();
	m_pTextRenderer->Init();
}
	
void CKernel::Update()
{
	//TAG_NEW_KERNEL_COMPONENT
	m_pLocalization->Update();
	m_pAssetsManager->Update();
	m_pTextRenderer->Update();
}

}
