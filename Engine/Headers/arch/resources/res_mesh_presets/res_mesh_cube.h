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


struct CubeHandle : public MeshHandle {
	inline CubeHandle(ResourceIdentifierArg _resIdArg) : MeshHandle(_resIdArg) {}
	// provide dimension data here.


	// - dimensions ---------------------------------
	inline void SetXDimensions(float _dims) {
		auto res = GetResource<CubeRes>();
		if (res) res->SetXDimensions(_dims);
	}
	inline void SetYDimensions(float _dims) {
		auto res = GetResource<CubeRes>();
		if (res) res->SetYDimensions(_dims);
	}
	inline void SetZDimensions(float _dims) {
		auto res = GetResource<CubeRes>();
		if (res) res->SetZDimensions(_dims);
	}
	inline void SetDimensions(glm::vec3 _dims) {
		auto res = GetResource<CubeRes>();
		if (res) res->SetDimensions(_dims);
	}


	inline float GetXDimensions() {
		auto res = GetResource<CubeRes>();
		return res ? res->GetXDimensions() : -1.f;
	}
	inline float GetYDimensions() {
		auto res = GetResource<CubeRes>();
		return res ? res->GetYDimensions() : -1.f;
	}
	inline float GetZDimensions() {
		auto res = GetResource<CubeRes>();
		return res ? res->GetZDimensions() : -1.f;
	}
	inline glm::vec3 GetDimensions() {
		auto res = GetResource<CubeRes>();
		return res ? res->GetDimensions() : glm::vec3(-1.f, -1.f, -1.f);
	}

	// - subdivs ------------------------------------
	inline void SetXSubdivisions(int _dims) {
		auto res = GetResource<CubeRes>();
		if (res) res->SetXSubdivisions(_dims);
	}
	inline void SetYSubdivisions(int _dims) {
		auto res = GetResource<CubeRes>();
		if (res) res->SetYSubdivisions(_dims);
	}
	inline void SetZSubdivisions(int _dims) {
		auto res = GetResource<CubeRes>();
		if (res) res->SetZSubdivisions(_dims);
	}
	inline void SetSubdivisions(glm::ivec3 _dims) {
		auto res = GetResource<CubeRes>();
		if (res) res->SetSubdivisions(_dims);
	}

	inline int GetXSubdivisions() {
		auto res = GetResource<CubeRes>();
		return res ? res->GetXSubdivisions() : -1;
	}
	inline int GetYSubdivisions() {
		auto res = GetResource<CubeRes>();
		return res ? res->GetYSubdivisions() : -1;
	}
	inline int GetZSubdivisions() {
		auto res = GetResource<CubeRes>();
		return res ? res->GetZSubdivisions() : -1;
	}
	inline glm::ivec3 GetSubdivisions() {
		auto res = GetResource<CubeRes>();
		return res ? res->GetSubdivisions() : glm::ivec3(-1, -1, -1);
	}
};