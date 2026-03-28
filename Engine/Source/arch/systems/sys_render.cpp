#include <pch.h>
#include <arch/systems/sys_render.h>
// ecs
#include <arch/core.h>
#include <arch/ecs/ecs_fwdDecl_entityRegistry.h>
// components
#include <arch/components/comp_meshrenderer.h>
#include <arch/resources/res_mesh_presets/res_mesh_cube.h>
#include <arch/components/comp_camera.h>
#include <arch/components/comp_transform.h>
#include <util/util_logging.h>
#include <util/util_graphics_debugging.h>

// Some other todos - make a shader editor! How hard can it be? :')



static const char* GLDebugTypeToString(GLenum type) {
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:               return "ERROR";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
    case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
    case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
    case GL_DEBUG_TYPE_MARKER:              return "MARKER";
    case GL_DEBUG_TYPE_PUSH_GROUP:          return "PUSH_GROUP";
    case GL_DEBUG_TYPE_POP_GROUP:           return "POP_GROUP";
    case GL_DEBUG_TYPE_OTHER:               return "OTHER";
    default:                                return "UNKNOWN";
    }
}

static const char* GLDebugSeverityToString(GLenum severity) {
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
    case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
    case GL_DEBUG_SEVERITY_LOW:          return "LOW";
    case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
    default:                             return "UNKNOWN";
    }
}

void GLAPIENTRY MessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    std::string msg(message);

    // Messages we want to ignore
    static const std::set<std::string> blackListSet = {
        "<location> is invalid",
        "Buffer detailed info",
        "Rasterization usage warning:"
    };
    for (const std::string& str : blackListSet) {
        if (msg.find(str) != std::string::npos) return;
    }

    // Convert type enum to string
    const char* typeStr = "UNKNOWN";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:              typeStr = "ERROR"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:typeStr = "DEPRECATED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = "UNDEFINED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_PORTABILITY:        typeStr = "PORTABILITY"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:        typeStr = "PERFORMANCE"; break;
    case GL_DEBUG_TYPE_MARKER:             typeStr = "MARKER"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:         typeStr = "PUSH_GROUP"; break;
    case GL_DEBUG_TYPE_POP_GROUP:          typeStr = "POP_GROUP"; break;
    case GL_DEBUG_TYPE_OTHER:              typeStr = "OTHER"; break;
    }

    // Convert severity enum to string
    const char* severityStr = "UNKNOWN";
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:         severityStr = "HIGH"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       severityStr = "MEDIUM"; break;
    case GL_DEBUG_SEVERITY_LOW:          severityStr = "LOW"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "NOTIFICATION"; break;
    }

    // Choose appropriate log macro
    if (type == GL_DEBUG_TYPE_ERROR) {
        LOG_ERROR("GL CALLBACK: " << message
            << " (type=" << typeStr
            << ", severity=" << severityStr
            << ", id=" << id << ")");
    }
    else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR ||
        type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR ||
        type == GL_DEBUG_TYPE_PORTABILITY ||
        type == GL_DEBUG_TYPE_PERFORMANCE) {
        LOG_WARN("GL CALLBACK: " << message
            << " (type=" << typeStr
            << ", severity=" << severityStr
            << ", id=" << id << ")");
    }
    else { // OTHER, MARKER, PUSH_GROUP, POP_GROUP
        LOG_INFO("GL CALLBACK: " << message
            << " (type=" << typeStr
            << ", severity=" << severityStr
            << ", id=" << id << ")");
    }
}





void SetupGLDebug() {
    GLint flags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        LOG_INFO("GEN");
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // easier for debugging (makes callback synchronous)
        glDebugMessageCallback(MessageCallback, nullptr);
        // Optional: control which messages come through
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    else {
        LOG_WARN("OpenGL context not created with debug flag; debug messages may be limited.");
    }
}



void RenderSystem::Init() {

    // - initialise gl settings -----------------
    glEnable(GL_DEPTH_TEST);
    // - init viewport manager ------------------

    Viewport::ViewportID vpId	{ m_viewportManager.CreateViewport() };
    Viewport& viewport			{ *m_viewportManager.GetViewport(vpId)	};
    RenderTargetManager::RenderTargetID rtId{ m_renderTargetManager.AddRenderTarget("Viewport",
        viewport.ViewportDimensions()
        ) };
    RenderTarget& renderTarget  { *m_renderTargetManager.GetRenderTarget(rtId) };
    std::shared_ptr<RenderTarget> rt{ std::make_shared<RenderTarget>(renderTarget) };
    rt->AddInitialColorAttachments();
    rt->Build();
    viewport.SetRenderTarget(rt);


    SetupGLDebug();
    m_uboManager.Init();
    m_uboManager.CreateUBO(DefaultUBOs::DEFAULTBUFFER_COMMON, 0, sizeof(CommonUBOData));
    m_uboManager.CreateUBO(DefaultUBOs::DEFAULTBUFFER_OBJECT, 1, sizeof(ObjectUBOData));
    m_uboManager.CreateUBO(DefaultUBOs::DEFAULTBUFFER_LIGHTS, 2, sizeof(LightUBOData));
    m_uboManager.CreateUBO(DefaultUBOs::DEFAULTBUFFER_SHADOW, 3, sizeof(ShadowMapUBOData));
    m_vaoManager.Init();
    m_compositor.Init();

    // - shadows -------------------------------
    SetupShadowProgram();
    SetupShadowBuffers();

}
 
void RenderSystem::PreUpdate() {
    // clear the buffer.



}


void RenderSystem::Update() {
    /*
        disable
        clear screen
        
        
        for each viewport

        // - viewport setup ----------------------------
        get the size of each viewport 
        get the position of each viewport 

        // - render setup ------------------------------
        get the camera matrix
        get the projection matrix
        render();
    */

    

    const std::vector<Viewport::ViewportID>& vpRenderOrder	{ m_viewportManager.ViewportRenderOrderList() };
    auto& viewportMap					{ m_viewportManager.ViewportList() };



    for (const Viewport::ViewportID& id : vpRenderOrder) {
        Viewport& currentViewport	{ *viewportMap.at(id ) };
        currentViewport.Update();
        Render(currentViewport); // replace with a single viewport.
    }
}





void RenderSystem::Render(const Viewport& _viewport) {
    const glm::vec2 vpDims{ _viewport.ViewportDimensions() };
    if (!vpDims.x || !vpDims.y) return;
    BeginViewportPass(_viewport);

    auto GetError = GraphicsDebug::GetError;
    EntityRegistry& registry = Core::GetInstance().GetRegistry();

    const std::vector<Light*> culledLights          { CullLights(_viewport, registry) };
    UpdateLightingData(culledLights, registry);

    FillCommonUBO(
        glm::inverse(_viewport.CameraMatrix()),
        _viewport.ProjectionMatrix(),
        _viewport.Position(),
        _viewport.Forward(),
        static_cast<GLfloat>(Clock::DeltaTime())
    );
    ShadowRenderPass(_viewport, registry);

    const std::vector<LightData> lightData          { GetLightData(culledLights) };
    const std::vector<ShadowData> shadowData        { GetShadowData(culledLights) };
    FillLightBufferUBO(lightData);
    FillShadowMapUBO(shadowData);


    if (_viewport.GetRenderTarget()) _viewport.GetRenderTarget()->Bind();
    LightingRenderPass(_viewport, registry);
    UBO::UnbindBuffer(); 
    

    EndViewportPass(_viewport);

}

void RenderSystem::ClearBuffers(const Viewport& _vp) {
    GLuint clearFlags { GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT };
    glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
    glClearDepth(1.0f);
    glClear(clearFlags);
}

void RenderSystem::UseViewport(const Viewport& _viewport) {
    std::shared_ptr<const RenderTarget> rtPtr{ _viewport.GetRenderTarget() };
    if (!rtPtr) return;
      
    rtPtr->Bind();
    glm::vec2 vpDims{ _viewport.ViewportDimensions() };
    glViewport(0, 0, static_cast<GLsizei>(vpDims.x), static_cast<GLsizei>(vpDims.y));
    glPolygonMode(_viewport.GetFaceToRender(), _viewport.GetRenderMode());
}

void RenderSystem::BeginViewportPass(const Viewport& _viewport) {
    UseViewport(_viewport);
    ClearBuffers(_viewport);
}

void RenderSystem::EndViewportPass(const Viewport& _viewport) {
    UnbindViewport(_viewport);
}

void RenderSystem::SetupRenderSettings(const Viewport& _viewport) {
    GLuint clearFlags{ GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT };
    glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
    glClearDepth(1.0f);
    glClearStencil(0x00);
    glClear(clearFlags);
    glStencilMask(0xff);

    GLuint clearID = 0;

    // viewport set.
    glClearBufferuiv(GL_COLOR, 1, &clearID);

}

void RenderSystem::RenderRangedLightShadows(
    const Light& light,
    const Transform& lightTransform,
    const EntityRegistry& entityRegistry,
    const ComponentPool<MeshRenderer>& meshPool
) {
    for (unsigned level{}; level < m_directionalShadowMaps.GetLODLevels(); ++level) {
        for (const MeshRenderer& mr : meshPool.Data()) {
            if (!mr.GetCastShadow()) continue;
            const auto meshEntity{ entityRegistry.GetEntity(mr.GetEntityID()) };
            if (!meshEntity->Active() || !meshEntity->IsVisible()) continue;

            std::shared_ptr<const Mesh> mesh = mr.GetMesh();
            if (!mesh) continue;
            VAOHandler* vaoHandler{ m_vaoManager.GetVAO(mesh->VAOIdentifier()) };
            if (!vaoHandler) continue;
            VAOHandler& currentVAO = *vaoHandler;
            currentVAO.BindVAO();
            currentVAO.UseMesh(*mesh);

            // do something.
            auto trsMesh = meshEntity->GetComponent<Transform>();
            const glm::mat4 objectTransformMatrix = trsMesh->WorldTransformMtx();


            //PassLightingMatrices(objectTransformMatrix, light.GetLightData().m_matrix);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->GetIndexDataCount() * 3), GL_UNSIGNED_INT, 0);
        }
    }
}

void RenderSystem::RenderPointLightShadows(
    const Light& light,
    const Transform& lightTransform,
    const EntityRegistry& entityRegistry,
    const ComponentPool<MeshRenderer>& meshPool
) {
    for (unsigned sides{}; sides < m_directionalShadowMaps.GetLODLevels(); ++sides) {
        // change perspective 
        for (const MeshRenderer& mr : meshPool.Data()) {
            const auto meshEntity{ entityRegistry.GetEntity(mr.GetEntityID()) };
            if (!meshEntity->Active() || !meshEntity->IsVisible()) continue;

            std::shared_ptr<const Mesh> mesh = mr.GetMesh();
            if (!mesh) continue;
            VAOHandler* vaoHandler{ m_vaoManager.GetVAO(mesh->VAOIdentifier()) };
            if (!vaoHandler) continue;
            VAOHandler& currentVAO = *vaoHandler;
            currentVAO.BindVAO();
            currentVAO.UseMesh(*mesh);

            // do something.
            auto trsMesh = meshEntity->GetComponent<Transform>();
            const glm::mat4 objectTransformMatrix = trsMesh->WorldTransformMtx();
            //PassLightingMatrices(objectTransformMatrix, light.GetLightData().m_matrix);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->GetIndexDataCount() * 3), GL_UNSIGNED_INT, 0);
        }
    }

}

void RenderSystem::FillLightBufferUBO(const std::vector<LightData>& _culledLightList) {
    m_ldData.m_count = std::min(C_MAX_LIGHTS, static_cast<unsigned>(_culledLightList.size()));
    for (size_t i = 0; i < m_ldData.m_count; ++i) {
        m_ldData.m_lightData[i] = _culledLightList[i]; // or whatever data source
    }

    UBO& lightBuffer = *m_uboManager.GetUBO(DefaultUBOs::DEFAULTBUFFER_LIGHTS);
    lightBuffer.BindBuffer();
    lightBuffer.FillBufferData(&m_ldData);
}

void RenderSystem::FillShadowMapUBO(const std::vector<ShadowData>& _shadowDataList) {
    int shadowCount{ std::min(static_cast<int>(_shadowDataList.size()), C_MAX_SHADOWS) };
    // - directional light data -----------------------------------------------------
    for (int i{}; i < shadowCount; ++i) {
        if (_shadowDataList[i].GetLightType() != LightType::DIRECTIONAL) continue;
        m_smData.m_shadowData[i] = _shadowDataList[i];
    }
    glm::vec2 dirLightFBSize{ static_cast<glm::vec2>(m_directionalShadowMaps.GetFramebufferSize()) };
    m_smData.m_directionalAtlasResAndTexelSize = glm::vec4(
        dirLightFBSize.x,
        dirLightFBSize.y, 
        1.0f / dirLightFBSize.x,
        1.0f / dirLightFBSize.y
        );
    // - point light data -----------------------------------------------------------
    UBO& shadowUBO = *m_uboManager.GetUBO(DefaultUBOs::DEFAULTBUFFER_SHADOW);
    shadowUBO.BindBuffer();
    shadowUBO.FillBufferData(&m_smData);
}

void RenderSystem::FillCommonUBO( 
    const glm::mat4& _cameraMatrix, 
    const glm::mat4& _projectionMatrix, 
    const glm::vec3& _cameraPosition, 
    const glm::vec3& _cameraForward, 
    const GLfloat& _deltaTime
) {
    UBO& commonUBO = *m_uboManager.GetUBO(DefaultUBOs::DEFAULTBUFFER_COMMON);
    m_commonUboData.m_cameraMatrix = _cameraMatrix;
    m_commonUboData.m_projectionMatrix = _projectionMatrix;
    m_commonUboData.m_cameraPosition = _cameraPosition;
    m_commonUboData.m_cameraForward = _cameraForward;
    m_commonUboData.m_deltaTime = _deltaTime;
    commonUBO.BindBuffer();
    commonUBO.FillBufferData(&m_commonUboData);
}

void RenderSystem::FillObjectUBO(const Entity& entity, const Transform& _trs) {
    UBO& objectUBO = *m_uboManager.GetUBO(DefaultUBOs::DEFAULTBUFFER_OBJECT);
    m_objectUboData.m_objectMatrix = _trs.WorldTransformMtx();
    m_objectUboData.m_position = _trs.Position();
    m_objectUboData.m_objectId = static_cast<GLuint>(entity.GetID().GetID());
    objectUBO.BindBuffer();
    objectUBO.FillBufferData(&m_objectUboData);
}

void RenderSystem::UnbindViewport(const Viewport& _viewport) {
    if (_viewport.GetRenderTarget()) {
        _viewport.GetRenderTarget()->Unbind();
    }
}

void RenderSystem::ShadowRenderPass(
    const Viewport& _viewport,
    const EntityRegistry& _er
) {

    const ComponentPool<Light>& lightPool { *_er.GetComponentPool<Light>() };
    const ComponentPool<MeshRenderer>& mrPool { *_er.GetComponentPool<MeshRenderer>() };
    glm::ivec2 tileSize = m_directionalShadowMaps.GetBaseTileSize();
    glm::ivec2 fbSize = m_directionalShadowMaps.GetFramebufferSize();
    

    if (!m_directionalShadowMaps.FBO() || !m_directionalShadowMaps.GetTextureID()) {
        LOG_ERROR("No shadow map bound to this light!");
        return;
    }

    // in the shadow pass, all meshes use the SAME material unless it has transparency or some SS nonsense. 
    BindShadowShader();
    // sort and batch render by light types.
    std::array<std::vector<const Light*>, 3> lightBuckets;
    for (std::vector<const Light*> bucket : lightBuckets) {
        bucket.reserve(lightPool.size());
    }

    for (const Light& light : lightPool.Data()) {
        const auto lightEntity{ _er.GetEntity(light.GetEntityID()) };
        if (!lightEntity->Active() || !lightEntity->IsVisible()) continue;
        int id = light.Type() == POINT ? 0 : light.Type() == SPOT ? 1 : light.Type() == DIRECTIONAL ? 2 : -1;
        if (id < 0) continue;
        lightBuckets[id].push_back(&light);
    }


    // rendering shadows.
    using ShadowRenderFunc = std::function<
        void(const Viewport&, const EntityRegistry&, const Light&, const ComponentPool<MeshRenderer>&)
    >;
    for (int i{}; i < 3; ++i) {
        ShadowMap* currentShadowMap{};
        ShadowRenderFunc shadowPassFunction;
        switch (i) {
        case 0:
            currentShadowMap = &m_pointLightShadowMaps;
            shadowPassFunction = [this](
                const Viewport& vp, 
                const EntityRegistry& er, 
                const Light& l, 
                const ComponentPool<MeshRenderer>& mr
                ) {
                    this->PointLightShadowRenderPass(vp, er, l, mr);
                };
            break;
        case 1:
            currentShadowMap = &m_spotLightShadowMaps;
            shadowPassFunction = [this](
                const Viewport& vp,
                const EntityRegistry& er,
                const Light& l,
                const ComponentPool<MeshRenderer>& mr
                ) {
                    this->SpotLightShadowRenderPass(vp, er, l, mr);
                };
            break;
        case 2:
            currentShadowMap = &m_directionalShadowMaps;
            shadowPassFunction = [this](
                const Viewport& vp,
                const EntityRegistry& er,
                const Light& l,
                const ComponentPool<MeshRenderer>& mr
                ) {
                    this->DirectionalLightShadowRenderPass(vp, er, l, mr);
                };
            break;
        }

        if (!currentShadowMap || !currentShadowMap->IsBuilt() || !shadowPassFunction) continue;

        currentShadowMap->Bind();
        for (const Light* light : lightBuckets[i]) {
            shadowPassFunction(_viewport, _er, *light, mrPool);
        }
    }
    ShadowMap::Unbind();
    UnbindShadowShader();
}

void RenderSystem::LightingRenderPass(
    const Viewport& _viewport,
    const EntityRegistry& _er
) {
    // - prereqs --------------------------------------------
    const glm::mat4& _cameraMatrix{ glm::inverse(_viewport.CameraMatrix()) };
    const glm::mat4& _projectionMatrix{ _viewport.ProjectionMatrix() };;

    EntityRegistry& registry = Core::GetInstance().GetRegistry();
    auto& entityList = registry.GetEntityList();
    auto& selectedEntityList = registry.SelectedEntities();
        
    _er.GetComponentPool<MeshRenderer>();


    for (const Entity& e : _er.GetEntityList()) {
        if (!e.Active() || !e.IsVisible()) {
            // to skip hidden ones as well.
            continue;
        }

        const auto& mr = e.GetComponent<MeshRenderer>();
        if (!mr) continue;

         
        auto trs = e.GetComponent<Transform>();
        const glm::mat4 objectTransformMatrix = trs->LocalTransformMtx();

        std::shared_ptr<Mesh> mesh = mr->GetMesh();
        if (!mesh) continue;
        VAOHandler* vaoHandler{ m_vaoManager.GetVAO(mesh->VAOIdentifier()) };

        if (!vaoHandler) continue;

        bool isSelected{ _er.EntityIsSelected(e.GetID()) };

        VAOHandler& currentVAO = *vaoHandler;
        currentVAO.BindVAO();
        //currentVAO.LogDebug();
        currentVAO.UseMesh(*mesh);
        // check if there is data here...
        FillObjectUBO(e, *trs);

        ResolveMeshRendererMaterials(*mr);
        mr->ApplyShadowMap(m_directionalShadowMaps);
        mr->Render();

        Render(*mr);

        m_vaoManager.UnbindVAO();
        
    }

    glBindVertexArray(0); 

} 

void RenderSystem::DirectionalLightShadowRenderPass(
    const Viewport& _viewport, 
    const EntityRegistry& _er, 
    const Light& _light,
    const ComponentPool<MeshRenderer>& _mrPool
) {
    const EntityViewConst& lightEntity  { _er.GetEntity(_light.GetEntityID()) };
    const ComponentView<Transform>& trs { lightEntity->GetComponent<Transform>() };
    const glm::vec2& fbSize             { m_directionalShadowMaps.GetFramebufferSize() };
    ShadowData& sdData                  { const_cast<Light*>(&_light)->GetShadowData() };

    

    // generate light data.
    if (_light.CastShadowDirty()) {
        bool wantsShadowId{ _light.GetCastShadow() };
        if (!wantsShadowId) {
            m_directionalShadowMaps.ReclaimID(_light.GetShadowMapID());
            _light.InvalidateShadowMapID();
            LOG_INFO("clearing light id.");
            --m_smData.m_directionalCount;
        }
        else {
            if (!m_directionalShadowMaps.HasFreeLayers()) {
                LOG_INFO("No free slots for shadow map. Waiting for next update.");
                // early exit, cannot update the var.
                return;
            }
            LOG_INFO("Assigning new id to light.");
            _light.SetShadowMapID(m_directionalShadowMaps.GenerateLayerID());
            ++m_smData.m_directionalCount;
        }
        _light.CleanCastShadow();
    }
    if (!_light.GetCastShadow()) return; // ignore anything not asking for it.
    unsigned shadowMapID{ _light.GetShadowMapID() };
    if (!m_directionalShadowMaps.ValidateID(shadowMapID)) return;
    m_directionalShadowMaps.SetBoundLayer(shadowMapID);

    // clear.
    glViewport(0, 0, static_cast<GLsizei>(fbSize.x), static_cast<GLsizei>(fbSize.y));
    glScissor(0, 0, static_cast<GLsizei>(fbSize.x), static_cast<GLsizei>(fbSize.y));
    glClear(GL_DEPTH_BUFFER_BIT);

    // convert light to matrix.
    glm::vec3 lightDir      { glm::normalize(trs->Forward()) };
    const glm::vec3& camPos        { _viewport.Position() };
    glm::vec3 lightPos = camPos - lightDir * 20.0f;

    glm::mat4 lightView = glm::lookAt(
        lightPos,
        camPos,
        glm::vec3(0, 1, 0)
    );

    float currentYOffset = 0;
    float halfLenIncrement = 50.f;
    for (unsigned level{}; level < m_directionalShadowMaps.GetLODLevels(); ++level) {
        glm::ivec2 offset{ (level % 2), (level / 2) };
        // every level generate a new ortho matrix

        float halfLength { halfLenIncrement * (level + 1) };
        glm::mat4 lightProj = glm::ortho(
            -halfLength, halfLength,
            -halfLength, halfLength,
            0.1f, 2 * halfLength
        );

        // calculate light space matrix.
        glm::mat4 lightSpaceMtx { lightProj * lightView }; 
        // textures must be in multiples of 2.
        int tileSizeX = m_directionalShadowMaps.GetBaseTileSize().x >> level;
        int tileSizeY = m_directionalShadowMaps.GetBaseTileSize().y >> level;
        m_directionalShadowMaps.GetBaseTileSize();
        GLint yOffset{ static_cast<GLint>(currentYOffset) };
        glViewport(0, yOffset, tileSizeX, tileSizeY);
        glScissor(0, yOffset, tileSizeX, tileSizeY);
     
        // - setting shadow data -------------------------------------------------------
        sdData.SetAtlasOffset(glm::vec2(0, currentYOffset), level);
        sdData.SetAtlasSize(glm::vec2(tileSizeX, tileSizeY), level);
        sdData.SetMatrix(lightSpaceMtx, level);

        currentYOffset += tileSizeY;

        for (const MeshRenderer& mr : _mrPool.Data()) {
            const auto meshEntity{ _er.GetEntity(mr.GetEntityID()) };
            if (!meshEntity->Active() || !meshEntity->IsVisible() || !mr.GetCastShadow()) continue;

            std::shared_ptr<const Mesh> mesh = mr.GetMesh();
            if (!mesh) continue;
            VAOHandler* vaoHandler{ m_vaoManager.GetVAO(mesh->VAOIdentifier()) };
            if (!vaoHandler) continue;
            VAOHandler& currentVAO = *vaoHandler;
            currentVAO.BindVAO();
            currentVAO.UseMesh(*mesh);



            // do something.
            auto trsMesh = meshEntity->GetComponent<Transform>();
            const glm::mat4 objectTransformMatrix = trsMesh->WorldTransformMtx();
            PassLightingMatrices(objectTransformMatrix, lightSpaceMtx);
            FillObjectUBO(*meshEntity, *trsMesh);
            glDrawElements(GL_TRIANGLES, mesh->GetIndexDataCount() * 3, GL_UNSIGNED_INT, 0);
        }
    }
    

    // once done pass this to the shadow UBO.
}

void RenderSystem::PointLightShadowRenderPass(
    const Viewport& _viewport, 
    const EntityRegistry& _er, 
    const Light& _light, 
    const ComponentPool<MeshRenderer>& _mrPool
) {

}

void RenderSystem::SpotLightShadowRenderPass(
    const Viewport& _viewport, 
    const EntityRegistry& _er, 
    const Light& _light, 
    const ComponentPool<MeshRenderer>& _mrPool
) {

}

void RenderSystem::Render(const MeshRenderer& _mr) const {
    std::shared_ptr<const Mesh> mesh{ _mr.GetMesh() };
    if (!mesh) return;
    GLsizei meshFloatCount{ static_cast<GLsizei>(mesh->GetIndexDataCount() * 3) };
    const std::vector<std::shared_ptr<Material>> matList    { _mr.GetMaterialList() };
    // use the default material and render.
    if (matList.size() == 0) {
        //const Material& mat = GetDefaultMaterial();
        //mat.ApplyUniforms();
        //glDrawElements(GL_TRIANGLES, meshFloatCount, GL_UNSIGNED_INT, 0);
        return;
    }
    // go through all materials
    for (const std::shared_ptr<Material> matPtr : matList) {
        matPtr->ApplyUniforms();
        glDrawElements(GL_TRIANGLES, meshFloatCount, GL_UNSIGNED_INT, 0);
    }
}

void RenderSystem::DebugRenderPass(const unsigned& textureId) {
    glDisable(GL_DEPTH_TEST);
    glUseProgram(m_planeShader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
    glUniform1i(glGetUniformLocation(m_planeShader, "u_texture"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

// ------------------------------------------------------------------------------------

Viewport::RENDERMODE RenderSystem::GetRenderMode() const {
    return m_renderMode;
};
void RenderSystem::SetRenderMode(Viewport::RENDERMODE _renderMode) {
    m_renderMode = _renderMode;
}
Viewport::FACETORENDER RenderSystem::GetFaceRenderedDirection() const {
    return m_facesToRender;
}
void RenderSystem::SetFaceRenderedDirection(Viewport::FACETORENDER _setting) {
    m_facesToRender = _setting;
}

// ------------------------------------------------------------------------------------


ViewportManager& RenderSystem::GetViewportManager() {
    return m_viewportManager;
}
const ViewportManager& RenderSystem::GetViewportManager() const {
    return m_viewportManager;
}


RenderTargetManager& RenderSystem::GetRenderTargetManager() {
    return m_renderTargetManager;
}
const RenderTargetManager& RenderSystem::GetRenderTargetManager() const {
    return m_renderTargetManager;
}

Compositor& RenderSystem::GetCompositor() {
    return m_compositor;
}
const Compositor& RenderSystem::GetCompositor() const {
    return m_compositor;
}




const std::vector<Light*> RenderSystem::CullLights(
    const Viewport& _viewport,
    EntityRegistry& _er
) {

    std::vector<Light*> potentialLights{};


    if (!_er.ComponentPoolExists<Light>()) {
        LOG_WARN("Light Component Pool does not exist");
        return potentialLights;
    }

    SparseSetView<ComponentPool<Light>> lightPoolRef    { _er.GetComponentPool<Light>() };
    ComponentPool<Light>& lightPool                     { *lightPoolRef };
    const unsigned lightCount = m_maxLightCount;
    SparseSet<EntityID, Light>& lightComponentData = lightPool.Data();

    potentialLights.reserve(lightPool.size());
    for (Light& light : lightComponentData) {

        EntityView entity  { _er.GetEntity(light.GetEntityID()) };
        if (!entity || !entity->Active()) continue;
        if (!LightCollisionTest(light, _viewport)) {
            // light does not appear or cause an effect in the camera frustum.
            continue;
        }
        potentialLights.push_back(&light);
    }
    return potentialLights;
}

void RenderSystem::UpdateLightingData(
    const std::vector<Light*> lightList, 
    const EntityRegistry& er
) {


    for (Light* light : lightList) {
        const Transform& trs{ *er.GetEntity(light->GetEntityID())->GetComponent<Transform>() };
        LightData& ld { light->GetLightData() };
        ShadowData& sd { light->GetShadowData() };
        ld.SetPosition(trs.Position());
        ld.SetDirection(trs.Forward());


        sd.SetShadowID(light->GetShadowMapID());
        sd.SetLightType(light->Type());
    }
}

std::vector<LightData> RenderSystem::GetLightData(const std::vector<Light*>& lightList) const {
    std::vector<LightData> lightData;
    lightData.reserve(lightList.size());

    for (const Light* light : lightList) {
        lightData.push_back(light->GetLightData());

        //ComponentView<Transform> trs = entity->GetComponent<Transform>();

        //LightData lightData = light.GetLightData();
        //glm::vec3 position = trs->Position();
        //lightData.SetPosition(position);
        //// direction of 0, -1, 0, * quat.
        //lightData.SetDirection(trs->Forward());
        //potentialLights.push_back(lightData);
    }
    lightData.shrink_to_fit();
    return lightData;
}

std::vector<ShadowData> RenderSystem::GetShadowData(const std::vector<Light*>& lightList) const {
    std::vector<ShadowData> shadowData;
    shadowData.reserve(lightList.size());
    for (const Light* light : lightList) {
        if (light->GetCastShadow()) {
            shadowData.push_back(light->GetShadowData());
        }
    }


    return shadowData;
}



bool RenderSystem::LightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const {
    bool testCase = false;
    switch (_lightComponent.Type()){
    case POINT:
        testCase = PointLightCollisionTest(_lightComponent, _viewport);
        break;
    case SPOT:
        testCase = SpotLightCollisionTest(_lightComponent, _viewport);
        break;
    case DIRECTIONAL:
        testCase = true;
        break;
    case AMBIENT:
        testCase = true;
        break;
    default:
        break;
    }
    return testCase;
}

bool RenderSystem::SpotLightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const {
    (void)_lightComponent;
    return true;
} 
 
bool RenderSystem::PointLightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const {
    (void)_lightComponent;
    return true;
}

void RenderSystem::SetupShadowProgram() {
    std::string vertexShaderSource = "#version 460 core\n" + ShaderUtilFunctions::ParseShaderCode("./Assets/Shaders/vtx_shadowPassVertex.vert");
    std::string fragmentShaderSource = "#version 460 core\n" + ShaderUtilFunctions::ParseShaderCode("./Assets/Shaders/frag_shadowPassFrag.frag");

    ShaderProgram shadowShader  {};
    Shader vertexShader         {};
    Shader fragmentShader       {};
    vertexShader.SetShaderCode(vertexShaderSource);
    fragmentShader.SetShaderCode(fragmentShaderSource);
    vertexShader.SetShaderType(ShaderConstants::ShaderType::VERTEX);
    fragmentShader.SetShaderType(ShaderConstants::ShaderType::FRAG);
    vertexShader.Build();
    fragmentShader.Build();
    shadowShader.SetShader(vertexShader, ShaderConstants::ShaderType::VERTEX);
    shadowShader.SetShader(fragmentShader, ShaderConstants::ShaderType::FRAG);

    
    
    shadowShader.Build();
    
    m_shadowPrg = shadowShader.GetShaderProgramID();
    m_shadowMeshLoc = glGetUniformLocation(m_shadowPrg, U_OBJECT_MATRIX);
    m_shadowLightLoc = glGetUniformLocation(m_shadowPrg, U_LIGHT_MATRIX);
    LOG_DEBUG("Setting up shadow shader with program id: ["<< m_shadowPrg << "] with mesh uniform location of <"<< m_shadowMeshLoc << "> and light uniform loc of <"<< m_shadowLightLoc<<">");
}

void RenderSystem::SetupShadowBuffers() {
    const unsigned SHADOW_WH    { 4096 };
    
    m_directionalShadowMaps.SetFramebufferSize({ SHADOW_WH, SHADOW_WH });
    m_directionalShadowMaps.SetShadowMapType(ShadowMapType::TEXTURE);
    m_directionalShadowMaps.BuildShadowMap(); 

    m_spotLightShadowMaps.SetFramebufferSize({ SHADOW_WH, SHADOW_WH });
    m_spotLightShadowMaps.SetShadowMapType(ShadowMapType::TEXTURE);
    m_spotLightShadowMaps.BuildShadowMap();

    m_pointLightShadowMaps.SetFramebufferSize({ SHADOW_WH, SHADOW_WH });
    m_pointLightShadowMaps.SetShadowMapType(ShadowMapType::CUBEMAP);
    m_pointLightShadowMaps.BuildShadowMap(); 
}

void RenderSystem::PassLightingMatrices(glm::mat4 _meshMatrix, glm::mat4 _lightMatrix) {
    glUniformMatrix4fv(m_shadowMeshLoc, 1, GL_FALSE, glm::value_ptr(_meshMatrix));
    glUniformMatrix4fv(m_shadowLightLoc, 1, GL_FALSE, glm::value_ptr(_lightMatrix));
}

void RenderSystem::BindShadowShader() {
    glUseProgram(m_shadowPrg); 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void RenderSystem::UnbindShadowShader() {
    glUseProgram(0);
}

void RenderSystem::ResolveMeshRendererMaterials(MeshRenderer& _mr) {
    if (!_mr.GetMesh()) return; // no point resolving something can't be seen
    for (std::shared_ptr<Material>& matPtr : _mr.GetMaterialList()) {
        Material& mat{ *matPtr };
        ResolveDefaultMaterial(mat);
    }
}

void RenderSystem::ResolveDefaultMaterial(Material& _mat) {
    ShaderManager& sr{ Core::GetInstance().GetShaderManager() };
    using namespace Materials;
    ShadingModel type{ _mat.GetShadingModel() };
    if (
        _mat.GetShaderProgram() != ShaderConstants::C_INVALIDSHADERID ||
        type == Materials::ShadingModel::NONE
        ) return;
    std::string shaderProgramAlias{};
    switch (type) {
    case ShadingModel::LAMBERT:
        shaderProgramAlias = ShaderConstants::C_ID_LAMBERTSHADERPROG;
        break;
    case ShadingModel::PHONG:
        shaderProgramAlias = ShaderConstants::C_ID_LAMBERTFRAGSHADER;
        break;
    case ShadingModel::BLINN_PHONG:
        break;
    case ShadingModel::COOK_TORRENCE:
        break;
    case ShadingModel::GGX:
        break;
    case ShadingModel::BURLEY:
        break;
    case ShadingModel::PRINCIPLED:
        break;
    default:
        break;
    }
    if (!shaderProgramAlias.empty()) {
        GLuint shaderId{ ShaderConstants::C_INVALIDSHADERID };
        shaderId = sr.GetShaderProgram(shaderProgramAlias)->GetShaderProgramID();
        LOG_INFO("Shader ID: " << shaderId);
        _mat.SetShaderProgram(shaderId);
        _mat.Init();
        _mat.ApplyUniforms();
    }

}


