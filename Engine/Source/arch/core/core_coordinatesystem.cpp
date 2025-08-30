#include <pch.h>
#include <arch/core/core_coordinatesystem.h>
#include <arch/ecs/ecs_fwdDecl_entityRegistry.h>
#include <arch/components/comp_transform.h>


glm::vec3 CoordinateSystem::GetWorldUp() const {
    return glm::vec3(
        m_world_up == X_POS ? 1 : m_world_up == X_NEG ? -1 : 0,
        m_world_up == Y_POS ? 1 : m_world_up == Y_NEG ? -1 : 0,
        m_world_up == Z_POS ? 1 : m_world_up == Z_NEG ? -1 : 0
    );
}

inline glm::vec3 CoordinateSystem::GetWorldForward() const {
    return glm::vec3(
        m_world_forward == X_POS ? 1 : m_world_forward == X_NEG ? -1 : 0,
        m_world_forward == Y_POS ? 1 : m_world_forward == Y_NEG ? -1 : 0,
        m_world_forward == Z_POS ? 1 : m_world_forward == Z_NEG ? -1 : 0
    );
}

glm::vec3 CoordinateSystem::GetLocalUp(const Entity& _entity) const {
    ComponentView<Transform> trs = _entity.GetComponent<Transform>();
    if (!trs) return glm::vec3();
    return trs->Up();
}

glm::vec3 CoordinateSystem::GetLocalFowrard(const Entity& _entity) const {
    ComponentView<Transform> trs = _entity.GetComponent<Transform>();

    if (!trs) return glm::vec3();
    return trs->Forward();
}
