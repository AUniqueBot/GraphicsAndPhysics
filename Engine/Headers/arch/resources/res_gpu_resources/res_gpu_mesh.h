#pragma once
#include <arch/resources/res_mesh/res_mesh.h>
#include <arch/resources/res_gpu_resources/res_gpu_vao.h>
#include <arch/resources/res_gpu_resources/res_gpu_buffer.h>
#include <arch/resources/res_gpu_resources/res_gpu_resourceHandle.h>




// attach a submesh
class GPU_Mesh : public GPU_VertexArrayObject {
public:
	void Init(Submesh& _mesh);

private:
	
};