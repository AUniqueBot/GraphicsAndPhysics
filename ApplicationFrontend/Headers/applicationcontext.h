#pragma once
#include <pch.h>
#include <arch/core.h>
#include <UI_Core.h>

enum class GraphicsAPI {
	OpenGL,
	// - unsupported --------------
	Vulkan,
	DX12,
	Metal
};

struct OpenGLProps {
	glm::ivec2 m_openglVersion	{ 4, 6 };
};

struct ApplicationContextProps {
	std::string m_name			{ "ApplicationName" };
	glm::ivec2 m_resolution		{ 1920, 1080 };
	bool m_resizable			{ true };
	bool m_fullscreen			{ false };

	OpenGLProps m_openglProps;

	static ApplicationContextProps GenerateWindowConfig(std::filesystem::path _path);
};

class ApplicationContext {

public:
	ApplicationContext() = default;
	ApplicationContext(ApplicationContextProps _props) : m_appProps{ _props } {}


public:
	void Init();
	void Update();
	void Cleanup();
public:
	Core& GetCore();
	const Core& GetCore() const;
	UI_Core& GetUICore();
	const UI_Core& GetUICore() const;
	
public:

	void SaveWindowConfig();
private:
	// -- init functions ----------------
	void SetupInputManager();
	void SetupGLFWWindow();


private:
	UI_Core uiCore;
	ApplicationContextProps m_appProps;
	GLFWwindow* m_mainWindow	{};

};