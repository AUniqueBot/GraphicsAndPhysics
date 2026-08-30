#include <Widgets/UIWidget_AssetBrowser.h>
#include <serialization/serialize_helperfunctions.h>
#include <Windows.h>
#include <chrono>
namespace {



}

UIWidget_AssetBrowser::UIWidget_AssetBrowser(std::string _widgetName) : UIWidget(_widgetName) {

};

UIWidget_AssetBrowser::~UIWidget_AssetBrowser() {

}

void UIWidget_AssetBrowser::Init() {
	Core& c = Core::GetInstance();
	
	SetResourceManager(&c.GetResourceManager());
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
	if (ImGui::BeginPopupContextWindow("Options")) {
		reload |= DrawPopupContextMenu({ 
			PopupContextMenuProps::Target::None, 
			{ m_currentDir } 
			});
		ImGui::EndPopup();
	}
	reload |= AssetBrowserTable();

	// reload 
	if (reload) {
		// do something.
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
	namespace fs = std::filesystem;
	m_directoryEntries.clear();
	for (const auto& dirEntry : std::filesystem::directory_iterator(m_currentDir)) {
		AssetBrowserDataEntry dataEntry;
		fs::path currentPath = dirEntry;
		if (!fs::is_directory(dirEntry)) {

			// ignore everything but metafiles
			fs::path metafilePath = dirEntry.path();
			if (metafilePath.extension() != ".meta") continue;

			currentPath = metafilePath.replace_extension();
			if (!fs::exists(currentPath)) {
				LOG_WARN(
					"Missing resource file for metafile: " << 
					currentPath << "\n searching for " << 
					metafilePath
				);
				continue;
			}

			Serialization::JSONFile json;
			json.Parse(dirEntry);
			dataEntry.m_guid = json["guid"].GetUint64();
		}
		dataEntry.m_path = currentPath;
		// get the id direct from the metafile.
		m_directoryEntries.push_back(dataEntry);
		// ignore files with no meta file.
		// acceptance criteria
		// is a folder (regardless of meta)
	}
	SortItemsBy(NAME);
	SortItemsBy(TYPE);
}

void UIWidget_AssetBrowser::SortItemsBy(SORTMETHOD _sortMethod, bool _inversed) const {
	using SortFn = std::function<bool(const AssetBrowserDataEntry&, const AssetBrowserDataEntry&)>;
	SortFn sortFunction {};

	switch (_sortMethod) {
	case NAME:
		sortFunction = [_inversed](const AssetBrowserDataEntry& a,
			const AssetBrowserDataEntry& b) {
				const bool aDir = std::filesystem::is_directory(a.m_path);
				const bool bDir = std::filesystem::is_directory(b.m_path);

				// 1. Directories first
				if (aDir != bDir)
					return aDir > bDir;

				// 2. Alphabetical by filename
				return 
					_inversed ?
					a.m_path.filename().string() > b.m_path.filename().string() :
					a.m_path.filename().string() < b.m_path.filename().string();
			};
		break;
	case TYPE:
		sortFunction = [_inversed](const AssetBrowserDataEntry& a, const AssetBrowserDataEntry& b) {
			const bool aDir = std::filesystem::is_directory(a.m_path);
			const bool bDir = std::filesystem::is_directory(b.m_path);

			if (aDir != bDir)
				return aDir > bDir;

			if (aDir)
				return a.m_path.filename().string()
				< b.m_path.filename().string();

			const auto extA = a.m_path.extension().string();
			const auto extB = b.m_path.extension().string();

			if (extA != extB)
				return _inversed ? extA > extB : extA < extB;

			return _inversed ? 
				a.m_path.filename().string() > b.m_path.filename().string() :
				a.m_path.filename().string() < b.m_path.filename().string();
			};
		break;
	case MODIFIED:
		sortFunction = [_inversed](const AssetBrowserDataEntry& a, const AssetBrowserDataEntry& b_) {

			return false;
			};
		break;
	}

	
	if (sortFunction) {
		std::sort(m_directoryEntries.begin(), m_directoryEntries.end(),sortFunction);
	}
}


// --- asset browser widgets ------------------------------------------------------------------



bool UIWidget_AssetBrowser::AssetBrowserTable() {
	using namespace ImGui;
	bool reload{};



	Separator();
	UI_Core& uic = *UICore();
	const ImGuiTableFlags tableFlags =
		ImGuiTableFlags_::ImGuiTableFlags_Resizable |
		ImGuiTableFlags_::ImGuiTableFlags_Borders |
		ImGuiTableFlags_::ImGuiTableFlags_RowBg |
		ImGuiTableFlags_::ImGuiTableFlags_Sortable |
		ImGuiTableFlags_::ImGuiTableFlags_Reorderable
		;



	bool tableBegin = BeginTable("DirectoryContents", 4, tableFlags);
	if (tableBegin) {
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


		
		std::filesystem::path parentPath = std::filesystem::absolute(m_currentDir.parent_path());
		reload |= DrawTableEntryDirectory("...", parentPath);
		for (const auto& entry : m_directoryEntries) {
			reload |= DrawTableEntry(entry);
		}
		EndTable();
	}

	return reload;
}

bool UIWidget_AssetBrowser::DrawTableEntry(const AssetBrowserDataEntry& _entry) {
	using namespace ImGui;
	ImGuiSelectableFlags selectableFlags =
		ImGuiSelectableFlags_::ImGuiSelectableFlags_AllowDoubleClick |
		ImGuiSelectableFlags_::ImGuiSelectableFlags_SpanAllColumns
		;
	std::string filename = _entry.m_path.filename().string();
	std::string filepath = _entry.m_path.string();
	std::string extension = _entry.m_path.extension().string();
	bool isDir = std::filesystem::is_directory(_entry.m_path);
	bool reload{};

	TableNextRow();
	//  - col 0 ------------------
	ImGui::TableSetColumnIndex(0);
	ImGui::PushID(filepath.c_str());
	const bool clicked = Selectable(filename.c_str(), m_selectedPath == _entry.m_path, selectableFlags);
	if (ImGui::BeginPopupContextItem("Options")) {
		DrawPopupContextMenu({
			PopupContextMenuProps::Target::Item,
			{ _entry.m_path }
			});
		
		ImGui::EndPopup();
	}
	ImGui::PopID();



	if (clicked) {
		m_selectedPath = _entry.m_path;
		if (isDir && IsMouseDoubleClicked(0)) {
			// navigate to the next one.
			m_currentDir = _entry.m_path;
			m_selectedPath.clear();
			reload = true;
		}
	}

	//  - col 1 ------------------
	ImGui::TableSetColumnIndex(1);
	if (!isDir) {
		RESTYPE_ID restypeid = m_resourceManager->GetResourceType(extension);

	}
	
	//  - col 2 ------------------
	ImGui::TableSetColumnIndex(2);

	using namespace std::chrono;
	std::filesystem::file_time_type ftime = std::filesystem::last_write_time(_entry.m_path);
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
	ImGui::Text(oss.str().c_str());
	//  - col 3 ------------------
	ImGui::TableSetColumnIndex(3);
	if (!isDir) {
		// show id.
	}


	return reload;
}

bool UIWidget_AssetBrowser::DrawTableEntryDirectory(const std::string& _name, const std::filesystem::path& _path) {
	if (!std::filesystem::exists(_path)) return false;
	bool reload{};
	// first entry is always move back if there's a root.
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	const bool clicked = ImGui::Selectable("...##ASSETBROWSERUPONELEVEL", m_selectedPath == _path,
		ImGuiSelectableFlags_::ImGuiSelectableFlags_AllowDoubleClick |
		ImGuiSelectableFlags_::ImGuiSelectableFlags_SpanAllColumns 
	);

	if (clicked) {
		// path is ALWAYS absolute.
		m_selectedPath = _path;
		// Double-click to enter directory
		if (ImGui::IsMouseDoubleClicked(0)) {
			m_currentDir = _path;
			m_selectedPath.clear();
			reload = true;
		}
	}
	return reload;
}


// -- context menu -------------------------------

bool UIWidget_AssetBrowser::DrawPopupContextMenu(const PopupContextMenuProps& _props) {

	AssetManager& asmgr = ApplicationCore()->GetAssetManager();
	MaterialManager& matmgr = asmgr.GetMaterialManager();
	ShaderManager& shdmgr = asmgr.GetShaderManager();
	ShaderProgramManager& spmgr = asmgr.GetShaderProgramManager();
	std::filesystem::path currentPath = m_currentDir;
	std::string resourceName;
	
	bool reload{};
	if (_props.target == PopupContextMenuProps::Target::Item) {
		std::filesystem::path path = _props.paths[0];
		if (ImGui::MenuItem("Open")) {
			// if the current object 
			// what kind of object is it?
			std::wstring quotedpath = L"\"" + path.wstring() + L"\"";
			ShellExecute(
				NULL,
				L"open",
				L"code",
				quotedpath.c_str(),
				NULL,
				SW_SHOWDEFAULT
			);
		}
		if (ImGui::MenuItem("Delete")) {
			// Delete this file
			std::filesystem::remove(path);
			path.clear();
			reload = true;
		}
	}

	else if (_props.target == PopupContextMenuProps::Target::None) {
		// open directory in explorer
		std::filesystem::path path = _props.paths[0];
		if (ImGui::MenuItem("Open in Explorer")) {
			std::wstring quotedpath = L"\"" + path.wstring() + L"\"";
			ShellExecute(
				NULL,                  // Parent window handle
				L"open",               // Operation to perform (verb)
				quotedpath.c_str(),  // Path to the target directory
				NULL,                  // Parameters (none)
				NULL,                  // Default working directory
				SW_SHOWDEFAULT         // Show command window behavior
			);
		}
	}

	ImGui::Separator();
	// - creation ------------------------
	if (ImGui::BeginMenu("Create")) {

		if (ImGui::MenuItem("Folder")) {
			std::filesystem::path dirPath = m_currentDir / "New Folder";
			std::filesystem::create_directory(Serialization::GetUniquePath(dirPath));
			reload = true;
		}

		ImGui::Separator();

		namespace fs = std::filesystem;
		if (ImGui::MenuItem("Lambert Material")) {
			LambertMaterialHandle mat = matmgr.CreateLambertMaterial();
			// add a meta file and a material file.
			resourceName = "LambertMaterial";
			mat->Name(resourceName);
			fs::path matpath = Serialization::GetUniquePath(currentPath / (resourceName + ".material"));
			mat->ResourcePath(matpath);
			if (matmgr.SaveMaterial(mat.Get())) {
				fs::path metapath = matpath;
				metapath += ".meta";
				asmgr.SaveMetafileData(
					matmgr.CreateMetafileData(metapath, mat.GetBaseResource())
				);
			}
			reload = true;
		}

		if (ImGui::MenuItem("Vertex Shader")) {

			ShaderHandle shader = shdmgr.CreateShader(ShaderConstants::ShaderType::VERTEX);
			std::string resourceName = "NewVertexShader";
			shader->Name(resourceName);
			fs::path shaderPath = Serialization::GetUniquePath(currentPath / (resourceName + ".glsl"));
			shader->ResourcePath(shaderPath);
			if (false) {
				fs::path metapath = shaderPath;
				metapath += ".meta";
				asmgr.SaveMetafileData(
					shdmgr.CreateMetafileData(metapath, shader.Get())
				);
			}

			reload = true;
		}
		if (ImGui::MenuItem("Frag Shader")) {

			ShaderHandle shader = shdmgr.CreateShader(ShaderConstants::ShaderType::FRAG);
			std::string resourceName = "NewFragShader";
			shader->Name(resourceName);
			fs::path shaderPath = Serialization::GetUniquePath(currentPath / (resourceName + ".glsl"));
			shader->ResourcePath(shaderPath);
			// save
			if (false) {
				fs::path metapath = shaderPath;
				metapath += ".meta";
				asmgr.SaveMetafileData(
					shdmgr.CreateMetafileData(metapath, shader.Get())
				);
			}
			reload = true;
		}

		ImGui::EndMenu();
	}


		// - import --------------------------

	return reload;
}

RES_ID UIWidget_AssetBrowser::GetRESIDFromMetafile(const std::filesystem::path& _path) {
	// assumes is a metafile path and exists.
	Serialization::JSONFile jsonFile;
	jsonFile.Parse(_path);
	

	return jsonFile["guid"].GetUint64();
}


