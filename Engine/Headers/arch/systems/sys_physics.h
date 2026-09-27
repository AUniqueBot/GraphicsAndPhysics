#pragma once
#include <pch.h>
#include <arch/common/system.h>
#include <arch/systems/sys_physics_modules/sys_physics_environmentValues.h>

#include <arch/components/comp_rigidbody.h>
#include <arch/components/comp_transform.h>
#include <arch/components/comp_collider.h>
#include <arch/components/comp_collider/comp_collider_collisions.h>




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
	struct PhysicsObjectData {
		EntityID id;
		Transform& trs;
		ComponentView<RigidBody> rb;
		ComponentView<Collider> col;
	};

	struct CollisionData {
		std::pair<PhysicsObjectData, PhysicsObjectData> pair;
		std::vector<CollisionInfo> collisioninfo;
	};

	struct PhysicsScene {
		std::vector<PhysicsObjectData> rbEntityList;
		std::vector<PhysicsObjectData> collideEntityList;
		std::vector<std::pair<PhysicsObjectData, PhysicsObjectData>> collisionPairs;
		std::vector<CollisionData> collisionData;

		double dt;
	};


private:
	void PrepareScene(PhysicsScene& _out);
	void EnvironmentPass(PhysicsScene& _scene);
	void RigidbodyPass(PhysicsScene& _scene);

	// examples to go through

	void BroadPhaseCollisionPass(PhysicsScene& _scene);
	void NarrowPhaseCollisionPass(PhysicsScene& _scene); // need the atlas
private:
	PhysicsEnvironmentSetup m_physicsEnv;

	static CollisionTestFunctionDB s_collisionFunctionDb;
};

// return a group of pairs