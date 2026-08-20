#pragma once
#include <arch/resources/res_mesh/res_mesh.h>

struct PlaneCreationProps {
	glm::vec2 dimensions		{ 1.f, 1.f };
	glm::ivec2 subdivisions		{ 0, 0 };
};


class PlaneRes : public MeshRes {
private:
public:
	PlaneRes(PlaneCreationProps _props = PlaneCreationProps());

	void Init() override;

	// - plane specific dimensions ---------------
	void SetXDimensions(float _dims);
	void SetYDimensions(float _dims);
	void SetDimensions(glm::vec2 _dims);

	const float& GetXDimensions() const;
	const float& GetYDimensions() const;
	const glm::vec2& GetDimensions() const;

	void SetXSubdivisions(int _dims);
	void SetYSubdivisions(int _dims);
	void SetSubdivisions(glm::ivec2 _dims);

	const int& GetXSubdivisions() const;
	const int& GetYSubdivisions() const;
	const glm::ivec2& GetSubdivisions() const;

protected:
	void UpdateVertexData();
private:
	glm::vec2 m_dimensions				{ 1.f, 1.f };
	glm::ivec2 m_subdivisions			{ 0, 0 };
};

// --------------------------------------------------------------------------------

using PlaneHandle = ResourceHandleDerivedT<MeshRes, PlaneRes>;