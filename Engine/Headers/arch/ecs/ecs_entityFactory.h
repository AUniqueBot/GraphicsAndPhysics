#pragma once
#include <arch/ecs/ecs_registry.h>
#include <arch/resources/res_scene.h>
#include <arch/resources/res_assetManager.h>
#include <arch/resources/res_sceneManager.h>

class EntityFactory {
public:
    explicit EntityFactory(SceneManager& _scm, AssetManager& _assetManager)
        : m_sceneManager(_scm), m_assetManager(_assetManager) {
    }

    EntityView CreateGameObject(const glm::vec3& pos = glm::vec3());

    // - basic primitives -----------------------------------------
    EntityView CreateCube(const glm::vec3& pos = glm::vec3());
    EntityView CreateSphere(const glm::vec3& pos = glm::vec3());
    EntityView CreatePlane(const glm::vec3& pos = glm::vec3());

    EntityView CreateDefaultCube(const glm::vec3& _pos = glm::vec3());
    EntityView CreateDefaultSphere(const glm::vec3& _pos = glm::vec3());
    EntityView CreateDefaultPlane(const glm::vec3& _pos = glm::vec3());


    EntityView CreatePointLight(
        const glm::vec3& pos = glm::vec3(), 
        const float& _intensity = 1.f, 
        const glm::vec4& _col = glm::vec4()
    );

    EntityView CreateAmbientLight(
        const glm::vec3& pos = glm::vec3(),
        const float& _intensity = 1.f,
        const glm::vec4& _col = glm::vec4()
    );

    EntityView CreateDirectionalLight(
        const glm::vec3& pos = glm::vec3(),
        const float& _intensity = 1.f,
        const glm::vec4& _col = glm::vec4()
    );


private:
    //EntityRegistry& m_registry;
    AssetManager& m_assetManager;
    SceneManager& m_sceneManager;

};