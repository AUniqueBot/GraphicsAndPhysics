#pragma once
#include <arch/resources/res_mesh/res_mesh.h>

struct SphereCreationProps {
	float radius				{ 0.5f };
	glm::ivec2 subdivisions		{ 8, 16 };
};

class SphereRes : public MeshRes {
private:
public:
	SphereRes(SphereCreationProps _props = SphereCreationProps());
	void Init() override;

	const float& GetRadius() const;
	void SetRadius(float _radius);

	const int& GetSubdivisionsX() const;
	const int& GetSubdivisionsY() const;
	const glm::ivec2& GetSubdivisions() const;

	void SetSubdivisionsX(int _divs);
	void SetSubdivisionsY(int _divs);
	void SetSubdivisions(glm::ivec2 _divs);



protected:
	void UpdateVertexData();
private:
	float m_radius				{ 0.5f };
	glm::ivec2 m_subdivisions	{ 8, 8 }; // width-wise
};

// --------------------------------------------------------------------------------

using SphereHandle = ResourceHandleDerivedT<MeshRes, SphereRes>;