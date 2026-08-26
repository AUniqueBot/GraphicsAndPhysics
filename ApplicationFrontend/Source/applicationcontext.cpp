#include <applicationcontext.h>
#include <graphics/gfx_glfwCustomCallbackFunctions.h>
#include <configparser.h>



ApplicationContextProps ApplicationContextProps::GenerateWindowConfig(std::filesystem::path _path) {
	ApplicationContextProps props;
	ConfigData data = ParseConfigFile(_path);

	if (!data.empty()) {
		const ConfigSection& applicationProps = data.at("application");
		props.m_resolution.x = applicationProps.Get<int>("width");
		props.m_resolution.y = applicationProps.Get<int>("height");
		props.m_name = applicationProps.Get<std::string>("name");
		
		const ConfigSection& openglProps = data.at("opengl");
		props.m_openglProps.m_openglVersion.x = openglProps.Get<int>("majorversion");
		props.m_openglProps.m_openglVersion.y = openglProps.Get<int>("minorversion");
	}

	return props;
}

// --------------------------------------------------------------

void ApplicationContext::Init() {
	SetupInputManager();
	SetupGLFWWindow();
}


void ApplicationContext::Update() {
	if (!m_mainWindow) {
		LOG_ERROR("window was not initialized!");
		return;
	}
	Core& c = GetCore();
	UI_Core& uic = uiCore;


	while (!glfwWindowShouldClose(m_mainWindow)) {
		c.GetInputRouter().BeginFrame();
		c.PreUpdate();
		glfwPollEvents();
		c.Update();
		uic.Update();
		glfwSwapBuffers(m_mainWindow);
		c.PostUpdate();
	}
}


void ApplicationContext::Cleanup() {
	Core& c = GetCore();
	UI_Core& uic = uiCore;

	if (!m_mainWindow) {
		return;
	}


	uic.Exit();
	c.Stop();
	c.Cleanup();
	//uic.Exit();
	glfwTerminate();


	// save config of the application
	SaveWindowConfig();
}



Core& ApplicationContext::GetCore() {
	return Core::GetInstance();
}

const Core& ApplicationContext::GetCore() const {
	return Core::GetInstance();
}


UI_Core& ApplicationContext::GetUICore() {
	return uiCore;
}

const UI_Core& ApplicationContext::GetUICore() const {
	return uiCore;
}

void ApplicationContext::SaveWindowConfig() {
	namespace fs = std::filesystem;
	
	fs::path currentPath = fs::current_path() / "Config";
	fs::path configDir =
		fs::current_path() / "Config";
	std::filesystem::create_directories(configDir);
	// -------------------------------------------------
	fs::path configPath =
		configDir / "applicationconfig.cfg";

	ConfigData config;
	ConfigSection& application = config["application"];
	application.props["name"] = m_appProps.m_name;
	application.props["width"] = std::to_string(m_appProps.m_resolution.x);
	application.props["height"] = std::to_string(m_appProps.m_resolution.y);
	
	ConfigSection& opengl = config["opengl"];
	OpenGLProps& openglProps = m_appProps.m_openglProps;
	opengl.props["majorversion"] = std::to_string(openglProps.m_openglVersion.x);
	opengl.props["minorversion"] = std::to_string(openglProps.m_openglVersion.y);

	SaveConfigData(config, configPath);
}

void ApplicationContext::SetupInputManager() {
	InputRouter& ir = GetCore().GetInputRouter();
	InputSystem& cis = GetCore().GetInputSystem();

	ir.RegisterSystem(cis, "Core");
	ir.RegisterSystem(uiCore, "UI");

}

void ApplicationContext::SetupGLFWWindow() {
	Core& c = GetCore();
	UI_Core& uic = GetUICore();
	const OpenGLProps& openglProps = m_appProps.m_openglProps;
	const glm::ivec2& openglVer = openglProps.m_openglVersion;
	
	if (!glfwInit()) {
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglVer.x);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglVer.y);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	m_mainWindow = glfwCreateWindow(
		m_appProps.m_resolution.x, 
		m_appProps.m_resolution.y,
		m_appProps.m_name.c_str(),
		nullptr,
		nullptr
		);
	
	if (!m_mainWindow) {
		glfwTerminate();
		return;
	}

	c.SetWindow(m_mainWindow);
	glfwSetWindowUserPointer(m_mainWindow, this);
	glfwSetWindowUserPointer(m_mainWindow, UI_Core::FileDropCallback);





	glfwMakeContextCurrent(m_mainWindow);
	glfwSetInputMode(m_mainWindow, GLFW_STICKY_KEYS, GLFW_TRUE);

	// - GLEW Initialisation -------------------------------------------------------
	GLenum glewStatus = glewInit();
	//uic.Init(4, 6, mainWindow);

	if (glewStatus != GLEW_OK) {
		LOG_ERROR("Bad GLEW Init.");
		//uic.Exit();
		glfwTerminate();
		return;
	}

	c.Init();

	uic.Init(
		openglVer.x, 
		openglVer.y, 
		m_mainWindow, 
		c
	);
}


