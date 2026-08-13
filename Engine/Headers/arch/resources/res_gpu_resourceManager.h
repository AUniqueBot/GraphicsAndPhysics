#pragma once
#include <arch/resources/res_gpu_resources/res_gpu_resource_list.h>
#include <arch/datatypes/type_sparseSet.h>


using GPURES_ID = uint16_t;
constexpr const GPURES_ID C_GPURES_INVALID_ID { 0 };

enum class GPUDatatype {
	Texture,
	Buffer,
	IndexBuffer,
	VAO,
	FrameBuffer
};

struct GPUResourceHandle {
	const GPURES_ID m_id;
	const GPUDatatype m_type;
};


class GPUResourceManager {


	GPUResourceHandle CreateGPUBuffer();
	GPUResourceHandle CreateTexture(
		TextureProperties::TextureType _type,
		glm::ivec3 _dims,
		TextureProperties::TextureProps _props
	);
	GPUResourceHandle CreateVAO();
	


	bool DeleteResource(GPUResourceHandle _handle);
	void ClearAll();
	void ClearGPUBuffers();
	void ClearTextures();
	void ClearVAOs();

	template<typename T>
	SparseSetView<T> GetResource(const GPUResourceHandle& _id) {
		GPURES_ID id = _id.m_id;
		if constexpr (std::is_same_v<T, GPU_Texture>) {
			return m_textureStorage.At(id);
		}
		else if constexpr (std::is_same_v<T, GPU_Buffer>) {
			return m_bufferStorage.At(id);
		}
		else if constexpr (std::is_same_v<T, GPU_VertexArrayObject>) {
			return m_vaoStorage.At(id);
		}
	}

	template<typename T>
	SparseSetView<const T> GetResource(const GPUResourceHandle& _id) const {
		GPURES_ID id = _id.m_id;
		if constexpr (std::is_same_v<T, GPU_Texture>) {
			return m_textureStorage.At(id);
		}
		else if constexpr (std::is_same_v<T, GPU_Buffer>) {
			return m_bufferStorage.At(id);
		}
		else if constexpr (std::is_same_v<T, GPU_VertexArrayObject>) {
			return m_vaoStorage.At(id);
		}
	}



private:
	template <std::derived_from<GPU_Resource> T>
	void ClearGPUResStorage(SparseSet<GPURES_ID, T> _storage) {
		for (T& res : _storage) {
			res.Destroy();
		}
		_storage.clear();
	}


	bool DeleteTexture(GPUResourceHandle _handle);
	bool DeleteGPUBuffer(GPUResourceHandle _handle);
	bool DeleteVAO(GPUResourceHandle _handle);
private:
	inline static GPURES_ID s_counter{};
	static GPURES_ID GenerateResID();
private:
	SparseSet<GPURES_ID, GPU_Buffer> m_bufferStorage;
	SparseSet<GPURES_ID, GPU_Texture> m_textureStorage;
	SparseSet<GPURES_ID, GPU_VertexArrayObject> m_vaoStorage;


	
};

