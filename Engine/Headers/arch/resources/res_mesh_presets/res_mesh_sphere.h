#pragma once
#include <arch/resources/res_mesh.h>

struct SphereCreationProps {
	float radius				{ 0.5f };
	glm::ivec2 subdivisions		{ 8, 16 };
};

class Sphere : public Mesh {
private:
public:
	Sphere(SphereCreationProps _props = SphereCreationProps());
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

struct SphereHandle : public MeshHandle {
	inline SphereHandle(ResourceIdentifier _idr) : MeshHandle(_idr) {}

	inline float GetRadius() const {
		auto res = GetResource<Sphere>();
		return res ? res->GetRadius() : 0.f;
	}
	inline void SetRadius(float _radius) {
		auto res = GetResource<Sphere>();
		if (res) res->SetRadius(_radius);
	}

	inline int GetSubdivisionsX() const {
		auto res = GetResource<Sphere>();
		return res ? res->GetSubdivisionsX() : -1;
	}
	inline int GetSubdivisionsY() const {
		auto res = GetResource<Sphere>();
		return res ? res->GetSubdivisionsY() : -1;
	}
	inline glm::ivec2 GetSubdivisions() const {
		auto res = GetResource<Sphere>();
		return res ? res->GetSubdivisions() : glm::ivec2(-1, -1);
	}

	inline void SetSubdivisionsX(int _divs) {
		auto res = GetResource<Sphere>();
		if (res) res->SetSubdivisionsX(_divs);
	}
	inline void SetSubdivisionsY(int _divs) {
		auto res = GetResource<Sphere>();
		if (res) res->SetSubdivisionsY(_divs);
	}
	inline void SetSubdivisions(glm::ivec2 _divs) {
		auto res = GetResource<Sphere>();
		if (res) res->SetSubdivisions(_divs);
	}
};