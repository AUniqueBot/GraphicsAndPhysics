#include <pch.h>
#include <arch/resources/res_gpu_resources/res_gpu_mesh.h>

void GPU_Mesh::Init(Submesh& _mesh) {
	Create();
	
	SetupAttributes(VertexLayouts::C_STATIC_MESH);


	const AttributeData& attrs = _mesh.GetVertexInformation();
}
