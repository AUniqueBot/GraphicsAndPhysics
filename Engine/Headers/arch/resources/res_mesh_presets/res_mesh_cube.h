#pragma once
#include <arch/resources/res_mesh.h>


class Cube : public Mesh {
private:
public:
	
	void Init() override;
	//const size_t GetVertexDataSize() const override;
	//const float* GetVertexData() const override;
	//const size_t GetNormalDataSize() const override;
	//const float* GetNormalData() const override;
	//const unsigned GetUVCount() const override;
	//const size_t GetUVDataSize(unsigned _index = 0) const override;
	//const float* GetUVData(unsigned _index = 0) const override;
	//const size_t GetIndexDataSize() const override;
	//const glm::uvec3* GetIndexData() const override;
	//const unsigned GetIndexDataCount() const override;


	void GenerateVertexData();

private:
	glm::vec3 m_dimensions					{ 1.f, 1.f, 1.f };	// generate by face
	glm::ivec3 m_subdivisions				{ 0, 0, 0 }; // default of 0; no subdivisions.
};