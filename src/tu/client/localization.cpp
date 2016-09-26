#include <tu/client/localization.h>

#include <engine/external/json-parser/json.h>
#include <engine/shared/config.h>
#include <engine/storage.h>

namespace tu
{

/* LOCALIZATION TABLE *************************************************/

CLocalization::CTable::CTable()
{
	
}

void CLocalization::CTable::Add(const char* pKey, const char* pTranslation)
{
	int Hash = str_quickhash(pKey)%TABLESIZE;
	for(int i=0; i<m_aTable[Hash].size(); i++)
	{
		if(str_comp(m_aTable[Hash][i].m_Key.c_str(), pKey) == 0)
		{
			m_aTable[Hash][i].m_Translation = pTranslation;
			return;
		}
	}
	CEntry& Entry = m_aTable[Hash][m_aTable[Hash].add(CEntry())];
	Entry.m_Key = pKey;
	Entry.m_Translation = pTranslation;
}

const char* CLocalization::CTable::Get(const char* pKey) const
{
	int Hash = str_quickhash(pKey)%TABLESIZE;
	for(int i=0; i<m_aTable[Hash].size(); i++)
	{
		if(str_comp(m_aTable[Hash][i].m_Key.c_str(), pKey) == 0)
		{
			return m_aTable[Hash][i].m_Translation.c_str();
		}
	}
	return pKey;
}

/* LANGUAGE ***********************************************************/

CLocalization::CLanguage::CLanguage() :
	m_Loaded(false),
	m_Code(-1),
	m_Direction(CLocalization::DIRECTION_LTR)
{
	m_aName[0] = 0;
	m_aFilename[0] = 0;
}

CLocalization::CLanguage::CLanguage(const char* pName, const char* pFilename, int Code) :
	m_Loaded(false),
	m_Code(Code),
	m_Direction(CLocalization::DIRECTION_LTR)
{
	str_copy(m_aName, pName, sizeof(m_aName));
	str_copy(m_aFilename, pFilename, sizeof(m_aFilename));
}

bool CLocalization::CLanguage::Load(IStorage* pStorage)
{
	// read file data into buffer
	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "languages/%s.json", m_aFilename);
	
	IOHANDLE File = pStorage->OpenFile(aBuf, IOFLAG_READ, IStorage::TYPE_ALL);
	if(!File)
		return false;
	
	// load the file as a string
	int FileSize = (int)io_length(File);
	char *pFileData = new char[FileSize+1];
	io_read(File, pFileData, FileSize);
	pFileData[FileSize] = 0;
	io_close(File);

	// parse json data
	json_settings JsonSettings;
	mem_zero(&JsonSettings, sizeof(JsonSettings));
	char aError[256];
	json_value *pJsonData = json_parse_ex(&JsonSettings, pFileData, aError);
	if(pJsonData == 0)
	{
		dbg_msg("Localization", "Can't load the localization file %s : ", aBuf, aError);
		delete[] pFileData;
		return false;
	}

	// extract data
	const json_value &rStart = (*pJsonData)["translated strings"];
	if(rStart.type == json_array)
	{
		for(unsigned i = 0; i < rStart.u.array.length; ++i)
		{
			m_Translations.Add((const char *)rStart[i]["or"], (const char *)rStart[i]["tr"]);
		}
	}

	// clean up
	json_value_free(pJsonData);
	delete[] pFileData;
	
	m_Loaded = true;
	
	return true;
}

const char* CLocalization::CLanguage::Localize(const char* pText) const
{
	return m_Translations.Get(pText);
}

/* LOCALIZATION *******************************************************/

CLocalization::CLocalization(CKernel* pKernel) :
	CKernel::CComponent(pKernel),
	m_pMainLanguage(0)
{
	
}

CLocalization::~CLocalization()
{
	for(int i=0; i<m_pLanguages.size(); i++)
		delete m_pLanguages[i];
}

bool CLocalization::Init()
{
	// read file data into buffer
	const char *pFilename = "languages/index.json";
	IOHANDLE File = Storage()->OpenFile(pFilename, IOFLAG_READ, IStorage::TYPE_ALL);
	if(!File)
		return false;
	
	int FileSize = (int)io_length(File);
	char *pFileData = new char[FileSize+1];
	io_read(File, pFileData, FileSize);
	pFileData[FileSize] = 0;
	io_close(File);

	// parse json data
	json_settings JsonSettings;
	mem_zero(&JsonSettings, sizeof(JsonSettings));
	char aError[256];
	json_value *pJsonData = json_parse_ex(&JsonSettings, pFileData, aError);
	if(pJsonData == 0)
	{
		delete[] pFileData;
		return false;
	}

	// extract data
	m_pMainLanguage = 0;
	const json_value &rStart = (*pJsonData)["language indices"];
	if(rStart.type == json_array)
	{
		for(unsigned i = 0; i < rStart.u.array.length; ++i)
		{
			CLanguage* pLanguage = new CLanguage((const char *)rStart[i]["name"], (const char *)rStart[i]["file"], (long)rStart[i]["code"]);
			if(str_comp(g_Config.m_ClLanguage, pLanguage->GetFilename()) == 0)
			{
				pLanguage->Load(Storage());
				
				if((const char *)rStart[i]["direction"] && str_comp((const char *)rStart[i]["direction"], "rtl") == 0)
					pLanguage->SetWritingDirection(DIRECTION_RTL);
				
				m_pMainLanguage = pLanguage;
			}
			m_pLanguages.add(pLanguage);
		}
	}

	// clean up
	json_value_free(pJsonData);
	delete[] pFileData;
	
	return true;
}
	
void CLocalization::AddListener(IListener* pListener)
{
	pListener->Localization_SetToken(m_pListeners.size());
	m_pListeners.add(pListener);
}

void CLocalization::RemoveListener(int Token)
{
	for(int i=0; i<m_pListeners.size(); i++)
	{
		if(m_pListeners[i]->Localization_GetToken() == Token)
			m_pListeners.remove_index(i);
	}
}

void CLocalization::Update()
{
	if(!m_pMainLanguage || str_comp(m_pMainLanguage->GetFilename(), g_Config.m_ClLanguage) != 0)
	{
		m_pMainLanguage = 0;
		
		for(int i=0; i<m_pLanguages.size(); i++)
		{
			if(str_comp(m_pLanguages[i]->GetFilename(), g_Config.m_ClLanguage) == 0)
			{
				m_pMainLanguage = m_pLanguages[i];
				break;
			}
		}
		
		for(int i=0; i<m_pListeners.size(); i++)
			m_pListeners[i]->OnLocalizationModified();
	}
}

const char* CLocalization::Localize(const char* pText, int Code)
{	
	CLanguage* pLanguage = m_pMainLanguage;
	if(Code >= 0)
	{
		for(int i=0; i<m_pLanguages.size(); i++)
		{
			if(m_pLanguages[i]->GetCode() == Code)
			{
				pLanguage = m_pLanguages[i];
				break;
			}
		}
	}
	
	if(!pLanguage)
		return pText;
	
	if(!pLanguage->IsLoaded())
		pLanguage->Load(Storage());
	
	return pLanguage->Localize(pText);
}

}
