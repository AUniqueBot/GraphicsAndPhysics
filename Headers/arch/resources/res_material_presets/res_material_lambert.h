#pragma once
#include <pch.h>
#include <arch/resources/res_material.h>


class LambertMaterial : public Material {

public:

	void Init() override;


	// - getters -------------------------------------
	const glm::vec4& Color() const;
	void Color(glm::vec4 _newColor);
	void Color(unsigned _newColor);



private:
	glm::vec4 m_color {};
};

