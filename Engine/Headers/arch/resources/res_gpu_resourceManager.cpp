#include "res_gpu_resourceManager.h"

GPUResourceHandle GPUResourceManager::CreateGPUBuffer() {
    GPURES_ID id = GenerateResID();
    m_bufferStorage.Add(std::move(GPU_Buffer()), id);
    return { id, GPUDatatype::Buffer };
}

GPUResourceHandle GPUResourceManager::CreateTexture(
    TextureProperties::TextureType _type,
    glm::ivec3 _dims,
    TextureProperties::TextureProps _props
) {
    GPURES_ID id = GenerateResID();
    GPU_Texture tex(_type, _dims, _props);
    tex.Create();
    tex.Allocate();
    m_textureStorage.Add(std::move(tex), id);
    return { id, GPUDatatype::Texture };
}



GPUResourceHandle GPUResourceManager::CreateVAO() {
    GPURES_ID id = GenerateResID();
    m_vaoStorage.Add(GPU_VertexArrayObject(), id);
    return { id, GPUDatatype::VAO };
}

bool GPUResourceManager::DeleteResource(GPUResourceHandle _handle) {
    GPURES_ID id = _handle.m_id;
    GPUDatatype type = _handle.m_type;
    switch (type) {
    case GPUDatatype::Texture: {
        return DeleteTexture(_handle);
    }
    case GPUDatatype::Buffer:
        return DeleteGPUBuffer(_handle);
    case GPUDatatype::VAO:
        return DeleteVAO(_handle);

    case GPUDatatype::IndexBuffer:
        break;
    case GPUDatatype::FrameBuffer:
        break;
        //return m_textureStorage.Remove(id);
    }
    return false;
}

void GPUResourceManager::ClearAll() {
    ClearGPUResStorage(m_bufferStorage);
    //ClearGPUResStorage(m_textureStorage);
    ClearGPUResStorage(m_vaoStorage);
}


bool GPUResourceManager::DeleteTexture(GPUResourceHandle _handle) {
    auto handle = m_textureStorage.At(_handle.m_id);
    if (!handle) return false;
    handle->Destroy();
    return m_textureStorage.Remove(_handle.m_id);
}

bool GPUResourceManager::DeleteGPUBuffer(GPUResourceHandle _handle) {
    auto handle = m_bufferStorage.At(_handle.m_id);
    if (!handle) return false;
    handle->Destroy();
    return m_bufferStorage.Remove(_handle.m_id);
}

bool GPUResourceManager::DeleteVAO(GPUResourceHandle _handle) {
    auto handle = m_vaoStorage.At(_handle.m_id);
    if (!handle) return false;
    handle->Destroy();
    return m_vaoStorage.Remove(_handle.m_id);
}

GPURES_ID GPUResourceManager::GenerateResID() {
    return GPURES_ID();
}
