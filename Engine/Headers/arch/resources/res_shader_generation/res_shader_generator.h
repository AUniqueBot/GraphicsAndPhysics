#pragma once
#include <arch/resources/res_shader.h>


class ShaderGenerator {
	
public:
	void SetVersion(unsigned _major, unsigned _minor);


protected:


	std::string GenerateVersionHeader() const;
	
	// - vertex shader -------------------------------------------------------
	std::string GenerateScreenSpaceAttributes() const;




private:
	unsigned m_majorVersion { 4 };
	unsigned m_minorVersion { 6 };
};


/*
	typical shaders can be broken down to simple



*/