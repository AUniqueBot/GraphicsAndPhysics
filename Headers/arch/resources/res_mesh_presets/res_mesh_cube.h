#pragma once
#include <arch/resources/res_mesh.h>


class Cube : public Mesh {
private:
public:
	
	void Init() override;
	const size_t GetVertexDataSize() const override;
	const float* GetVertexData() const override;
	const size_t GetNormalDataSize() const override;
	const float* GetNormalData() const override;
	const unsigned GetUVCount() const override;
	const size_t GetUVDataSize(unsigned _index = 0) const override;
	const float* GetUVData(unsigned _index = 0) const override;
	const size_t GetIndexDataSize() const override;
	const unsigned* GetIndexData() const override;
	const unsigned GetIndexDataCount() const override;


private:
	
};