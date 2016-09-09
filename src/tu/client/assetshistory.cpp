#include <tu/client/assetshistory.h>
#include <tu/client/assetsmanager.h>

namespace tu
{

void CAssetsHistory::CEntry::Reset()
{
	for(int i=0; i<m_Operations.size(); i++)
	{
		if(m_Operations[i].m_pAsset)
		{
			#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
			{\
				ClassName* pAsset = (ClassName*) m_Operations[i].m_pAsset;\
				delete pAsset;\
				break;\
			}
			switch(m_Operations[i].m_AssetPath.GetType())
			{
				#include <tu/client/assetsmacro.h>
			}
			#undef TU_MACRO_ASSETTYPE
			
			m_Operations[i].m_pAsset = 0;
		}
	}
	m_Operations.clear();
	m_Token = NO_TOKEN;
}

CAssetsHistory::CAssetsHistory(CAssetsManager* pAssetsManager) :
	m_pAssetsManager(pAssetsManager),
	m_LastEntry(0),
	m_Size(0),
	m_LastToken(0)
{
	
}

CAssetsHistory::~CAssetsHistory()
{
	for(int i=0; i<m_Size; i++)
		m_Entries[i].Reset();
}

void CAssetsHistory::NewEntry(int Token)
{
	m_LastEntry = (m_LastEntry+1)%MAXHISTORYSIZE;
	if(m_Size < MAXHISTORYSIZE)
		m_Size++;
	
	m_Entries[m_LastEntry].Reset();
	
	if(Token == NEW_TOKEN)
		Token = GenerateToken();
	
	m_Entries[m_LastEntry].m_Token = Token;
}

void CAssetsHistory::AddOperation_EditAsset(CAssetPath AssetPath, int Token)
{
	if(Token == NO_TOKEN)
		return;
	
	bool NewOperationNeeded = true;
	bool NewEntryNeeded = true;
	if(m_Size > 0 && Token != NEW_TOKEN && m_Entries[m_LastEntry].m_Token == Token)
	{
		NewEntryNeeded = false;
		for(int i=0; i<m_Entries[m_LastEntry].m_Operations.size(); i++)
		{
			if(
				m_Entries[m_LastEntry].m_Operations[i].m_AssetPath == AssetPath &&
				m_Entries[m_LastEntry].m_Operations[i].m_Operation == OPERATION_EDITASSET
			)
			{
				NewOperationNeeded = false;
				break;
			}
		}
	}
	
	if(NewOperationNeeded)
	{
		if(NewEntryNeeded)
			NewEntry(Token);
		
		#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
		{\
			ClassName* pAsset = AssetsManager()->GetAsset<ClassName>(AssetPath);\
			if(pAsset)\
			{\
				COperation Operation;\
				Operation.m_AssetPath = AssetPath;\
				Operation.m_pAsset = new ClassName(*pAsset);\
				Operation.m_Operation = OPERATION_EDITASSET;\
				m_Entries[m_LastEntry].m_Operations.add(Operation);\
				dbg_msg("TeeUniv", "History::EditAsset. Token:%d, HistorySize:%d, Pointer:%p", Token, m_Size, Operation.m_pAsset);\
			}\
			break;\
		}

		switch(AssetPath.GetType())
		{
			#include <tu/client/assetsmacro.h>
		}
		
		#undef TU_MACRO_ASSETTYPE
	}
}

void CAssetsHistory::AddOperation_AddAsset(CAssetPath AssetPath, int Token)
{
	if(Token == NO_TOKEN)
		return;
	
	bool NewOperationNeeded = true;
	bool NewEntryNeeded = true;
	if(m_Size > 0 && Token != NEW_TOKEN && m_Entries[m_LastEntry].m_Token == Token)
	{
		NewEntryNeeded = false;
		for(int i=0; i<m_Entries[m_LastEntry].m_Operations.size(); i++)
		{
			if(
				m_Entries[m_LastEntry].m_Operations[i].m_AssetPath == AssetPath &&
				m_Entries[m_LastEntry].m_Operations[i].m_Operation == OPERATION_ADDASSET
			)
			{
				NewOperationNeeded = false;
				break;
			}
		}
	}
	
	if(NewOperationNeeded)
	{
		if(NewEntryNeeded)
			NewEntry(Token);
		
		#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
		{\
			ClassName* pAsset = AssetsManager()->GetAsset<ClassName>(AssetPath);\
			if(pAsset)\
			{\
				COperation Operation;\
				Operation.m_AssetPath = AssetPath;\
				Operation.m_pAsset = 0;\
				Operation.m_Operation = OPERATION_ADDASSET;\
				m_Entries[m_LastEntry].m_Operations.add(Operation);\
			}\
			break;\
		}

		switch(AssetPath.GetType())
		{
			#include <tu/client/assetsmacro.h>
		}
		
		#undef TU_MACRO_ASSETTYPE
	}
	
	dbg_msg("TeeUniv", "History::AddAsset. Token:%d, HistorySize:%d", Token, m_Size);
}

void CAssetsHistory::Undo()
{
	if(m_Size <= 0)
		return;
	
	for(int i=m_Entries[m_LastEntry].m_Operations.size()-1; i>=0; i--)\
	{
		#define TU_MACRO_ASSETTYPE(ClassName, CatalogName, AssetTypeName, AssetDefaultName) case ClassName::TypeId:\
		{\
			if(m_Entries[m_LastEntry].m_Operations[i].m_Operation == OPERATION_EDITASSET)\
			{\
				ClassName* pOldAsset = (ClassName*) m_Entries[m_LastEntry].m_Operations[i].m_pAsset;\
				dbg_msg("TeeUniv", "History::Undo (EditOperation). HistorySize:%d, Pointer:%p", m_Size-1, pOldAsset);\
				ClassName* pNewAsset = AssetsManager()->GetAsset<ClassName>(m_Entries[m_LastEntry].m_Operations[i].m_AssetPath);\
				if(pNewAsset)\
					*pNewAsset = *pOldAsset;\
			}\
			else if(m_Entries[m_LastEntry].m_Operations[i].m_Operation == OPERATION_ADDASSET)\
			{\
				dbg_msg("TeeUniv", "History::Undo (AddOperation). HistorySize:%d", m_Size-1);\
			}\
			break;\
		}

		switch(m_Entries[m_LastEntry].m_Operations[i].m_AssetPath.GetType())
		{
			#include <tu/client/assetsmacro.h>
		}
		
		#undef TU_MACRO_ASSETTYPE
	}
	
	m_Entries[m_LastEntry].Reset();
	
	m_LastEntry = (MAXHISTORYSIZE + m_LastEntry - 1)%MAXHISTORYSIZE;
	m_Size--;
}

int CAssetsHistory::GenerateToken()
{
	m_LastToken++;
	return m_LastToken;
}

}
