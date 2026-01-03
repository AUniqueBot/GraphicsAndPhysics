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
#include <util/util_ostreamOverrides.h>
#include <util/util_logging.h>
#include <util/util_graphics_debugging.h>

// Some other todos - make a shader editor! How hard can it be? :')





void GLAPIENTRY
MessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    // GL CALLBACK: ** GL ERROR ** type = 0x824c, severity = 0x9146, message = GL_INVALID_OPERATION error generated. <location> is invalid.
    std::string msg(message);
    std::set<std::string> blackListSet = {
        "<location> is invalid",
        "Buffer detailed info"
    };

    for (const std::string& str : blackListSet) {
        if (msg.find(str) != std::string::npos) {
            return;
        }
    }

    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
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
    viewport.SetRenderTarget(std::make_shared<RenderTarget>(renderTarget));


    SetupGLDebug();
    m_uboManager.Init();
    m_uboManager.CreateUBO(UBO::LIGHTS, sizeof(LightUBOData));

    m_vaoManager.Init();

}

void RenderSystem::PreUpdate() {
    // clear the buffer.
    //LOG_INFO("Run PreUpdate");
}


void RenderSystem::Update() {
    glm::mat4 dummy{};
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


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_SCISSOR_TEST);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_SCISSOR_TEST); // assuming the engine doesn't need overlays. then agin, can be alleviated with a render order test.
    for (const Viewport::ViewportID& id : vpRenderOrder) {
        Viewport& currentViewport	{ *viewportMap.at(id ) };
        currentViewport.Update();
        Render(currentViewport); // replace with a single viewport.
    }	
}





void RenderSystem::Render(const Viewport& _viewport) {
    auto GetError = GraphicsDebug::GetError;

    const glm::mat4& _cameraMatrix			{ glm::inverse(_viewport.CameraMatrix()) };
    const glm::mat4 & _projectionMatrix		{ _viewport.ProjectionMatrix() };;
    
    EntityRegistry& registry = Core::GetInstance().Registry();
    // use the current camera for projection matrix.



    if (_viewport.GetRenderTarget()) {
        _viewport.GetRenderTarget()->Bind();
        glViewport(0,0, _viewport.ViewportDimensions().x, _viewport.ViewportDimensions().y);
    }

    GLuint clearFlags{ GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT };
    
    // viewport set.
    glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
    glClear(clearFlags);
    glClearDepth(1.0f);


    // rendering
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glm::mat4 pos, rot, scl{ 1.f };
    glm::mat4 viewMtx{ 1.f };
    pos = glm::translate(glm::mat4{ 1.f }, glm::vec3());
    rot = glm::mat4{ 1.f };

    std::vector<LightData> culledLights = CullLights(_viewport);
    // set into UBO here.
    
    LightUBOData lightData{};
    lightData.m_count = std::min(C_MAX_LIGHTS, static_cast<unsigned>(culledLights.size()));
    for (size_t i = 0; i < lightData.m_count; ++i) {
        lightData.m_lightData[i] = culledLights[i]; // or whatever data source
    }

    UBO& lightBuffer = *(m_uboManager.GetUBO(UBO::LIGHTS));
    lightBuffer.FillBufferData(&lightData);


    auto& entityList = registry.GetEntityList();
    for (Entity& e : entityList) {
        if (!e.Active() || !e.IsVisible()) {

            // to skip hidden ones as well.
            continue;
        }


        const auto& mr = e.GetComponent<MeshRenderer>();
        if (!mr) { continue; }
        
        auto trs = e.GetComponent<Transform>();
        const glm::mat4 objectTransformMatrix = trs->LocalTransformMtx();

        std::shared_ptr<Mesh> mesh = mr->GetMesh();
        if (!mesh) continue;
        VAOHandler* vaoHandler{ m_vaoManager.GetVAO(mesh->VAOIdentifier()) };

        if (!vaoHandler) continue;
        VAOHandler& currentVAO = *vaoHandler;
        currentVAO.BindVAO();
        //currentVAO.LogDebug();
        currentVAO.UseMesh(*mesh);
        // check if there is data here...

        mr->Render(
            objectTransformMatrix,
            _projectionMatrix,
            _cameraMatrix
        );


        //glBindTexture(GL_TEXTURE_2D, 0);


        m_vaoManager.UnbindVAO();
    }
       
    glBindVertexArray(0);

    if (_viewport.GetRenderTarget()) {
        _viewport.GetRenderTarget()->Unbind();
    }

    //LOG_SPLITTER();
}



ViewportManager& RenderSystem::GetViewportManager() {
    return m_viewportManager;
}
const ViewportManager& RenderSystem::GetViewportManager() const {
    return m_viewportManager;
}


std::vector<LightData> RenderSystem::CullLights(const Viewport& _viewport) {

    std::vector<LightData> potentialLights{};

    EntityRegistry& registry = Core::GetInstance().Registry();
    const auto& lightPoolRef = registry.GetComponentPool<Light>();

    if (!registry.ComponentPoolExists<Light>()) {
        LOG_WARN("Light Component Pool does not exist");
        return std::vector<LightData> {};
    }


    const ComponentPool<Light>& lightPool = *lightPoolRef;
    const unsigned lightCount = m_maxLightCount;
    const auto& lightComponentData = lightPool.Data();

        
    for (const auto& light : lightComponentData) {

        EntityView entity = registry.Get(light.GetEntityID());
        if (!entity || !entity->Active()) continue;


        //
        if (!LightCollisionTest(light, _viewport)) {
            // light does not appear or cause an effect in the camera frustum.
            continue;
        }

        ComponentView<Transform> trs = entity->GetComponent<Transform>();
        
        LightData lightData = light.GetLightData();
        glm::vec3 position = trs->Position();
        lightData.SetPosition(position);
        // direction of 0, -1, 0, * quat.
        lightData.SetDirection(trs->Forward());



        potentialLights.push_back(lightData);
    }
    return potentialLights;
}



bool RenderSystem::LightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const {
    bool testCase = false;
    switch (_lightComponent.Type()){
    case POINT:
        testCase = PointLightCollisionTest(_lightComponent, _viewport);
        break;
    case AREA:
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
    case SUN:
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


