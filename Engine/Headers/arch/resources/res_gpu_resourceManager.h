#pragma once
#include <graphics/resources/gfx_gpuresourceHandle.h>
#include <graphics/resources/gfx_resourcelist.h>
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
		if constexpr (std::is_same_v<T, GPUTexture>) {
			return GetResourceInternal(m_textureStorage, _id);
		}
		else if constexpr (std::is_same_v<T, GPUBuffer>) {
			return GetResourceInternal(m_bufferStorage, _id);
		}
		else if constexpr (std::is_same_v<T, GPUVertexArrayObject>) {
			return GetResourceInternal(m_vaoStorage, _id);
		}
		else if constexpr (std::is_same_v<T, GPUMesh>) {
			return GetResourceInternal(m_meshStorage, _id);
		}
	}

	template<typename T>
	SparseSetView<const T> GetResource(const GPUResourceHandle& _id) const {
		if constexpr (std::is_same_v<T, GPUTexture>) {
			return GetResourceInternal(m_textureStorage, _id);
		}
		else if constexpr (std::is_same_v<T, GPUBuffer>) {
			return GetResourceInternal(m_bufferStorage, _id);
		}
		else if constexpr (std::is_same_v<T, GPUVertexArrayObject>) {
			return GetResourceInternal(m_vaoStorage, _id);
		}
		else if constexpr (std::is_same_v<T, GPUMesh>) {
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
	TrackedStorage<GPUMesh> m_meshStorage;
	TrackedStorage<GPUBuffer> m_bufferStorage;
	TrackedStorage<GPUTexture> m_textureStorage;
	TrackedStorage<GPUVertexArrayObject> m_vaoStorage;
};

