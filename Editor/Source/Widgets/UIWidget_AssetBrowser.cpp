#include <Widgets/UIWidget_AssetBrowser.h>
#include <chrono>


UIWidget_AssetBrowser::UIWidget_AssetBrowser(std::string _widgetName) : UIWidget(_widgetName) {

};

UIWidget_AssetBrowser::~UIWidget_AssetBrowser() {

}

void UIWidget_AssetBrowser::Init() {
	Core& c = Core::GetInstance();
	
	SetResourceManager(&c.ResManager());
	// initialise the starting directory path.
	m_currentDir = "Assets/";
	m_currentDir = std::filesystem::absolute(m_currentDir);
	LoadEntries();

	LOG_INFO("Asset Manager Init.");
}

void UIWidget_AssetBrowser::Draw() {
	using namespace ImGui;

	bool reload = false;

	const bool goUpOneDir = ArrowButton("GoUp1Dir", ImGuiDir_Up);
	if (goUpOneDir && m_currentDir.has_parent_path()) {
		// navigate to parent dir (if any)
		m_currentDir = m_currentDir.parent_path();
		reload = true;
	}
	SameLine();
	Text("Path: %s", m_currentDir.string().c_str());


	Separator();
	//EngineInputEnabled();

	;


	const ImGuiTableFlags tableFlags = 
		ImGuiTableFlags_::ImGuiTableFlags_Resizable | 
		ImGuiTableFlags_::ImGuiTableFlags_Borders |
		ImGuiTableFlags_::ImGuiTableFlags_RowBg |
		ImGuiTableFlags_::ImGuiTableFlags_Sortable |
		ImGuiTableFlags_::ImGuiTableFlags_Reorderable
		;

	BeginTable("DirectoryContents", 4, tableFlags);

	TableSetupColumn("Name");
	TableSetupColumn("File Type");
	TableSetupColumn("Last Modified");
	TableSetupColumn("Asset ID");
	TableHeadersRow();




	if (ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs()) {
		if (sortSpecs->SpecsDirty) {
			const auto& sortSpec = sortSpecs->Specs[0]; // only primary sort
			SORTMETHOD method = NONE;
			switch (sortSpec.ColumnIndex) {
			case 0: method = NAME; break;
			case 1: method = TYPE; break;
			case 2: method = MODIFIED; break; // could implement last modified sort
			}
			bool inversed = (sortSpec.SortDirection == ImGuiSortDirection_Descending);
			SortItemsBy(method, inversed);
			sortSpecs->SpecsDirty = false; // mark as handled
		}
	}


	for (const std::filesystem::path& path : m_directoryPaths) {
		TableNextRow();
		/* [name | extension | last modified]*/
		bool isDir = std::filesystem::is_directory(path);
		std::string label = isDir
			? "[DIR] " + path.filename().string()
			: path.filename().string();

		PushID(path.c_str());

		ImGuiSelectableFlags selectableFlags = 
			ImGuiSelectableFlags_::ImGuiSelectableFlags_AllowDoubleClick |
			ImGuiSelectableFlags_::ImGuiSelectableFlags_SpanAllColumns 
			;
			
		TableSetColumnIndex(0);
		const bool clicked = Selectable(label.c_str(), m_selectedPath == path, selectableFlags);



	

		if (BeginDragDropSource()) {
			// You can set any payload type you want, e.g., "ASSET_MESH"

			// send the data of the thing here.
			SetDragDropPayload("ASSET", path.string().c_str(), path.string().size() + 1);
			Text("Dragging %s", label.c_str());
			EndDragDropSource();
		}

		if (clicked) {
			// path is ALWAYS absolute.
			std::filesystem::path currentPathAbs = std::filesystem::absolute(path);
			m_selectedPath = currentPathAbs;
			// Double-click to enter directory

			if (isDir && ImGui::IsMouseDoubleClicked(0)) {
				m_currentDir /= path.filename();
				reload = true;
			}
		}
		PopID();

		// file type

		std::string fileType{isDir ? "Directory" : ""};

		if (m_resourceManager && !isDir) {
			RESTYPE_ID type = m_resourceManager->GetResourceType(path.extension().string());
			if (type != 0) {
				const ResourceTypeMetadata& rtm	{ m_resourceManager->GetResourceTypeMetadata(type) } ;
				fileType = rtm.GetName();
			}
		}
		ImGui::TableSetColumnIndex(1);
		Text(fileType.c_str());

		// last modified
		using namespace std::chrono;
		std::filesystem::file_time_type ftime = std::filesystem::last_write_time(path);
		auto sctp = time_point_cast<system_clock::duration>(
			ftime - decltype(ftime)::clock::now() + system_clock::now()
		);

		std::time_t cftime = system_clock::to_time_t(sctp);
		std::tm localTime;


#ifdef _WIN32
		localtime_s(&localTime, &cftime);
#else
		localtime_r(&cftime, &localTime);
#endif
		std::ostringstream oss;
		oss << std::put_time(&localTime, "%d/%m/%Y %H:%M:%S");
		
		TableSetColumnIndex(2);
		ImGui::Text(oss.str().c_str());



		
		if (!m_resourceManager) {
			continue;
		}
		//auto& rsPool = m_resourceManager->GetResourcePool();
		ResourceManager& resm = *m_resourceManager;
		std::string fname = path.filename().string();
		std::shared_ptr<BaseResource> resPoint = resm.GetResource(fname);
		if (!resPoint) continue;
		BaseResource& res = *resPoint;
		// find if item is already loaded.
		std::stringstream ss{ };
		ss << res.ResourceID();
		TableSetColumnIndex(3);
		Text(ss.str().c_str()); // looks ok.
	}
	EndTable();


	// reload entries when the directory changes
	if (reload) {
		LoadEntries();
	}
}

void UIWidget_AssetBrowser::Exit() {

}

void UIWidget_AssetBrowser::SetResourceManager(ResourceManager* _resManager) {
	if (_resManager == m_resourceManager) return;
	m_resourceManager = _resManager;
}

void UIWidget_AssetBrowser::LoadEntries() const {
	m_directoryPaths.clear();

	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_currentDir)) {
		m_directoryPaths.push_back(entry.path());
	}
	SortItemsBy(NAME);
	SortItemsBy(TYPE);
}

void UIWidget_AssetBrowser::SortItemsBy(SORTMETHOD _sortMethod, bool _inversed) const {
	using DirPath = std::filesystem::path;
	using SortFn = std::function<bool(const DirPath&, const DirPath&)>;
	SortFn sortFunction {};

	switch (_sortMethod) {
	case NAME:
		sortFunction = [_inversed](const DirPath& a,
			const DirPath& b) {
				const bool aDir = std::filesystem::is_directory(a);
				const bool bDir = std::filesystem::is_directory(b);

				// 1. Directories first
				if (aDir != bDir)
					return aDir > bDir;

				// 2. Alphabetical by filename
				return 
					_inversed ?
					a.filename().string() > b.filename().string():
					a.filename().string() < b.filename().string();
			};
		break;
	case TYPE:
		sortFunction = [_inversed](const DirPath& a, const DirPath& b) {
			const bool aDir = std::filesystem::is_directory(a);
			const bool bDir = std::filesystem::is_directory(b);

			if (aDir != bDir)
				return aDir > bDir;

			if (aDir)
				return a.filename().string()
				< b.filename().string();

			const auto extA = a.extension().string();
			const auto extB = b.extension().string();

			if (extA != extB)
				return _inversed ? extA > extB : extA < extB;

			return _inversed ? 
				a.filename().string() > b.filename().string() : 				
				a.filename().string() < b.filename().string();
			};
		break;
	case MODIFIED:
		sortFunction = [_inversed](const DirPath& a, const DirPath& b_) {

			return false;
			};
		break;
	}

	
	if (sortFunction) {
		std::sort(m_directoryPaths.begin(), m_directoryPaths.end(),sortFunction);
	}
}


