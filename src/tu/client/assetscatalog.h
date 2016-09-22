#ifndef TU_CLIENT_ASSETSCATALOG_H
#define TU_CLIENT_ASSETSCATALOG_H

#include <base/tl/array.h>
#include <engine/shared/datafile.h>
#include <tu/client/assets.h>

namespace tu
{

struct CAssetState
{
	bool m_ListedInEditor;
};

template<class ASSET, class STATE = CAssetState>
class CAssetCatalog
{
public:
	struct CCatalogEntry
	{
		ASSET m_Asset;
		STATE m_State;
	};
	
public:
	array<CCatalogEntry> m_Assets[CAssetPath::NUM_SOURCES];

public:
	void InitAssetState(int Source, const STATE& State)
	{
		for(int i=0; i<m_Assets[Source].size(); i++)
		{
			m_Assets[Source][i].m_State = State;
		}
	}
	
	const ASSET* GetAsset(const CAssetPath& path) const
	{
		if(path.GetType() != ASSET::TypeId)
			return 0;
		
		int Id = path.GetId();
		if(Id < 0)
			return 0;
			
		if(Id < m_Assets[path.GetSource()].size())
			return &m_Assets[path.GetSource()][Id].m_Asset;
		else
			return 0;
	}
	
	ASSET* GetAsset(const CAssetPath& path)
	{
		if(path.GetType() != ASSET::TypeId)
			return 0;
		
		int Id = path.GetId();
		if(Id < 0)
			return 0;
			
		if(Id < m_Assets[path.GetSource()].size())
			return &m_Assets[path.GetSource()][Id].m_Asset;
		else
			return 0;
	}
	
	STATE* GetAssetState(const CAssetPath& path)
	{
		if(path.GetType() != ASSET::TypeId)
			return 0;
		
		int Id = path.GetId();
		if(Id < 0)
			return 0;
			
		if(Id < m_Assets[path.GetSource()].size())
			return &m_Assets[path.GetSource()][Id].m_State;
		else
			return 0;
	}
	
	ASSET* NewAsset(class CAssetsManager* pAssetsManager, CAssetPath* path, int Source)
	{
		int Id = m_Assets[Source].add(CCatalogEntry());
		*path = CAssetPath::Asset(ASSET::TypeId, Source, Id);
		
		m_Assets[Source][Id].m_Asset.SetAssetsManager(pAssetsManager);
		
		return &m_Assets[Source][Id].m_Asset;
	}
	
	ASSET* NewAsset(class CAssetsManager* pAssetsManager, const CAssetPath& path)
	{
		if(path.GetType() != ASSET::TypeId)
			return 0;
		
		int Id = path.GetId();
		if(Id < 0)
			return 0;
		
		int OldSize = m_Assets[path.GetSource()].size();
		int NewSize = max(OldSize, Id+1);
		m_Assets[path.GetSource()].set_size(NewSize);
		
		for(int i=OldSize; i<NewSize; i++)
			m_Assets[path.GetSource()][i].m_Asset.SetAssetsManager(pAssetsManager);
		
		return &m_Assets[path.GetSource()][Id].m_Asset;
	}
	
	void LoadFromAssetsFile(class CAssetsManager* pAssetsManager, CDataFileReader* pAssetsFile, int Source)
	{
		int Start, Num;
		pAssetsFile->GetType(CAssetPath::TypeToStoredType(ASSET::TypeId), &Start, &Num);
		
		m_Assets[Source].clear();
		m_Assets[Source].set_size(Num);
		
		for(int i = 0; i < Num; i++)
		{
			class ASSET::CStorageType* pItem = (class ASSET::CStorageType*) pAssetsFile->GetItem(Start+i, 0, 0);
			ASSET* pAsset = &m_Assets[Source][i].m_Asset;
			pAsset->SetAssetsManager(pAssetsManager);
			pAsset->InitFromAssetsFile(pAssetsFile, pItem);
		}
	}
	
	void SaveInAssetsFile(CDataFileWriter* pFileWriter, int Source)
	{
		for(int i=0; i<m_Assets[Source].size(); i++)
		{
			m_Assets[Source][i].m_Asset.SaveInAssetsFile(pFileWriter, i);
		}
	}
	
	void Update()
	{
		for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
		{
			for(int i=0; i<m_Assets[s].size(); i++)
			{
				m_Assets[s][i].m_Asset.Update();
			}
		}
	}
	
	void Unload(int Source)
	{
		for(int i=0; i<m_Assets[Source].size(); i++)
		{
			m_Assets[Source][i].m_Asset.Unload();
		}
		m_Assets[Source].clear();
	}
	
	void DeleteAsset(const CAssetPath& Path)
	{
		if(!Path.IsNull() && Path.GetType() == ASSET::TypeId)
		{
			m_Assets[Path.GetSource()].remove_index(Path.GetId());
		}
	}
	
	void OnAssetDeleted(const CAssetPath& Path)
	{
		for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
		{
			for(int i=0; i<m_Assets[s].size(); i++)
			{
				m_Assets[s][i].m_Asset.OnAssetDeleted(Path);
			}
		}
	}
	
	void OnSubItemDeleted(const CAssetPath& Path, int SubItemPath)
	{
		for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
		{
			for(int i=0; i<m_Assets[s].size(); i++)
			{
				m_Assets[s][i].m_Asset.OnSubItemDeleted(Path, SubItemPath);
			}
		}
	}
};

}

#endif
