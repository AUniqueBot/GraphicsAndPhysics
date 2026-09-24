#include <arch/systems/sys_physics_modules/sys_physics_environmentValues.h>




const glm::vec3& PhysicsEnvironmentSetup::Gravity() const {
    return m_gravity;
}

void PhysicsEnvironmentSetup::Gravity(const glm::vec3& _gravity) {
    m_gravity = _gravity;
}
