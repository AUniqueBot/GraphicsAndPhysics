#pragma once
#include <arch/resources/res_mesh/res_mesh.h>

struct CubeCreationProps {
	glm::vec3 dimensions		{ 1.f, 1.f, 1.f };
	glm::ivec3 subdivisions		{ 0, 0, 0 };
};


class CubeRes : public MeshRes {
private:
public:
	CubeRes(CubeCreationProps _props = CubeCreationProps());
	void Init() override;


	// - cube specific dimensions ----------------
	void SetXDimensions(float _dims);
	void SetYDimensions(float _dims);
	void SetZDimensions(float _dims);
	void SetDimensions(glm::vec3 _dims);

	const float& GetXDimensions() const;
	const float& GetYDimensions() const;
	const float& GetZDimensions() const;
	const glm::vec3& GetDimensions() const;

	void SetXSubdivisions(int _dims);
	void SetYSubdivisions(int _dims);
	void SetZSubdivisions(int _dims);
	void SetSubdivisions(glm::ivec3 _dims);


	const int& GetXSubdivisions() const;
	const int& GetYSubdivisions() const;
	const int& GetZSubdivisions() const;
	const glm::ivec3& GetSubdivisions() const;


	// 

protected:
	void UpdateVertexData();

private:
	glm::vec3 m_dimensions					{ 1.f, 1.f, 1.f };	// generate by face
	glm::ivec3 m_subdivisions				{ 0, 0, 0 }; // default of 0; no subdivisions.
};


// ----------------------------------------------------------------------------------


using CubeHandle = ResourceHandleDerivedT<MeshRes, CubeRes>;