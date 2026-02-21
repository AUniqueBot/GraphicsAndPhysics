#pragma once
#include <pch.h>
#include <arch/common/system.h>





class PhysicsSystem final : public System, public Singleton<PhysicsSystem> {

public:
	void Init() override;
	void Start() override { LOG_INFO("Physics Start"); }


	void PreUpdate()	override;
	void Update()		override;
	void Stop()			override { LOG_INFO("Stop"); };
	void Cleanup()		override { LOG_INFO("Cleanup"); };

public:



	// do ur physx.
	

	// need a registry for collisions

};