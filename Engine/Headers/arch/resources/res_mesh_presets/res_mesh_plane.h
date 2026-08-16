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

struct PlaneHandle : public MeshHandle {
	inline PlaneHandle(ResourceIdentifierArg _resIdArg) : MeshHandle(_resIdArg) {}
	// provide dimension data here.


	// - dimensions ---------------------------------
	inline void SetXDimensions(float _dims) {
		auto res = GetResource<PlaneRes>();
		if (res) res->SetXDimensions(_dims);
	}
	inline void SetYDimensions(float _dims) {
		auto res = GetResource<PlaneRes>();
		if (res) res->SetYDimensions(_dims);
	}
	inline void SetDimensions(glm::vec3 _dims) {
		auto res = GetResource<PlaneRes>();
		if (res) res->SetDimensions(_dims);
	}


	inline float GetXDimensions() {
		auto res = GetResource<PlaneRes>();
		return res ? res->GetXDimensions() : -1.f;
	}
	inline float GetYDimensions() {
		auto res = GetResource<PlaneRes>();
		return res ? res->GetYDimensions() : -1.f;
	}
	inline glm::vec2 GetDimensions() {
		auto res = GetResource<PlaneRes>();
		return res ? res->GetDimensions() : glm::vec2(-1.f, -1.f);
	}

	// - subdivs ------------------------------------
	inline void SetXSubdivisions(int _dims) {
		auto res = GetResource<PlaneRes>();
		if (res) res->SetXSubdivisions(_dims);
	}
	inline void SetYSubdivisions(int _dims) {
		auto res = GetResource<PlaneRes>();
		if (res) res->SetYSubdivisions(_dims);
	}
	inline void SetSubdivisions(glm::ivec3 _dims) {
		auto res = GetResource<PlaneRes>();
		if (res) res->SetSubdivisions(_dims);
	}

	inline int GetXSubdivisions() {
		auto res = GetResource<PlaneRes>();
		return res ? res->GetXSubdivisions() : -1;
	}
	inline int GetYSubdivisions() {
		auto res = GetResource<PlaneRes>();
		return res ? res->GetYSubdivisions() : -1;
	}
	inline glm::ivec2 GetSubdivisions() {
		auto res = GetResource<PlaneRes>();
		return res ? res->GetSubdivisions() : glm::ivec2(-1, -1);
	}
};