#ifndef TU_CLIENT_LOCALIZATION_H
#define TU_CLIENT_LOCALIZATION_H

#include <tu/client/kernel.h>
#include <base/tl/array.h>
#include <string>

namespace tu
{

struct CLocalizableString
{
	const char* m_pText;
	
	CLocalizableString(const char* pText) :
		m_pText(pText)
	{ }
};

#define _(TEXT) (CLocalizableString(TEXT))

class CLocalization : public CKernel::CComponent
{
public:
	class IListener
	{
	protected:
		int m_LocalizationListener_Token;
	
	public:
		virtual void OnLocalizationModified() = 0;
		inline void Localization_SetToken(int Token) { m_LocalizationListener_Token = Token; };
		inline int Localization_GetToken() const { return m_LocalizationListener_Token; };
	};
	
	class CTable
	{
	public:
		enum
		{
			TABLESIZE = 128
		};
		
		class CEntry
		{
		public:
			std::string m_Key;
			std::string m_Translation;
		};
	
	protected:
		array<CEntry> m_aTable[TABLESIZE];
		
	public:
		CTable();
		void Add(const char* pKey, const char* pTranslation);
		const char* Get(const char* pKey) const;
	};
	
	class CLanguage
	{
	protected:
		char m_aName[64];
		char m_aFilename[64];
		int m_Code;
		bool m_Loaded;
		int m_Direction;
		
		CTable m_Translations;
	
	public:
		CLanguage();
		CLanguage(const char* pName, const char* pFilename, int Code);
		
		inline int GetCode() const { return m_Code; }
		inline const char* GetFilename() const { return m_aFilename; }
		inline const char* GetName() const { return m_aName; }
		inline int GetWritingDirection() const { return m_Direction; }
		inline void SetWritingDirection(int Direction) { m_Direction = Direction; }
		inline bool IsLoaded() const { return m_Loaded; }
		bool Load(IStorage* pStorage);
		const char* Localize(const char* pKey) const;
	};
	
	enum
	{
		DIRECTION_LTR=0,
		DIRECTION_RTL,
		NUM_DIRECTIONS,
	};

protected:
	CLanguage* m_pMainLanguage;
	array<IListener*> m_pListeners;
	bool m_UpdateListeners;

public:
	array<CLanguage*> m_pLanguages;
	
public:
	CLocalization(CKernel* pKernel);
	virtual ~CLocalization();
	
	virtual bool Init();
	virtual void Update();
	
	void AddListener(IListener* pListener);
	void RemoveListener(int Token);
	
	const char* Localize(const char* pText, int Code = -1);
	inline bool GetWritingDirection() const { return (!m_pMainLanguage ? DIRECTION_LTR : m_pMainLanguage->GetWritingDirection()); }
	
};

}

#endif
