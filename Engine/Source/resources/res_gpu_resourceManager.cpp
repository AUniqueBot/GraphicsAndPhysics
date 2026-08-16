#include <arch/resources/res_gpu_resourceManager.h>



GPUResourceHandle GPUResourceManager::CreateGPUBuffer() {
    GPU_Buffer buffer;
    GPURES_ID id { m_bufferStorage.AddResource(std::move(buffer)) };
    return { id, GPUDatatype::Buffer };
}

GPUResourceHandle GPUResourceManager::CreateTexture(
    TextureProperties::TextureType _type,
    glm::ivec3 _dims,
    TextureProperties::TextureProps _props
) {
    GPU_Texture tex(_type, _dims, _props);
    tex.Create();
    tex.Allocate();
    GPURES_ID id = m_textureStorage.AddResource(std::move(tex));
    return { id, GPUDatatype::Texture };
}



GPUResourceHandle GPUResourceManager::CreateVAO() {
    GPU_VertexArrayObject vao;
    GPURES_ID id = m_vaoStorage.AddResource(std::move(vao));
    return { id, GPUDatatype::VAO };
}

GPUResourceHandle GPUResourceManager::CreateMesh(MeshRes& _mesh) {
    GPU_Mesh mesh;
    mesh.Load(_mesh);
    GPURES_ID id = m_meshStorage.AddResource(std::move(mesh));
    return { id, GPUDatatype::Mesh };
}

bool GPUResourceManager::DeleteResource(GPUResourceHandle _handle) {
    GPURES_ID id = _handle.m_id;
    GPUDatatype type = _handle.m_type;
    switch (type) {
    case GPUDatatype::Texture: 
        return DeleteResourceInternal(m_textureStorage, _handle);
    case GPUDatatype::Buffer:
        return DeleteResourceInternal(m_bufferStorage, _handle);
    case GPUDatatype::VAO:
        return DeleteResourceInternal(m_vaoStorage, _handle);
    case GPUDatatype::Mesh:
        return DeleteResourceInternal(m_meshStorage, _handle);
    case GPUDatatype::IndexBuffer:
        break;
    case GPUDatatype::FrameBuffer:
        break;
    }
    return false;
}

void GPUResourceManager::Cleanup() {
    ClearAll();
}

void GPUResourceManager::ClearAll() {
    ClearGPUResourceStorage(m_bufferStorage);
    ClearGPUResourceStorage(m_textureStorage);
    ClearGPUResourceStorage(m_vaoStorage);
    ClearGPUResourceStorage(m_meshStorage);
}

void GPUResourceManager::ClearGPUBuffers() {
    ClearGPUResourceStorage(m_bufferStorage);
}

void GPUResourceManager::ClearTextures() {
    ClearGPUResourceStorage(m_textureStorage);
}

void GPUResourceManager::ClearVAOs() {
    ClearGPUResourceStorage(m_vaoStorage);
}

void GPUResourceManager::ClearMeshes() {
    ClearGPUResourceStorage(m_meshStorage);
}