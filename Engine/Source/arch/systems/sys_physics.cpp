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
	
	
	auto rbPoolView = er.GetComponentPool<RigidBody>();

	if (!rbPoolView) return;

	auto& rbPool = *rbPoolView;
	
	if (rbPool.size() == 0) return;

	// rb pass here.
	_out.rbEntityList.reserve(rbPool.size());
	for (RigidBody& rb : rbPool) {
		EntityID id = rb.GetEntityID();
		ComponentView<Transform> trsView = er.GetEntity(id)->GetComponent<Transform>();
		if (trsView) {
			RBData data {
				.id = id,
				.trs = *trsView,
				.rb = ComponentView<RigidBody>(rb)
			};
			_out.rbEntityList.emplace_back(data);
		}
	}

	// collider pass here.
	_out.rbEntityList.shrink_to_fit();
	_out.collideEntityList.shrink_to_fit();

}

void PhysicsSystem::EnvironmentPass(PhysicsScene& _scene) {

	auto& rbList = _scene.rbEntityList;
	if (rbList.empty()) return;

	glm::vec3 gravity = m_physicsEnv.Gravity();
	gravity *= _scene.dt;
	
	auto parallelFunction = [gravity](RBData& _data) {
		auto& [id, trs, rb] = _data;
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
	auto parallelFunction = [dt](RBData& _data) {
		auto& [id, trs, rb] = _data; // destructuring.

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


