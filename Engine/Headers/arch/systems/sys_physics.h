#pragma once
#include <pch.h>
#include <arch/common/system.h>
#include <arch/systems/sys_physics_modules/sys_physics_environmentValues.h>

#include <arch/components/comp_rigidbody.h>
#include <arch/components/comp_transform.h>




class PhysicsSystem final : public System, public Singleton<PhysicsSystem> {

public:
	void Init() override;
	void Start() override { LOG_INFO("Physics Start"); }


	void PreUpdate()	override;
	void Update()		override;
	void FixedUpdate()	override;
	void Stop()			override { LOG_INFO("Stop"); };
	void Cleanup()		override { LOG_INFO("Cleanup"); };

public:



	// do ur physx.
	// need a registry for collisions
	PhysicsEnvironmentSetup& PhysicsEnvironment();
	const PhysicsEnvironmentSetup& PhysicsEnvironment() const;


private:
	struct RBData {
		EntityID id;
		Transform& trs;
		ComponentView<RigidBody> rb;
		// ComponentView<Collider> col;
	};

	struct PreparedScene {
		std::vector<RBData> rbEntityList;
		std::vector<RBData> collideEntityList;
	};


private:
	PreparedScene PrepareScene();
	void EnvironmentPass(double _dt, PreparedScene& _scene);
	void RigidbodyPass(double _dt, PreparedScene& _scene);
	void CollisionPass(double _dt, PreparedScene& _scene);
	void BroadPhaseCollisionPass(double _dt);
	// examples to go through
	// 
	void NarrowPhaseCollisionPass(); // need the atlas
private:
	PhysicsEnvironmentSetup m_physicsEnv;
};