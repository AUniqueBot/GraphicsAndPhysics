#pragma once
#include <UI_Widget.h>


// for use in the thing.
struct AssetBrowserDataEntry {
	std::filesystem::path	 m_path {};
	RES_ID m_guid			{ ResourceConstants::C_RES_INVALID_ID };
};


struct PopupContextMenuProps {
	enum class Target {
		None,
		Item,
		Selection
	};
	Target target;
	std::vector<std::filesystem::path> paths;
};

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
	void Draw() override;
	void Exit() override;


	void SetResourceManager(ResourceManager* _resManager);




	void LoadEntries() const;
	void SortItemsBy(SORTMETHOD _sortMethod, bool _inversed = false) const;

private:
	// internal widgets
	bool AssetBrowserTable();
	bool DrawTableEntry(const AssetBrowserDataEntry& _entry);
	bool DrawTableEntryDirectory(
		const std::string& _name, 
		const std::filesystem::path& _path,
		const int& _flags
	);
	bool DrawTableEntryResource(
		const std::string& _name, 
		const RES_ID& _guid,  
		const std::filesystem::path& _path,
		const int& _flags 
		
	);

	// --------------------------------------------
	bool DrawPopupContextMenu(const PopupContextMenuProps& _props);

	// --------------------------------------------
	RES_ID GetRESIDFromMetafile(const std::filesystem::path& _path) const;
private:
	ResourceManager* m_resourceManager{};
	
private:
	// mutable states the UI stores.
	// cache and reload.
	// path entries.
	mutable std::filesystem::path m_currentDir = std::filesystem::absolute("Assets/");
	mutable std::filesystem::path m_selectedPath;
	mutable RES_ID m_selectedResource;
	mutable std::vector<AssetBrowserDataEntry> m_directoryEntries;
	mutable bool m_itemContextMenu{};
};
 
