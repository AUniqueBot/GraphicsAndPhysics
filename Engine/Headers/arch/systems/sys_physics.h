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

	// structs for preparation.
	struct RBData {
		EntityID id;
		Transform& trs;
		ComponentView<RigidBody> rb;
		// ComponentView<Collider> col;
	};

	struct PhysicsScene {
		std::vector<RBData> rbEntityList;
		std::vector<RBData> collideEntityList;
		double dt;
	};


private:
	void PrepareScene(PhysicsScene& _out);
	void EnvironmentPass(PhysicsScene& _scene);
	void RigidbodyPass(PhysicsScene& _scene);
	void CollisionPass(PhysicsScene& _scene);
	void BroadPhaseCollisionPass(double _dt);
	// examples to go through
	// 
	void NarrowPhaseCollisionPass(); // need the atlas
private:
	PhysicsEnvironmentSetup m_physicsEnv;
};