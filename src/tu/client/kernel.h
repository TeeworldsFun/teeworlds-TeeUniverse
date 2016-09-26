#ifndef TU_KERNEL_H
#define TU_KERNEL_H

//TAG_NEW_KERNEL_COMPONENT
class IGraphics;
class ITextRender;
class IInput;
class IStorage;

namespace tu
{
	
//TAG_NEW_KERNEL_COMPONENT
class CAssetsManager;
class CAssetsRenderer;
class CTextRenderer;
class CLocalization;

class CKernel
{
public:
	class CComponent
	{
	private:
		CKernel* m_pKernel;
		
	public:
		CComponent(CKernel* pKernel) : m_pKernel(pKernel) { }
		virtual ~CComponent() {}
		CKernel* Kernel() { return m_pKernel; }
		#define TU_MACRO_KERNELCOMPONENT(TypeName, FuncName, MemberName, IsAllocated) inline TypeName* FuncName() { return m_pKernel->FuncName(); };
		#include <tu/client/kernelmacro.h>
		#undef TU_MACRO_KERNELCOMPONENT
	};
	
	class CGuest
	{
	private:
		CKernel* m_pKernel;
		
	public:
		CGuest(CKernel* pKernel) : m_pKernel(pKernel) { }
		CKernel* Kernel() { return m_pKernel; }
		#define TU_MACRO_KERNELCOMPONENT(TypeName, FuncName, MemberName, IsAllocated) inline TypeName* FuncName() { return m_pKernel->FuncName(); };
		#include <tu/client/kernelmacro.h>
		#undef TU_MACRO_KERNELCOMPONENT
	};
	
private:
	#define TU_MACRO_KERNELCOMPONENT(TypeName, FuncName, MemberName, IsAllocated) TypeName* MemberName;
	#include <tu/client/kernelmacro.h>
	#undef TU_MACRO_KERNELCOMPONENT
	
public:
	#define TU_MACRO_KERNELCOMPONENT(TypeName, FuncName, MemberName, IsAllocated) TypeName* FuncName() { return MemberName; };
	#include <tu/client/kernelmacro.h>
	#undef TU_MACRO_KERNELCOMPONENT
	
	CKernel();
	~CKernel();
	void Init(IGraphics* pGraphics, ITextRender* pTextRender, IInput* pInput, IStorage* pStorage);
	void Update();
};

}

#endif
