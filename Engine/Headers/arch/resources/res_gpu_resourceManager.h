#pragma once
#include <arch/resources/res_gpu_resources/res_gpu_resource_list.h>
#include <arch/datatypes/type_trackedStorage.h>
#include <arch/resources/res_gpu_resourceHandle.h>



class GPUResourceManager {
	
public:


public:
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
		if constexpr (std::is_same_v<T, GPU_Texture>) {
			return GetResourceInternal(m_textureStorage, _id);
		}
		else if constexpr (std::is_same_v<T, GPU_Buffer>) {
			return GetResourceInternal(m_bufferStorage, _id);
		}
		else if constexpr (std::is_same_v<T, GPU_VertexArrayObject>) {
			return GetResourceInternal(m_vaoStorage, _id);
		}
	}

	template<typename T>
	SparseSetView<const T> GetResource(const GPUResourceHandle& _id) const {
		if constexpr (std::is_same_v<T, GPU_Texture>) {
			return GetResourceInternal(m_textureStorage, _id);
		}
		else if constexpr (std::is_same_v<T, GPU_Buffer>) {
			return GetResourceInternal(m_bufferStorage, _id);
		}
		else if constexpr (std::is_same_v<T, GPU_VertexArrayObject>) {
			return GetResourceInternal(m_vaoStorage, _id);
		}
	}



private:
	// helper template functions.
	template <std::derived_from<GPU_Resource> T>
	SparseSetView<T> GetResourceInternal(
		TrackedStorage<T>& _storage, 
		const GPUResourceHandle& _handle
	) {
		return _storage.GetResource(_handle.m_id);
	}

	template <std::derived_from<GPU_Resource> T>
	SparseSetView<const T> GetResourceInternal(
		const TrackedStorage<T>& _storage, 
		const GPUResourceHandle& _handle
	) const {
		return _storage.GetResource(_handle.m_id);
	}

	template <std::derived_from<GPU_Resource> T>
	static void ClearGPUResourceStorage(TrackedStorage<T>& _storage) {
		_storage.Clear();
	}

	template <std::derived_from<GPU_Resource> T>
	bool DeleteResourceInternal(TrackedStorage<T>& _storage, GPUResourceHandle _handle) {
		return _storage.RemoveResource(_handle.m_id);
	}


private:
	// private functions

private:
	// private functions
	TrackedStorage<GPU_Buffer> m_bufferStorage;
	TrackedStorage<GPU_Texture> m_textureStorage;
	TrackedStorage<GPU_VertexArrayObject> m_vaoStorage;
};

