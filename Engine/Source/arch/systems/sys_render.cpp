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






// ---------------------------------------------------------------------------------------------
// - helper -----------------

static void AssignLightShadowID(const Light& _light, ShadowMap& _shadowmap) {
    // generate light data.
    if (_light.CastShadowDirty()) {
        bool wantsShadowId{ _light.GetCastShadow() };
        if (!wantsShadowId) {
            _shadowmap.ReclaimID(_light.GetShadowMapID());
            _light.InvalidateShadowMapID();
            LOG_INFO("clearing light id.");
        }
        else {
            if (!_shadowmap.HasFreeLayers()) {
                LOG_INFO("No free slots for shadow map. Waiting for next update.");
                // early exit, cannot update the var.
                return;
            }
            LOG_INFO("Assigning new id to light.");
            _light.SetShadowMapID(_shadowmap.GenerateLayerID());
        }
        _light.CleanCastShadow();
    }
    if (!_light.GetCastShadow()) return; // ignore anything not asking for it.
    unsigned shadowMapID{ _light.GetShadowMapID() };
    if (!_shadowmap.ValidateID(shadowMapID)) return;
    _shadowmap.SetBoundLayer(shadowMapID);
}


// ---------------------------------------------------------------------------------------------

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
    m_compositor.Init();

    // - shadows -------------------------------
    // TODO -> use the texture manager to create shadow maps.
    SetupShadowProgram();
    SetupShadowBuffers();
}
 
void RenderSystem::PreUpdate() {
    // - aliases -----
    Core& c = Core::GetInstance();
    EntityRegistry& er = c.GetRegistry();
    ResourceManager& resMgr = c.GetResourceManager();
    GPUResourceManager& gpuMgr = c.GetGPUResourceManager();
    
    // - update GPU meshes ----
    MeshManager& mrMgr = c.GetAssetManager().GetMeshManager();
    TextureManager& texMgr = c.GetAssetManager().GetTextureManager();
    ComponentPool<MeshRenderer> mrPool = *er.GetComponentPool<MeshRenderer>();
    for (MeshRenderer& mr : mrPool) {
        MeshHandle itr = mr.GetMesh();
        if (!itr.HandleIsValid()) continue;

        std::shared_ptr<MeshRes> meshRes = mr.GetMesh().Get();
        if (!meshRes || !meshRes->InfoDirty())
            continue;
        
        MeshRes& meshCpu = *meshRes;
        meshCpu.FlagInfoClean();

        if (meshCpu.GetGPUResourceHandle().IsValid()) {
            // not created - create a mesh for the GPU
            // this works for now as long as the buffers are set to dynamic_storage_bit but will break.
            gpuMgr.DeleteResource(meshCpu.GetGPUResourceHandle());
        }
        meshCpu.SetGPUResourceHandle(gpuMgr.CreateMesh(meshCpu));
    }


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
    
    SceneRes& scene = *Core::GetInstance().GetSceneManager().GetCurrentScene().Get();
    SceneRenderData sceneRender = PrepareScene(scene);
    
    // game render /
    for (const Viewport::ViewportID& id : vpRenderOrder) {
        Viewport& currentViewport	{ *viewportMap.at(id ) };
        currentViewport.Update();
        Render(sceneRender, currentViewport); // replace with a single viewport.
    }
}


void RenderSystem::ClearBuffers(const Viewport& _vp) {
    GLuint clearFlags { GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT };
    glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
    glClearDepth(1.0f);
    glClear(clearFlags);



    // clear object id.
    GLuint clearValue = EntityConstants::C_ENTITYID_INVALID;
    glClearBufferuiv(
        GL_COLOR,
        C_RENDER_OBJECTID,
        &clearValue
    );
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

    GLuint clearValue = EntityConstants::C_ENTITYID_INVALID;
    glClearBufferuiv(
        GL_COLOR,
        C_RENDER_OBJECTID,
        &clearValue
    );

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
    //commonUBO.BindBuffer();
    commonUBO.FillBufferData(&m_commonUboData);
}

void RenderSystem::FillObjectUBO(const Entity& entity, const Transform& _trs) {
    UBO& objectUBO = *m_uboManager.GetUBO(DefaultUBOs::DEFAULTBUFFER_OBJECT);
    m_objectUboData.m_objectMatrix = _trs.WorldTransformMtx();
    m_objectUboData.m_position = _trs.Position();
    m_objectUboData.m_objectId = static_cast<GLuint>(entity.GetID().GetID());
    //objectUBO.BindBuffer();
    objectUBO.FillBufferData(&m_objectUboData);
}

void RenderSystem::UnbindViewport(const Viewport& _viewport) {
    if (_viewport.GetRenderTarget()) {
        _viewport.GetRenderTarget()->Unbind();
    }
}

void RenderSystem::UpdateLightingData(const SceneRenderData& _scene) {
    for (const LightRenderable& renderable: _scene.lights) {
        Light& light = const_cast<Light&>(*renderable.light);
        const Transform& trs = *renderable.transform;
        LightData& ld = light.GetLightData();
        ShadowData& sd = light.GetShadowData();
        ld.SetPosition(trs.Position());
        ld.SetDirection(trs.Forward());


        sd.SetShadowID(light.GetShadowMapID());
        sd.SetLightType(light.Type());
    }
}

void RenderSystem::FillLightBufferUBO(const SceneRenderData& _scene) {
    auto& lightlist = _scene.lights;
    m_ldData.m_count = std::min(
        LightConstants::C_MAX_LIGHTS,
        static_cast<int>(lightlist.size())
    );
    for (size_t i = 0; i < m_ldData.m_count; ++i) {
        m_ldData.m_lightData[i] = lightlist[i].light->GetLightData(); // or whatever data source
    }

    UBO& lightBuffer = *m_uboManager.GetUBO(DefaultUBOs::DEFAULTBUFFER_LIGHTS);
    //lightBuffer.BindBuffer();
    lightBuffer.FillBufferData(&m_ldData);
}

void RenderSystem::FillShadowMapUBO(const SceneRenderData& _scene) {
    auto lightList = _scene.lights;
    int shadowCount{ std::min(
        LightConstants::C_MAX_DIRECTIONAL_SHADOWS,
        static_cast<int>(lightList.size()) 
    ) };
    // - directional light data -----------------------------------------------------
    for (int i{}; i < shadowCount; ++i) {
        Light& light = *lightList[i].light;
        if (light.Type() == LightType::DIRECTIONAL) {
            m_smData.m_directionalShadowData[i] = light.GetShadowData();
        }
    }
    glm::vec2 dirLightFBSize{ static_cast<glm::vec2>(m_directionalShadowMaps.GetFramebufferSize()) };
    m_smData.m_directionalAtlasResAndTexelSize = glm::vec4(
        dirLightFBSize.x,
        dirLightFBSize.y,
        1.0f / dirLightFBSize.x,
        1.0f / dirLightFBSize.y
    );
    m_smData.m_directionalCount = m_directionalShadowMaps.GetShadowMapUsageCount();
    // - point light data -----------------------------------------------------------


    UBO& shadowUBO = *m_uboManager.GetUBO(DefaultUBOs::DEFAULTBUFFER_SHADOW);
    //shadowUBO.BindBuffer();
    shadowUBO.FillBufferData(&m_smData);
}


void RenderSystem::Render(SceneRenderData& _scene, const Viewport& _vp) {
    if (!_vp.GetRenderTarget()) return;

    // update data.
    FillCommonUBO(
        glm::inverse(_vp.CameraMatrix()),
        _vp.ProjectionMatrix(),
        _vp.Position(),
        _vp.Forward(),
        static_cast<GLfloat>(Clock::DeltaTime())
    );
    UpdateLightingData(_scene);


    // rendering passes.
    BeginViewportPass(_vp);
    BackgroundRenderPass(_scene, _vp);

    ShadowRenderPass(_scene, _vp);

    FillLightBufferUBO(_scene);
    FillShadowMapUBO(_scene);

    LightingRenderPass(_scene, _vp);
    EndViewportPass(_vp);
}

void RenderSystem::BackgroundRenderPass(SceneRenderData& _scene, const Viewport& _vp) {
    
}

void RenderSystem::ShadowRenderPass(SceneRenderData& _scene, const Viewport& _vp) {
    glm::ivec2 tileSize = m_directionalShadowMaps.GetBaseTileSize();
    glm::ivec2 fbSize = m_directionalShadowMaps.GetFramebufferSize();


    if (!m_directionalShadowMaps.FBO() || !m_directionalShadowMaps.GetTextureID()) {
        LOG_ERROR("No shadow map bound to this light!");
        return;
    }

    // in the shadow pass, all meshes use the SAME material unless it has transparency or some SS nonsense. 
    BindShadowShader();
    // sort and batch render by light types.
    std::array<std::vector<LightRenderable>, 3> lightBuckets;
    for (std::vector<LightRenderable> bucket : lightBuckets) {
        bucket.reserve(_scene.lights.size());
    }

    for (const LightRenderable& lr : _scene.lights) {
        if (!lr.entity.Active() || !lr.entity.IsVisible()) continue;
        Light& light = *lr.light;
        int id = light.Type() == POINT ? 0 : light.Type() == SPOT ? 1 : light.Type() == DIRECTIONAL ? 2 : -1;
        if (id < 0) continue;
        lightBuckets[id].push_back(lr);
    }

    using ShadowRenderFunc = std::function<
        void(const Viewport&, const LightRenderable& , const SceneRenderData&)
    >;
    for (int i{}; i < 3; ++i) {
        ShadowMap* currentShadowMap{};
        ShadowRenderFunc shadowPassFunction;
        switch (i) {
        case 0:
            currentShadowMap = &m_pointLightShadowMaps;
            shadowPassFunction = [this](
                const Viewport& _vp, 
                const LightRenderable& _light, 
                const SceneRenderData& _scene
                ) {
                    this->RenderShadowsPointLight(_vp, _light, _scene);
                };
            break;
        case 1:
            currentShadowMap = &m_spotLightShadowMaps;
            shadowPassFunction = [this](
                const Viewport& _vp,
                const LightRenderable& _light,
                const SceneRenderData& _scene
                ) {
                    this->RenderShadowsSpotLight(_vp, _light, _scene);
                };
            break;
        case 2:
            currentShadowMap = &m_directionalShadowMaps;
            shadowPassFunction = [this](
                const Viewport& _vp,
                const LightRenderable& _light,
                const SceneRenderData& _scene
                ) {
                    this->RenderShadowsDirectionalLight(_vp, _light, _scene);
                };
            break;
        }

        if (!currentShadowMap || !currentShadowMap->IsBuilt() || !shadowPassFunction) continue;

        currentShadowMap->Bind();
        for (const LightRenderable& light : lightBuckets[i]) {
            glClear(GL_DEPTH_BUFFER_BIT);
            shadowPassFunction(_vp, light, _scene);
        }
    }
    ShadowMap::Unbind();
    UnbindShadowShader();
}

void RenderSystem::LightingRenderPass(SceneRenderData& _scene, const Viewport& _vp) {

    _vp.GetRenderTarget()->Bind();
    // - prereqs --------------------------------------------
    const glm::mat4& _cameraMatrix{ glm::inverse(_vp.CameraMatrix()) };
    const glm::mat4& _projectionMatrix{ _vp.ProjectionMatrix() };;
        
    for (const MeshRenderable& renderable : _scene.meshes) {
        if (!renderable.entity.Active() || !renderable.entity.IsVisible()) {
            // to skip hidden ones as well.
            continue;
        }

        const ComponentView<Transform>& trs = renderable.transform;
        const ComponentView<MeshRenderer>& mr = renderable.mesh;
        const glm::mat4 objectTransformMatrix = trs->TransformMtx();

        RES_ID meshId = mr->GetMesh().GetResourceID();
        if (meshId == ResourceConstants::C_RES_INVALID_ID) continue;

        FillObjectUBO(renderable.entity, *trs);
        ResolveMeshRendererMaterials(*mr);
        Render(*mr);
    }

    glBindVertexArray(0);
}

void RenderSystem::RenderShadowsDirectionalLight(
    const Viewport& _vp, 
    const LightRenderable& _light, 
    const SceneRenderData& _scene
) {
    const auto& renderables = _scene.meshes;
    const Light& light = *_light.light;
    AssignLightShadowID(light, m_directionalShadowMaps);
    if (!light.GetCastShadow()) return;
    ShadowData& sdData = const_cast<ShadowData&>(light.GetShadowData());

    // convert light to matrix.
    
    glm::vec3 lightDir{ glm::normalize(_light.transform->Forward()) };
    const glm::vec3& camPos{ _vp.Position() };
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

        float halfLength{ halfLenIncrement * (level + 1) };
        glm::mat4 lightProj = glm::ortho(
            -halfLength, halfLength,
            -halfLength, halfLength,
            0.1f, 2 * halfLength
        );

        // calculate light space matrix.
        glm::mat4 lightSpaceMtx{ lightProj * lightView };
        // textures must be in multiples of 2.
        int tileSizeX = m_directionalShadowMaps.GetBaseTileSize().x >> level;
        int tileSizeY = m_directionalShadowMaps.GetBaseTileSize().y >> level;

        GLint yOffset{ static_cast<GLint>(currentYOffset) };
        glViewport(0, yOffset, tileSizeX, tileSizeY);
        glScissor(0, yOffset, tileSizeX, tileSizeY);

        // - setting shadow data -------------------------------------------------------
        sdData.SetAtlasOffset(glm::vec2(0, currentYOffset), level);
        sdData.SetAtlasSize(glm::vec2(tileSizeX, tileSizeY), level);
        sdData.SetMatrix(lightSpaceMtx, level);

        currentYOffset += tileSizeY;

        for (const MeshRenderable& renderable : renderables) {
            const Entity& meshEntity = renderable.entity;
            const MeshRenderer& mr = *renderable.mesh;
            if (!meshEntity.Active()|| !meshEntity.IsVisible() || !mr.CastShadows()) continue;

            // do something.
            const ComponentView<Transform>& trsMesh = renderable.transform;
            const glm::mat4 objectTransformMatrix = trsMesh->WorldTransformMtx();
            PassLightingMatrices(objectTransformMatrix, lightSpaceMtx);
            FillObjectUBO(meshEntity, *trsMesh);
            Render(mr);
        }
    }
}

void RenderSystem::RenderShadowsPointLight(
    const Viewport& _vp, 
    const LightRenderable& _light, 
    const SceneRenderData& _scene
) {
    const std::vector<MeshRenderable>& renderables = _scene.meshes;
    const Light& light = *_light.light;

    AssignLightShadowID(light, m_pointLightShadowMaps);
    if (!light.GetCastShadow()) return;
    ShadowData& sdData = const_cast<ShadowData&>(light.GetShadowData());

    // convert light to matrix.

    glm::vec3 position = _light.transform->Position();
    std::array<glm::mat4, 6> directions = {
        glm::lookAt(position, position + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)), // -X
        glm::lookAt(position, position + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)), // +X
        glm::lookAt(position, position + glm::vec3(0,-1, 0), glm::vec3(0, 0,-1)), // -Y
        glm::lookAt(position, position + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)), // +Y
        glm::lookAt(position, position + glm::vec3(0, 0,-1), glm::vec3(0, -1, 0)), // -Z
        glm::lookAt(position, position + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0))  // +Z
    };

    int tileSizeDims = m_pointLightShadowMaps.GetBaseTileSize().x;
    for (unsigned side{}; side < 6; ++side) {
        // in order: -x, +x, -y, +y, -z, +z
        glm::mat4 lightProj = glm::perspective(glm::radians(90.f), 1.f, 0.01f, 100.f);
        glm::mat4 lightSpaceMtx{ lightProj * directions[side] };

        // - setting shadow data -------------------------------------------------------
        
        sdData.SetAtlasSize(glm::vec2(tileSizeDims * 6, tileSizeDims), side);
        sdData.SetMatrix(lightSpaceMtx, side);

        int offset = side * tileSizeDims;
        glViewport(offset, 0, tileSizeDims, tileSizeDims);
        glScissor(offset, 0, tileSizeDims, tileSizeDims);

        for (const MeshRenderable& renderable : renderables) {
            const Entity& meshEntity = renderable.entity;
            const MeshRenderer& mr = *renderable.mesh;
            if (!meshEntity.Active() || !meshEntity.IsVisible() || !mr.CastShadows()) continue;

            // do something.
            const ComponentView<Transform>& trsMesh = renderable.transform;
            const glm::mat4 objectTransformMatrix = trsMesh->WorldTransformMtx();
            PassLightingMatrices(objectTransformMatrix, lightSpaceMtx);
            FillObjectUBO(meshEntity, *trsMesh);
            Render(mr);
        }

    }
}

void RenderSystem::RenderShadowsSpotLight(const Viewport& _vp, const LightRenderable& _light, const SceneRenderData& _scene)
{
}

SceneRenderData RenderSystem::PrepareScene(const SceneRes& _sceneRes) {
    SceneRenderData frameData;
    Core& c = Core::GetInstance();
    EntityRegistry& reg = c.GetRegistry();
    for (const EntityNode& node : _sceneRes.GetEntities()) {
        EntityView enttV = reg.GetEntity(node.m_entityId);
        if (!enttV || !enttV->IsVisible()) continue;
        Entity& entt = *enttV;



        // - process to different types --------------------------------
        ComponentView<Transform> trs = entt.GetComponent<Transform>();
        if (!trs) continue;

        ComponentView<MeshRenderer> mesh = entt.GetComponent<MeshRenderer>();
        if (mesh) {
            MeshRenderable renderableData{
                .id = node.m_entityId,
                .entity = entt,
                .transform = trs,
                .mesh = mesh
            };
            frameData.meshes.push_back(renderableData);
            continue;
        }

        ComponentView<Light> light = entt.GetComponent<Light>();
        if (light) {
            // Test light culling here.

            LightRenderable renderableData{
                .id = node.m_entityId,
                .entity = entt,
                .transform = trs,
                .light = light
            };
            frameData.lights.push_back(renderableData);
        }
    }
    return frameData;
}




void RenderSystem::Render(const MeshRenderer& _mr) {
    Core& c = Core::GetInstance();
    std::shared_ptr<const MeshRes> mesh    { _mr.GetMesh().Get() };
    if (!mesh) return;
    GPUResourceManager& gpuResMgr = c.GetGPUResourceManager();
    
    SparseSetView<GPUMesh> gpuMesh = gpuResMgr.GetResource<GPUMesh>(mesh->GetGPUResourceHandle());
    if (!gpuMesh) return;
    for (const GPUSubmesh& submesh : gpuMesh->GetGPUSubmeshList()) {
        submesh.Bind();
        GLsizei idxCount { static_cast<GLsizei>(submesh.GetIndexBufferElementCount()) };
        glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, 0);
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

    ShaderProgramRes shadowShader  {};
    ShaderRes vertexShader         {};
    ShaderRes fragmentShader       {};
    vertexShader.SetShaderCode(vertexShaderSource);
    fragmentShader.SetShaderCode(fragmentShaderSource);
    vertexShader.ShaderType(ShaderConstants::ShaderType::VERTEX);
    fragmentShader.ShaderType(ShaderConstants::ShaderType::FRAG);
    vertexShader.Build();
    fragmentShader.Build();
    shadowShader.SetShader(vertexShader, ShaderConstants::ShaderType::VERTEX);
    shadowShader.SetShader(fragmentShader, ShaderConstants::ShaderType::FRAG);

    
    
    shadowShader.Build();
    
    m_shadowPrg = shadowShader.GetShaderProgramID();
    m_shadowMeshLoc = glGetUniformLocation(m_shadowPrg, ShaderUniformConstants::U_OBJECT_MATRIX);
    m_shadowLightLoc = glGetUniformLocation(m_shadowPrg, ShaderUniformConstants::U_LIGHT_MATRIX);
    LOG_DEBUG("Setting up shadow shader with program id: ["<< m_shadowPrg << "] with mesh uniform location of <"<< m_shadowMeshLoc << "> and light uniform loc of <"<< m_shadowLightLoc<<">");
}

void RenderSystem::SetupShadowBuffers() {
    const unsigned SHADOW_DIR_WH        { 4096 };
    const unsigned SHADOW_SPOT_WH       { 4096 };
    const unsigned SHADOW_POINT_WH      { 512 };
     
    Core& c = Core::GetInstance();

    TextureProperties::TextureProps props{
        .m_internalImageFormat = TextureProperties::TextureFormat::DEPTH32F,
        .m_wrapU = TextureProperties::WrapBehaviour::TO_BORDER,
        .m_wrapV = TextureProperties::WrapBehaviour::TO_BORDER,
        .m_filterMin = TextureProperties::FilterBehaviour::LINEAR,
        .m_filterMag = TextureProperties::FilterBehaviour::LINEAR
    };

    TextureManager& texManager = c.GetAssetManager().GetTextureManager();
    GPUResourceManager& gpuMgr = c.GetGPUResourceManager();

    glm::ivec3 dims{};
    std::shared_ptr<Texture2DArrayRes> ptr = nullptr;


    dims = { SHADOW_DIR_WH, SHADOW_DIR_WH, m_directionalShadowMaps.GetLayers() };
    Texture2DArrayHandle dirTex = texManager.Create2DArrayTexture(dims.x, dims.y, dims.z, props);
    m_directionalShadowMaps.SetBaseTileSize({SHADOW_DIR_WH, SHADOW_DIR_WH});
    ptr = dirTex.Get();
    ptr->SetGPUResourceHandle(gpuMgr.CreateTexture(*ptr));
    m_directionalShadowMaps.SetTexture(dirTex);
    m_directionalShadowMaps.BuildShadowMap(); 


    dims = { SHADOW_POINT_WH * 6, SHADOW_POINT_WH, m_pointLightShadowMaps.GetLayers() };
    Texture2DArrayHandle pointTex = texManager.Create2DArrayTexture(dims.x, dims.y, dims.z, props);
    m_pointLightShadowMaps.SetBaseTileSize({ SHADOW_POINT_WH, SHADOW_POINT_WH });
    ptr = pointTex.Get();
    ptr->SetGPUResourceHandle(gpuMgr.CreateTexture(*ptr));
    m_pointLightShadowMaps.SetTexture(pointTex);
    m_pointLightShadowMaps.BuildShadowMap(); 

}

void RenderSystem::PassLightingMatrices(glm::mat4 _meshMatrix, glm::mat4 _lightMatrix) {
    // glProgramUniformMatrix4fv(m_shadowPrg, m_shadowMeshLoc, 1, GL_FALSE, glm::value_ptr(_meshMatrix));
    // glProgramUniformMatrix4fv(m_shadowPrg, m_shadowLightLoc, 1, GL_FALSE, glm::value_ptr(_lightMatrix));
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
    if (!_mr.GetMesh().HandleIsValid()) return; // no point resolving something can't be seen
    std::vector<MaterialHandle>& materialList = _mr.GetMaterialList();
    if (materialList.empty()) {
        ResolveMaterial(MeshRenderer::GetDefaultMaterial());
        return;
    }

    for (MaterialHandle& matHandle : materialList) {
        //MaterialHandle matHandle = handle;
        auto matPtr = matHandle.Get();
        if (!matPtr) continue;
        MaterialRes& mat = *matPtr;
        ResolveMaterial(mat);
    }
}

void RenderSystem::ResolveMaterial(MaterialRes& _mat) {
    ShaderProgramManager& spr { Core::GetInstance().GetAssetManager().GetShaderProgramManager() };
    using namespace Materials;
    ShadingModel type{ _mat.GetShadingModel() };

    if (type == Materials::ShadingModel::NONE) return;

    std::string shaderProgramAlias{};
    switch (type) {
    case ShadingModel::LAMBERT:
        shaderProgramAlias = ShaderConstants::C_ID_LAMBERTSHADERPROG;
        break;
    case ShadingModel::PHONG:
        shaderProgramAlias = ShaderConstants::C_ID_PHONGSHADERPROG;
        break;
    case ShadingModel::BLINN_PHONG:
        shaderProgramAlias = ShaderConstants::C_ID_BLINNPHONGSHADERPROG;
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
        shaderProgramAlias = ShaderConstants::C_ID_ERRORSHADERPROG;
        break;
    }
    if (shaderProgramAlias.empty()) { 
        // do an unlit unused texture!
        return; 
    }
    

    if (!_mat.IsInitialised()) {
        GLuint shaderId{ ShaderConstants::C_INVALIDSHADERID };
        shaderId = spr.GetShaderProgram(shaderProgramAlias)->GetShaderProgramID();
        _mat.SetShaderProgram(shaderId);
        _mat.Init();
    }
    _mat.UseMaterial();
    _mat.ApplyUniforms();
    // - setup shadow maps ----------------------------
    _mat.ApplyShadowMap(m_directionalShadowMaps.GetTextureID(), 0, 0);
    

}

// ------------------------------------------------------------------------------------------

std::shared_ptr<MaterialRes> RenderSystem::GetMaterial(RES_ID _matId) {
    MaterialManager& mmgr = Core::GetInstance().GetAssetManager().GetMaterialManager();
    if (!mmgr.Has(_matId)) {
        LOG_WARN("Provided mesh id is not a registered material.");
        return nullptr;
    }
    ResourceManager& rsmgr = Core::GetInstance().GetResourceManager();
    auto ptr = static_pointer_cast<MaterialRes>(rsmgr.GetResource(_matId));
    return ptr;
}

std::shared_ptr<const MaterialRes> RenderSystem::GetMaterial(RES_ID _matId) const {
    MaterialManager& mmgr = Core::GetInstance().GetAssetManager().GetMaterialManager();
    if (!mmgr.Has(_matId)) {
        LOG_WARN("Provided mesh id is not a registered material.");
        return nullptr;
    }
    ResourceManager& rsmgr = Core::GetInstance().GetResourceManager();
    auto ptr = static_pointer_cast<MaterialRes>(rsmgr.GetResource(_matId));
    return ptr;
}

std::shared_ptr<MeshRes> RenderSystem::GetMesh(RES_ID _meshId) {
    MeshManager& mmgr = Core::GetInstance().GetAssetManager().GetMeshManager();
    if (!mmgr.Has(_meshId)) {
        LOG_WARN("Provided mesh id is not a registered mesh.");
        return nullptr;
    }
    ResourceManager& rsmgr = Core::GetInstance().GetResourceManager();
    auto ptr = static_pointer_cast<MeshRes>(rsmgr.GetResource(_meshId));
    return ptr;
}

std::shared_ptr<const MeshRes> RenderSystem::GetMesh(RES_ID _meshId) const {
    MeshManager& mmgr = Core::GetInstance().GetAssetManager().GetMeshManager();
    if (!mmgr.Has(_meshId)) {
        LOG_WARN("Provided mesh id is not a registered mesh.");
        return nullptr;
    }
    ResourceManager& rsmgr = Core::GetInstance().GetResourceManager();
    auto ptr = static_pointer_cast<MeshRes>(rsmgr.GetResource(_meshId));
    return ptr;
}
