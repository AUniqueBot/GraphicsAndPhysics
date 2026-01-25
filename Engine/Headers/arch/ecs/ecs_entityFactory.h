#pragma once
#include <arch/ecs/ecs_registry.h>

class EntityFactory {
public:
    explicit EntityFactory(EntityRegistry& registry)
        : m_registry(registry) {
    }

    EntityView CreateCube(const glm::vec3& pos = glm::vec3());
    EntityView CreateSphere(const glm::vec3& pos = glm::vec3());
    EntityView CreatePlane(const glm::vec3& pos = glm::vec3());




private:
    EntityRegistry& m_registry;
};