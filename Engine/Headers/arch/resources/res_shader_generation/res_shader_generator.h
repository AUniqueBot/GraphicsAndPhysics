#pragma once
#include <arch/resources/res_shader.h>
#include <arch/systems/sys_render_modules/sys_render_vaoManager.h>


class ShaderGenerator {
	
public:
	void SetVersion(unsigned _major, unsigned _minor);


protected:


	std::string GenerateVersionHeader() const;
	
	// - vertex shader -------------------------------------------------------
	std::string GenerateWorldObjectAttributes(const VAOHandler& _vaoHandler) const;
	std::string GenerateScreenSpaceAttributes() const;




private:
	unsigned m_majorVersion { 4 };
	unsigned m_minorVersion { 6 };
};


/*
	typical shaders can be broken down to simple



*/