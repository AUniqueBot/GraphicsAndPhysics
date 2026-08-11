#pragma once
#include <arch/ecs/ecs_registry.h>
#include <arch/resources/res_assetManager.h>

class EntityFactory {
public:
    explicit EntityFactory(EntityRegistry& registry, AssetManager& _assetManager)
        : m_registry(registry), m_assetManager(_assetManager) {
    }

    EntityView CreateGameObject(const glm::vec3& pos = glm::vec3());

    // - basic primitives -----------------------------------------
    EntityView CreateCube(const glm::vec3& pos = glm::vec3());
    EntityView CreateSphere(const glm::vec3& pos = glm::vec3());
    EntityView CreatePlane(const glm::vec3& pos = glm::vec3());


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
    EntityRegistry& m_registry;
    AssetManager& m_assetManager;

};