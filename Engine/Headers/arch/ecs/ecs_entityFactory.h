#pragma once
#include <arch/ecs/ecs_registry.h>

class EntityFactory {
public:
    explicit EntityFactory(EntityRegistry& registry)
        : m_registry(registry) {
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

};