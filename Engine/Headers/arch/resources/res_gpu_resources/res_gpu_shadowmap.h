#pragma once
#include <arch/resources/res_gpu_resources/res_gpu_fbo.h>
#include <arch/resources/res_gpu_resources/res_gpu_texture.h>


class GPU_Shadowmap : public GPU_Resource{


private:
	GPU_Texture m_texture;
	GPU_FrameBufferObject m_fbo;
};