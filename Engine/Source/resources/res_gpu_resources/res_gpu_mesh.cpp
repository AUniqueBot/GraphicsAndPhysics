#include <pch.h>
#include <arch/resources/res_gpu_resources/res_gpu_mesh.h>

void GPU_Mesh::Init(Mesh& _mesh) {
	Create();
	const AttributeData& attrs = _mesh.GetVertexInformation();
	for (const auto& [attrName, attr] : attrs) {
		LOG_INFO("Getting attribute for: " << attrName);
		


		//SetAttribute();
	}

}
