#pragma once
#include <arch/resources/res_gpu_resources/res_gpu_resourceHandle.h>
#include <arch/resources/res_gpu_resources/res_gpu_resource_list.h>
#include <arch/datatypes/type_trackedStorage.h>





class GPUResourceManager {
	
public:


public:
	// Texture
	GPUResourceHandle CreateTexture(
		TextureProperties::TextureType _type,
		glm::ivec3 _dims,
		TextureProperties::TextureProps _props
	);
	GPUResourceHandle CreateTexture(const TextureRes& _res);

	GPUResourceHandle CreateGPUBuffer();
	GPUResourceHandle CreateVAO();
	GPUResourceHandle CreateMesh(MeshRes& _mesh);


	bool DeleteResource(GPUResourceHandle _handle);
	void Cleanup();
	void ClearAll();
	void ClearGPUBuffers();
	void ClearTextures();
	void ClearVAOs();
	void ClearMeshes();


	


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
		else if constexpr (std::is_same_v<T, GPU_Mesh>) {
			return GetResourceInternal(m_meshStorage, _id);
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
		else if constexpr (std::is_same_v<T, GPU_Mesh>) {
			return GetResourceInternal(m_meshStorage, _id);
		}
	}



private:
	// helper template functions.
	template <typename T>
	SparseSetView<T> GetResourceInternal(
		TrackedStorage<T>& _storage, 
		const GPUResourceHandle& _handle
	) {
		return _storage.GetResource(_handle.m_id);
	}

	template <typename T>
	SparseSetView<const T> GetResourceInternal(
		const TrackedStorage<T>& _storage, 
		const GPUResourceHandle& _handle
	) const {
		return _storage.GetResource(_handle.m_id);
	}

	template <typename T>
	static void ClearGPUResourceStorage(TrackedStorage<T>& _storage) {
		_storage.Clear();
	}

	template <typename T>
	bool DeleteResourceInternal(TrackedStorage<T>& _storage, GPUResourceHandle _handle) {
		return _storage.RemoveResource(_handle.m_id);
	}


private:
	// private functions

private:
	// private functions
	TrackedStorage<GPU_Mesh> m_meshStorage;
	TrackedStorage<GPU_Buffer> m_bufferStorage;
	TrackedStorage<GPU_Texture> m_textureStorage;
	TrackedStorage<GPU_VertexArrayObject> m_vaoStorage;
};

