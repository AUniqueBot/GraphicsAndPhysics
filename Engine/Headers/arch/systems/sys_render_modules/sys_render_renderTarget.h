#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <unordered_map>
#include <set>
#include <arch/systems/sys_render_modules/sys_render_renderTargetAttachments.h>
/*
	class to handle render targets - where the gpu pastes it's render data to 
	basically render layers
*/



constexpr unsigned C_RENDER_MATERIAL	{ 0 };
constexpr unsigned C_RENDER_OBJECTID	{ 1 };


class RenderTarget	 {
public:
	RenderTarget(glm::vec2 _res = glm::vec2{1920, 1080}, std::string _name="rt") : 
		m_resolution{_res}, 
		m_renderTargetName{_name}
	{
	};
	~RenderTarget();


	// 0 == color attachment
	// 1 == object id



	void Resolution(glm::ivec2 _res);
	void Resolution(unsigned _width, unsigned _height);
	glm::ivec2 Resolution() const { return m_resolution; }


	const std::string& Name() const { return m_renderTargetName; }
	std::string& Name() { return m_renderTargetName; }
	void Name(const std::string& _name) { m_renderTargetName = _name; }


	const ColorAttachment& GetColorAttachment(unsigned _index) const;
	unsigned GetColorAttachmentTextureID(unsigned _index = C_RENDER_MATERIAL) const;
	unsigned GetDepthAttachmentTextureID() const;
	std::vector<ColorAttachment>& GetColorAttachments() { return m_colorAttachments; }
	const std::vector<ColorAttachment>& GetColorAttachments() const { return m_colorAttachments; }

	

	void AddInitialColorAttachments();
	bool AddColorAttachment(ColorAttachment _colAttachment);
	bool SetDepthAttachment(DepthAttachment _depthAttachment);
	size_t GetColorAttachmentCount() const { return m_colorAttachments.size(); }
	
	
	// - building ------------------------------------------------------------------------

	// builds the render target
	bool Build();
	void Destroy();

	unsigned FBO() const;

	void Bind() const;
	void Unbind() const;

	unsigned PickPixel(
		glm::vec2 _vec, 
		unsigned _colorAttachment = C_RENDER_MATERIAL,
		glm::vec2 _readResolution = glm::vec2(1, 1)
	) const;


	void LogBindErrors() const;
	bool VerifyFBOCompleteness() const;



private:
	void Resize(glm::ivec2 _newDimensions);
	void Resize(unsigned _width, unsigned _height);


private:

	unsigned m_fbo													{}; // frame buffer identifier 
	std::vector<unsigned> m_fboColor								{};	// color attachments
	std::vector<ColorAttachment> m_colorAttachments					{};
	unsigned m_currentColorAttachmentCounter						{};


	// specific color attachments that must always be there.
	DepthAttachment m_depthAttachment;	
	unsigned m_fboDepth												{}; // depth attachment

	bool m_isValid													{};

	std::string m_renderTargetName;
	glm::ivec2 m_resolution											{};
};