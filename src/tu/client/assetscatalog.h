#ifndef TU_CLIENT_ASSETSCATALOG_H
#define TU_CLIENT_ASSETSCATALOG_H

#include <base/tl/array.h>
#include <tu/client/assets.h>

namespace tu
{

template<class T>
class CAssetCatalog
{
public:
	array<T> m_Assets[CAssetPath::NUM_SOURCES];

public:	
	T* GetAsset(const CAssetPath& path)
	{
		if(path.GetType() != T::TypeId)
			return 0;
		
		int Id = path.GetId();
		if(Id < 0)
			return 0;
			
		if(Id < m_Assets[path.GetSource()].size())
			return &m_Assets[path.GetSource()][Id];
		else
			return 0;
	}
	
	T* NewAsset(CAssetPath* path, int Source)
	{
		int Id = m_Assets[Source].add(T());
		*path = CAssetPath::Asset(T::TypeId, Source, Id);
		
		return &m_Assets[Source][Id];
	}
	
	T* NewAsset(const CAssetPath& path)
	{
		if(path.GetType() != T::TypeId)
			return 0;
		
		int Id = path.GetId();
		if(Id < 0)
			return 0;
		
		int Size = max(m_Assets[path.GetSource()].size(), Id+1);
		m_Assets[path.GetSource()].set_size(Size);
		
		return &m_Assets[path.GetSource()][Id];
	}
	
	void LoadFromAssetsFile(class CAssetsManager* pAssetsManager, tu::IAssetsFile* pAssetsFile, int Source)
	{
		dbg_msg("assetmanager", "load assets of type %d", T::TypeId);
		int Start, Num;
		pAssetsFile->GetType(CAssetPath::TypeToStoredType(T::TypeId), &Start, &Num);
		
		m_Assets[Source].clear();
		m_Assets[Source].set_size(Num);
		
		for(int i = 0; i < Num; i++)
		{
			class T::CStorageType* pItem = (class T::CStorageType*) pAssetsFile->GetItem(Start+i, 0, 0);
			T* pAsset = &m_Assets[Source][i];
			pAsset->InitFromAssetsFile(pAssetsManager, pAssetsFile, pItem);
		}
	}
	
	void SaveInAssetsFile(class CDataFileWriter* pFileWriter, int Source)
	{
		for(int i=0; i<m_Assets[Source].size(); i++)
		{
			m_Assets[Source][i].SaveInAssetsFile(pFileWriter, i);
		}
	}
	
	void Update()
	{
		for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
		{
			for(int i=0; i<m_Assets[s].size(); i++)
			{
				m_Assets[s][i].Update();
			}
		}
	}
	
	void Unload(int Source)
	{
		for(int i=0; i<m_Assets[Source].size(); i++)
		{
			m_Assets[Source][i].Unload();
		}
		m_Assets[Source].clear();
	}
	
	void DeleteAsset(const CAssetPath& Path)
	{
		if(!Path.IsNull() && Path.GetType() == T::TypeId)
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
				m_Assets[s][i].OnAssetDeleted(Path);
			}
		}
	}
	
	void OnSubItemDeleted(const CAssetPath& Path, int SubItemPath)
	{
		for(int s=0; s<CAssetPath::NUM_SOURCES; s++)
		{
			for(int i=0; i<m_Assets[s].size(); i++)
			{
				m_Assets[s][i].OnSubItemDeleted(Path, SubItemPath);
			}
		}
	}
};

}

#endif
