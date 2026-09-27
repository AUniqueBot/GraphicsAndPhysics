#include <pch.h>
#include <arch/core.h>
#include <arch/systems/sys_physics.h>


//// test function I got from chatgpt
//template<typename T, typename Function>
//void ParallelFor(std::vector<T>& _data, Function&& _function) {
//	const size_t count = _data.size();
//
//	if (count == 0)
//		return;
//
//	const unsigned int threadCount = std::min(
//		std::thread::hardware_concurrency(),
//		static_cast<unsigned int>(count)
//	);
//
//	std::vector<std::thread> threads;
//	threads.reserve(threadCount);
//
//	const size_t chunkSize = (count + threadCount - 1) / threadCount;
//
//	for (unsigned int threadIndex = 0; threadIndex < threadCount; ++threadIndex) {
//		const size_t begin = threadIndex * chunkSize;
//		const size_t end = std::min(begin + chunkSize, count);
//
//		if (begin >= end)
//			break;
//
//		threads.emplace_back([&, begin, end]() {
//			for (size_t i = begin; i < end; ++i)
//				_function(_data[i]);
//			});
//	}
//
//	for (std::thread& thread : threads)
//		thread.join();
//}

CollisionTestFunctionDB PhysicsSystem::s_collisionFunctionDb = {
	{ 
		{ ColliderShapeConstants::Type::AABB, ColliderShapeConstants::Type::AABB }, 
		&CollisionFunction::TestDiscreteAABB_AABB 
	},
	{ 
		{ ColliderShapeConstants::Type::Sphere, ColliderShapeConstants::Type::Sphere }, 
		&CollisionFunction::TestDiscreteSphere_Sphere 
	},
	{
		{ ColliderShapeConstants::Type::AABB, ColliderShapeConstants::Type::Sphere },
		&CollisionFunction::TestDiscreteAABB_Sphere
	},
};



void PhysicsSystem::Init() {
	// a job system!
}

void PhysicsSystem::PreUpdate() {

}

void PhysicsSystem::Update() {

}

void PhysicsSystem::FixedUpdate() {


	PhysicsScene scene{};
	PrepareScene(scene);
	EnvironmentPass(scene);
	RigidbodyPass(scene);
	// per collider collisions

	BroadPhaseCollisionPass(scene);
	NarrowPhaseCollisionPass(scene);

}

PhysicsEnvironmentSetup& PhysicsSystem::PhysicsEnvironment() {
	return m_physicsEnv;
}

const PhysicsEnvironmentSetup& PhysicsSystem::PhysicsEnvironment() const {
	return m_physicsEnv;
}


void PhysicsSystem::PrepareScene(PhysicsScene& _out) {

	EntityRegistry& er = Core::GetInstance().GetRegistry();
	_out.dt = Core::GetInstance().FixedDeltaTime();
	
	

	// rb preparation does not care about collisions
	auto rbPoolView = er.GetComponentPool<RigidBody>();
	if (rbPoolView) {
		auto& rbPool = *rbPoolView;
		// rb pass here.
		_out.rbEntityList.reserve(rbPool.size());
		for (RigidBody& rb : rbPool) {
			EntityID id = rb.GetEntityID();
			ComponentView<Transform> trsView = er.GetEntity(id)->GetComponent<Transform>();
			if (!trsView) continue;

			PhysicsObjectData data{
				.id = id,
				.trs = *trsView,
				.rb = ComponentView<RigidBody>(rb),
				.col = ComponentView<Collider>(std::nullopt)
			};
			_out.rbEntityList.emplace_back(data);
		}
		_out.rbEntityList.shrink_to_fit();
	}

	// collider preparation needs to worry about rb for collisions.
	auto colPoolView = er.GetComponentPool<Collider>();
	if (colPoolView) {
		auto& colPool = *colPoolView;
		_out.collideEntityList.reserve(colPool.size());
		for (Collider& collider : colPool) {
			EntityID id = collider.GetEntityID();
			Entity& entity = *er.GetEntity(id);

			ComponentView<Transform> trsView = entity.GetComponent<Transform>();
			if (!trsView) continue;
			ComponentView<RigidBody> rbView = entity.GetComponent<RigidBody>();
			PhysicsObjectData data {
				.id = id,
				.trs = *trsView,
				.rb = rbView,
				.col = ComponentView<Collider>(collider)
			};
			_out.collideEntityList.emplace_back(data);
		}
		_out.collideEntityList.shrink_to_fit();
	}
}

void PhysicsSystem::EnvironmentPass(PhysicsScene& _scene) {

	auto& rbList = _scene.rbEntityList;
	if (rbList.empty()) return;

	glm::vec3 gravity = m_physicsEnv.Gravity();
	gravity *= _scene.dt;
	
	auto parallelFunction = [gravity](PhysicsObjectData& _data) {
		auto& [id, trs, rb, _] = _data;
		if (rb->Gravity()) {
			rb->LinearVelocity(rb->LinearVelocity() + gravity);
		}
	};


	std::for_each(
		rbList.begin(),
		rbList.end(),
		parallelFunction
	);
	
}

void PhysicsSystem::RigidbodyPass(PhysicsScene& _scene) {
	auto& rbList = _scene.rbEntityList;
	if (rbList.empty()) return;
	
	float dt = static_cast<float>(_scene.dt);
	auto parallelFunction = [dt](PhysicsObjectData& _data) {
		auto& [id, trs, rb, _] = _data; // destructuring.

		// half implicit euler.
		rb->LinearVelocity(rb->LinearVelocity() + rb->LinearAcceleration() * dt);
		trs.Position(trs.Position() + rb->LinearVelocity() * dt);
	};


	

	std::for_each(
		rbList.begin(), 
		rbList.end(), 
		parallelFunction
	);


}

void PhysicsSystem::BroadPhaseCollisionPass(PhysicsScene& _scene) {
	auto& colliderList = _scene.collideEntityList;
	size_t pairlistsize = colliderList.size();
	pairlistsize *= pairlistsize;
	pairlistsize /= 2;
	
	// atm just getting all possible pairs.
	_scene.collisionPairs.reserve(pairlistsize);
		using namespace ColliderConstants;
	for (size_t i { 0 }; i < colliderList.size(); ++i) {
		for (size_t j { i + 1 }; j < colliderList.size(); ++j) {
			
			// ignore static to static collisions.
			if (
				colliderList[i].col->MotionType() == MotionType::Static &&
				colliderList[j].col->MotionType() == MotionType::Static
				) continue;


			std::pair<PhysicsObjectData, PhysicsObjectData> pair{
				colliderList[i],
				colliderList[j]
			};
			_scene.collisionPairs.emplace_back(pair);
		}
	}
	_scene.collisionPairs.shrink_to_fit();
}

void PhysicsSystem::NarrowPhaseCollisionPass(PhysicsScene& _scene) {
	// get the collider type and 
	auto& collisionPairs = _scene.collisionPairs;
	_scene.collisionPairs.reserve(collisionPairs.size());

	for (auto& [rbdata1, rbdata2] : collisionPairs) {
		auto& colliderList1 = rbdata1.col->GetColliderList();
		auto& colliderList2 = rbdata2.col->GetColliderList();

		ColliderInfo info1{};
		ColliderInfo info2{};

		info1.position = rbdata1.trs.Position();
		info1.rotation = rbdata1.trs.Rotation();

		info2.position = rbdata2.trs.Position();
		info2.rotation = rbdata2.trs.Rotation();



		std::vector<CollisionInfo> collideData;
		for (auto& collider1 : colliderList1) {
			info1.collider = collider1.get();

			for (auto& collider2 : colliderList2) {
				std::pair<
					ColliderShapeConstants::Type, 
					ColliderShapeConstants::Type
				> collisionPair{
					collider1->ColliderType(),
					collider2->ColliderType()
				};
				if (!s_collisionFunctionDb.contains(collisionPair)) continue;
				CollisionTestFunction func = s_collisionFunctionDb[collisionPair];
				info2.collider = collider2.get();
				CollisionInfo info = func(info1, info2);

				if (info.m_collide) {
					collideData.push_back(info);
				}
			}
		}


		if (!collideData.empty()) {
			CollisionData data{
				.pair = {rbdata1, rbdata2},
				.collisioninfo = collideData
			};
			_scene.collisionData.emplace_back(data);
		}


		
		/*
			for collider in data1
				for collider in data2
					collisionfunction call.
		
		*/
		
		
		// collisionFunction(a, b);
	}
	_scene.collisionData.shrink_to_fit();
}