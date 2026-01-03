#pragma once
#include <UI_Widget.h>



class UIWidget_AssetBrowser : public UIWidget {
public:
	enum SORTMETHOD {
		NAME,
		TYPE,
		MODIFIED,
		NONE
	};

public:
	UIWidget_AssetBrowser(std::string _widgetName);
	~UIWidget_AssetBrowser();


	void Init() override;
	void Draw() const override;
	void Exit() override;


	void SetResourceManager(ResourceManager* _resManager);

	void LoadEntries() const;



	void SortItemsBy(SORTMETHOD _sortMethod, bool _inversed = false) const;



private:
	ResourceManager* m_resourceManager{};
	
private:
	// mutable states the UI stores.
	// cache and reload.
	// path entries.
	mutable std::filesystem::path m_currentDir = "Assets/";
	mutable std::filesystem::path m_selectedPath;
	mutable std::vector<std::filesystem::path> m_directoryPaths;
};
 
